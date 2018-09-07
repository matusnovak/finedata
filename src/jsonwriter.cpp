#include <json.h>
#include <ffw/data/jsonwriter.h>

struct ffw::JsonWriter::Cache {
    json_object* elem = nullptr;
    bool isArray = false;
};

void ffw::JsonWriter::json_object_deleter::operator() (json_object* v) const {
    json_object_put(v);
}

ffw::JsonWriter::JsonWriter() {
}

ffw::JsonWriter::JsonWriter(JsonWriter&& other) NOEXCEPT {
    swap(other);
}

ffw::JsonWriter& ffw::JsonWriter::operator = (JsonWriter&& other) NOEXCEPT {
    if (this != &other) {
        swap(other);
    }
    return *this;
}

void ffw::JsonWriter::swap(JsonWriter& other) NOEXCEPT {
    std::swap(root, other.root);
    std::swap(cache, other.cache);
}

ffw::JsonWriter::~JsonWriter() {
    close();
}

void ffw::JsonWriter::close() {
    root.reset();
    cache.clear();
}

bool ffw::JsonWriter::isOpen() const {
    return false;
}

void ffw::JsonWriter::stepOut() {
    /*if (keys.empty()) throw JsonWriterException("can not step out any further");

    if (isArray.back()) {
        isArray.pop_back();
        keys.pop_back();
        return;
    }

    printer->CloseElement();
    keys.pop_back();
    isArray.pop_back();*/

    if (cache.empty()) throw JsonWriterException("can not step out any further");
    cache.pop_back();
}

#define IS_EMPTY JsonWriterException("can not add value to empty json object");
#define IS_NOT_ARRAY JsonWriterException("can not add array element without a key into an object")
#define IS_ARRAY JsonWriterException("can not add key value pair element into an array")

void ffw::JsonWriter::add(const std::string& key, const Node& value) {
    switch (value.getType()) {
        case Node::Type::FLOAT: add(key, value.getAsFloat()); break;
        case Node::Type::INTEGER: add(key, value.getAsInt()); break;
        case Node::Type::BOOLEAN: add(key, value.getAsBool()); break;
        case Node::Type::NULLVAL: add(key, nullptr); break;
        case Node::Type::STRING: add(key, value.getAsString()); break;
        case Node::Type::ARRAY: {
            addArray(key);
            for (const auto& pair : value.getAsArray()) {
                add(pair);
            }
            stepOut();
            break;
        }
        case Node::Type::OBJECT: {
            addObject(key);
            for (const auto& pair : value.getAsObject()) {
                add(pair.first, pair.second);
            }
            stepOut();
            break;
        }
        default: break;
    }
}

void ffw::JsonWriter::add(const std::string& key, const Node::Integer value) {
    if (cache.empty()) throw IS_EMPTY;
    if (cache.back().isArray) throw IS_ARRAY;
    const auto obj = json_object_new_int64(value);
    json_object_object_add(cache.back().elem, key.c_str(), obj);
}

void ffw::JsonWriter::add(const std::string& key, const Node::Float value) {
    if (cache.empty()) throw IS_EMPTY;
    if (cache.back().isArray) throw IS_ARRAY;
    const auto obj = json_object_new_double(value);
    json_object_object_add(cache.back().elem, key.c_str(), obj);
}

void ffw::JsonWriter::add(const std::string& key, const Node::Boolean value) {
    if (cache.empty()) throw IS_EMPTY;
    if (cache.back().isArray) throw IS_ARRAY;
    const auto obj = json_object_new_boolean(value ? 1 : 0);
    json_object_object_add(cache.back().elem, key.c_str(), obj);
}

void ffw::JsonWriter::add(const std::string& key, const Node::String& value) {
    if (cache.empty()) throw IS_EMPTY;
    if (cache.back().isArray) throw IS_ARRAY;
    const auto obj = json_object_new_string_len(value.c_str(), value.size());
    json_object_object_add(cache.back().elem, key.c_str(), obj);
}

void ffw::JsonWriter::add(const std::string& key, const Node::Null value) {
    (void)value;
    if (cache.empty()) throw IS_EMPTY;
    if (cache.back().isArray) throw IS_ARRAY;
    json_object_object_add(cache.back().elem, key.c_str(), nullptr);
}

void ffw::JsonWriter::addArray(const std::string& key) {
    const auto obj = json_object_new_array();

    if (!cache.empty()) {
        auto& last = cache.back();
        if (!last.isArray) {
            json_object_object_add(last.elem, key.c_str(), obj);
        }
        else {
            json_object_put(obj);
            throw IS_ARRAY;
        }
    }

    cache.emplace_back();
    auto& child = cache.back();
    child.elem = obj;
    child.isArray = true;
}

void ffw::JsonWriter::addObject(const std::string& key) {
    const auto obj = json_object_new_object();

    if (!cache.empty()) {
        auto& last = cache.back();
        if (!last.isArray) {
            json_object_object_add(last.elem, key.c_str(), obj);
        }
        else {
            json_object_put(obj);
            throw IS_ARRAY;
        }
    }

    cache.emplace_back();
    auto& child = cache.back();
    child.elem = obj;
    child.isArray = false;
}

void ffw::JsonWriter::add(const Node& value) {
    switch (value.getType()) {
        case Node::Type::FLOAT: add(value.getAsFloat()); break;
        case Node::Type::INTEGER: add(value.getAsInt()); break;
        case Node::Type::BOOLEAN: add(value.getAsBool()); break;
        case Node::Type::NULLVAL: add(nullptr); break;
        case Node::Type::STRING: add(value.getAsString()); break;
        case Node::Type::ARRAY: {
            addArray();
            for (const auto& pair : value.getAsArray()) {
                add(pair);
            }
            stepOut();
            break;
        }
        case Node::Type::OBJECT: {
            addObject();
            for (const auto& pair : value.getAsObject()) {
                add(pair.first, pair.second);
            }
            stepOut();
            break;
        }
        default: break;
    }
}

void ffw::JsonWriter::add(const Node::Integer value) {
    if (cache.empty()) throw IS_EMPTY;
    if (!cache.back().isArray) throw IS_NOT_ARRAY;
    const auto obj = json_object_new_int64(value);
    json_object_array_add(cache.back().elem, obj);
}

void ffw::JsonWriter::add(const Node::Float value) {
    if (cache.empty()) throw IS_EMPTY;
    if (!cache.back().isArray) throw IS_NOT_ARRAY;
    const auto obj = json_object_new_double(value);
    json_object_array_add(cache.back().elem, obj);
}

void ffw::JsonWriter::add(const Node::Boolean value) {
    if (cache.empty()) throw IS_EMPTY;
    if (!cache.back().isArray) throw IS_NOT_ARRAY;
    const auto obj = json_object_new_boolean(value ? 1 : 0);
    json_object_array_add(cache.back().elem, obj);
}

void ffw::JsonWriter::add(const Node::String& value) {
    if (cache.empty()) throw IS_EMPTY;
    if (!cache.back().isArray) throw IS_NOT_ARRAY;
    const auto obj = json_object_new_string_len(value.c_str(), value.size());
    json_object_array_add(cache.back().elem, obj);
}

void ffw::JsonWriter::add(const Node::Null value) {
    (void)value;
    if (cache.empty()) throw IS_EMPTY;
    if (!cache.back().isArray) throw IS_NOT_ARRAY;
    json_object_array_add(cache.back().elem, nullptr);
}

void ffw::JsonWriter::addArray() {
    const auto obj = json_object_new_array();
    if (!root) root.reset(obj);

    if (!cache.empty()) {
        auto& last = cache.back();
        if (last.isArray) {
            json_object_array_add(last.elem, obj);
        } else {
            json_object_put(obj);
            throw IS_NOT_ARRAY;
        }
    }

    cache.emplace_back();
    auto& child = cache.back();
    child.elem = obj;
    child.isArray = true;
}

void ffw::JsonWriter::addObject() {
    const auto obj = json_object_new_object();
    if (!root) root.reset(obj);

    if (!cache.empty()) {
        auto& last = cache.back();
        if (last.isArray) {
            json_object_array_add(last.elem, obj);
        }
        else {
            json_object_put(obj);
            throw IS_NOT_ARRAY;
        }
    }

    cache.emplace_back();
    auto& child = cache.back();
    child.elem = obj;
    child.isArray = false;
}

std::string ffw::JsonWriter::str() const {
    if (!root) throw JsonWriterException("not initialized");
    return std::string(json_object_to_json_string_ext(root.get(), JSON_C_TO_STRING_PRETTY));
}

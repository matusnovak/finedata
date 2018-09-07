#include <json.h>
#include <fstream>
#include <streambuf>
#include <ffw/data/jsonreader.h>

ffw::JsonIterator::JsonIterator() :elem(nullptr), key(nullptr), type(-1) {

}

ffw::JsonIterator::JsonIterator(json_object* elem, const char* key, int type):elem(elem), key(key), type(type) {

}

std::string ffw::JsonIterator::getKey() const {
    if (type >= 0 && key) return key ? std::string(key) : "";
    throw JsonReaderException("empty node");
}

ffw::Node ffw::JsonIterator::get(const bool attrIgnore) {
    (void)attrIgnore;
    switch (type) {
        case json_type_null: return Node(nullptr);
        case json_type_int: return Node(toInteger());
        case json_type_double: return Node(toFloat());
        case json_type_boolean: return Node(toBoolean());
        case json_type_string: return Node(toString());
        case json_type_array: return Node(toArray());
        case json_type_object: return Node(toObject());
        default: return Node();
    }
}

bool ffw::JsonIterator::isEmpty() const {
    return type < 0;
}

bool ffw::JsonIterator::isString() const {
    if (type < 0) return false;
    return type == json_type_string;
}

bool ffw::JsonIterator::isNumber() const {
    return isInteger() || isFloat();
}

bool ffw::JsonIterator::isInteger() const {
    if (type < 0) return false;
    return type == json_type_int;
}

bool ffw::JsonIterator::isFloat() const {
    if (type < 0) return false;
    return type == json_type_double;
}

bool ffw::JsonIterator::isBoolean() const {
    if (type < 0) return false;
    return type == json_type_boolean;
}

bool ffw::JsonIterator::isNull() const {
    if (type < 0) return false;
    return type == json_type_null;
}

bool ffw::JsonIterator::isObject() const {
    if (type < 0) return false;
    return type == json_type_object;
}

bool ffw::JsonIterator::isArray() const {
    if (type < 0) return false;
    return type == json_type_array;
}

std::string ffw::JsonIterator::toString() const {
    if (elem && isString()) return std::string(json_object_get_string(elem));
    throw JsonReaderException("invalid node value");
}

ffw::Node::Integer ffw::JsonIterator::toInteger() const {
    if (elem && isInteger()) return json_object_get_int64(elem);
    throw JsonReaderException("invalid node value");
}

ffw::Node::Float ffw::JsonIterator::toFloat() const {
    if (elem && isFloat()) return json_object_get_double(elem);
    throw JsonReaderException("invalid node value");
}

ffw::Node::Boolean ffw::JsonIterator::toBoolean() const {
    if (elem && isBoolean()) return json_object_get_boolean(elem);
    throw JsonReaderException("invalid node value");
}

ffw::Node::Array ffw::JsonIterator::toArray(const bool attrIgnore) const {
    (void)attrIgnore;
    if (elem && isArray()) {
        Node::Array ret;
        const auto total = json_object_array_length(elem);
        ret.reserve(total);
        for (size_t i = 0; i < total; i++) {
            const auto child = json_object_array_get_idx(elem, i);
            ret.push_back(JsonIterator(child, "", child ? json_object_get_type(child) : json_type_null).get());
        }
        return ret;
    }
    throw JsonReaderException("invalid node value");
}

ffw::Node::Object ffw::JsonIterator::toObject(const bool attrIgnore) const {
    (void)attrIgnore;
    if (elem && isObject()) {
        Node::Object ret;
        size_t total = 0;
        json_object_object_foreach(elem, k, v) {
            total++;
        }
        ret.reserve(total);

        json_object_object_foreach(elem, key, val) {
            ret.insert(std::pair<std::string, Node>(key, JsonIterator(val, key, json_object_get_type(val)).get()));
        }

        return ret;
    }
    throw JsonReaderException("invalid node value");
}

void ffw::JsonReader::json_object_deleter::operator() (json_object* v) const {
    json_object_put(v);
}

struct ffw::JsonReader::Cache {
    json_object_iterator it, end;
    json_object* elem;
    size_t idx;
};

ffw::JsonReader::JsonReader(const std::string& src):JsonReader(src.c_str()) {

}

ffw::JsonReader::JsonReader(const char* src) :root(nullptr), now(nullptr) {
    json_tokener_error err;
    root.reset(json_tokener_parse_verbose(src, &err));
    if (!root) throw JsonReaderException(json_tokener_error_desc(err));
}

ffw::JsonReader::JsonReader(JsonReader&& other) NOEXCEPT :root(nullptr), now(nullptr) {
    swap(other);
}

ffw::JsonReader& ffw::JsonReader::operator = (JsonReader&& other) NOEXCEPT {
    if (this != &other) {
        swap(other);
    }
    return *this;
}

void ffw::JsonReader::swap(JsonReader& other) NOEXCEPT {
    std::swap(root, other.root);
    std::swap(cache, other.cache);
    std::swap(now, other.now);
}

ffw::JsonReader::~JsonReader() {
    close();
}

void ffw::JsonReader::close() {
    root.reset();
    cache.clear();
    now = nullptr;
}

bool ffw::JsonReader::isOpen() const {
    return root != nullptr;
}

bool ffw::JsonReader::getNext(JsonIterator* value) {
    if (cache.empty()) {
        cache.emplace_back();
        auto& child = cache.back();
        child.elem = root.get();

        const auto type = json_object_get_type(child.elem);
        if (type == json_type_object) {
            child.it = json_object_iter_begin(child.elem);
            child.end = json_object_iter_end(child.elem);

            if (value) *value = JsonIterator(child.elem, "", int(json_object_get_type(child.elem)));
            now = child.elem;
            return true;
        } else if (type == json_type_array) {
            child.idx = 0;
            if (value) *value = JsonIterator(child.elem, "", int(json_object_get_type(child.elem)));
            now = child.elem;
            return true;
        }
        return false;
    }

    if (cache.size() == 1) return false;

    auto& child = cache.back();
    const auto type = json_object_get_type(child.elem);

    if (type == json_type_object) {
        // empty object
        if (json_object_iter_equal(&child.it, &child.end)) return false;

        const auto key = json_object_iter_peek_name(&child.it);
        const auto elem = json_object_iter_peek_value(&child.it);
        if (!elem && !key) return false;

        json_object_iter_next(&child.it);
        if (value) *value = JsonIterator(elem, key, elem ? json_object_get_type(elem) : json_type_null);
        now = elem;
        return true;
    }

    if (type == json_type_array) {
        // out of bounds
        if (child.idx >= json_object_array_length(child.elem)) return false;

        const auto elem = json_object_array_get_idx(child.elem, child.idx);
        child.idx++;
        if (value) *value = JsonIterator(elem, "", elem ? json_object_get_type(elem) : json_type_null);
        now = elem;
        return true;
    }

    return false;
}

bool ffw::JsonReader::stepInto() {
    if (!now) return false;
    const auto type = json_object_get_type(now);
    if (type == json_type_object) {
        cache.emplace_back();
        auto& added = cache.back();

        added.elem = now;
        added.it = json_object_iter_begin(added.elem);
        added.end = json_object_iter_end(added.elem);
        now = nullptr;

        return true;
    }
    if (type == json_type_array) {
        cache.emplace_back();
        auto& added = cache.back();

        added.elem = now;
        added.idx = 0;

        return true;
    }

    return false;
}

bool ffw::JsonReader::stepOut() {
    if (cache.size() > 1) {
        cache.pop_back();
        return true;
    }
    return false;
}

ffw::Node ffw::decodeJsonFile(const std::string& path) {
    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
    return decodeJson(str);
}

ffw::Node ffw::decodeJson(const std::string& json) {
    ffw::JsonReader reader(json);
    ffw::JsonIterator node;

    reader.getNext(&node);
    return node.get();
}
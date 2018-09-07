#include <yaml-cpp/yaml.h>
#include <ffw/data/yamlwriter.h>

struct ffw::YamlWriter::Cache {
    bool isArray = false;
};

ffw::YamlWriter::YamlWriter() {
    doc.reset(new YAML::Emitter);
}

ffw::YamlWriter::YamlWriter(YamlWriter&& other) NOEXCEPT {
    swap(other);
}

ffw::YamlWriter& ffw::YamlWriter::operator = (YamlWriter&& other) NOEXCEPT {
    if (this != &other) {
        swap(other);
    }
    return *this;
}

void ffw::YamlWriter::swap(YamlWriter& other) NOEXCEPT {
    std::swap(doc, other.doc);
    std::swap(cache, other.cache);
}

ffw::YamlWriter::~YamlWriter() {
    close();
}

void ffw::YamlWriter::close() {
    doc.reset();
    cache.clear();
}

bool ffw::YamlWriter::isOpen() const {
    return false;
}

void ffw::YamlWriter::stepOut() {
    if (cache.empty()) throw YamlWriterException("can not step out any further");
    if (cache.back().isArray) {
        *doc << YAML::EndSeq;
    }
    else {
        *doc << YAML::EndMap;
    }
    cache.pop_back();
}

#define IS_EMPTY YamlWriterException("can not add value to empty yaml object");
#define IS_NOT_ARRAY YamlWriterException("can not add array element without a key into an object")
#define IS_ARRAY YamlWriterException("can not add key value pair element into an array")

void ffw::YamlWriter::add(const std::string& key, const Node& value) {
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

void ffw::YamlWriter::add(const std::string& key, const Node::Integer value) {
    if (cache.empty()) throw IS_EMPTY;
    if (cache.back().isArray) throw IS_ARRAY;
    *doc << YAML::Key << key;
    *doc << YAML::Value << value;
}

void ffw::YamlWriter::add(const std::string& key, const Node::Float value) {
    if (cache.empty()) throw IS_EMPTY;
    if (cache.back().isArray) throw IS_ARRAY;
    *doc << YAML::Key << key;
    *doc << YAML::Value << value;
}

void ffw::YamlWriter::add(const std::string& key, const Node::Boolean value) {
    if (cache.empty()) throw IS_EMPTY;
    if (cache.back().isArray) throw IS_ARRAY;
    *doc << YAML::Key << key;
    *doc << YAML::Value << value;
}

void ffw::YamlWriter::add(const std::string& key, const Node::String& value) {
    if (cache.empty()) throw IS_EMPTY;
    if (cache.back().isArray) throw IS_ARRAY;
    *doc << YAML::Key << key;
    *doc << YAML::Value << value;
}

void ffw::YamlWriter::add(const std::string& key, const Node::Null value) {
    (void)value;
    if (cache.empty()) throw IS_EMPTY;
    *doc << YAML::Key << key;
    *doc << YAML::Value << value;
}

void ffw::YamlWriter::addArray(const std::string& key) {
    if (!cache.empty() && cache.back().isArray) throw IS_ARRAY;

    *doc << YAML::Key << key;
    *doc << YAML::Value << YAML::BeginSeq;

    cache.emplace_back();
    auto& child = cache.back();
    child.isArray = true;
}

void ffw::YamlWriter::addObject(const std::string& key) {
    if (!cache.empty() && cache.back().isArray) throw IS_ARRAY;

    *doc << YAML::Key << key;
    *doc << YAML::Value << YAML::BeginMap;

    cache.emplace_back();
    auto& child = cache.back();
    child.isArray = false;
}

void ffw::YamlWriter::add(const Node& value) {
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

void ffw::YamlWriter::add(const Node::Integer value) {
    if (cache.empty()) throw IS_EMPTY;
    if (!cache.back().isArray) throw IS_NOT_ARRAY;
    *doc << value;
}

void ffw::YamlWriter::add(const Node::Float value) {
    if (cache.empty()) throw IS_EMPTY;
    if (!cache.back().isArray) throw IS_NOT_ARRAY;
    *doc << value;
}

void ffw::YamlWriter::add(const Node::Boolean value) {
    if (cache.empty()) throw IS_EMPTY;
    if (!cache.back().isArray) throw IS_NOT_ARRAY;
    *doc << value;
}

void ffw::YamlWriter::add(const Node::String& value) {
    if (cache.empty()) throw IS_EMPTY;
    if (!cache.back().isArray) throw IS_NOT_ARRAY;
    *doc << value;
}

void ffw::YamlWriter::add(const Node::Null value) {
    (void)value;
    if (cache.empty()) throw IS_EMPTY;
    if (!cache.back().isArray) throw IS_NOT_ARRAY;
    *doc << value;
}

void ffw::YamlWriter::addArray() {
    if (!cache.empty() && !cache.back().isArray) throw IS_NOT_ARRAY;

    *doc << YAML::BeginSeq;

    cache.emplace_back();
    auto& child = cache.back();
    child.isArray = true;

}

void ffw::YamlWriter::addObject() {
    if (!cache.empty() && !cache.back().isArray) throw IS_NOT_ARRAY;

    *doc << YAML::BeginMap;

    cache.emplace_back();
    auto& child = cache.back();
    child.isArray = false;
}

std::string ffw::YamlWriter::str() const {
    if (!doc) throw YamlWriterException("not initialized");
    return std::string(doc->c_str());
}

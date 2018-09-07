#include <regex>
#include <fstream>
#include <streambuf>
#include <yaml-cpp/yaml.h>
#include <ffw/data/yamlreader.h>

static bool isBoolean(const char* str) {
    if (!str || *str == '\0') return false;
    static const auto re = std::regex("^(true|false)$", std::regex::icase);
    return std::regex_match(str, re);
}

static bool isInteger(const char* str) {
    if (!str || *str == '\0') return false;
    static const auto re = std::regex("^[-+]?\\d+([Ee][+-]?\\d+)?$", std::regex::icase);
    return std::regex_match(str, re);
}

static bool isFloat(const char* str) {
    if (!str || *str == '\0') return false;
    static const auto re = std::regex("^[-+]?\\d*\\.{1}\\d+([Ee][+-]?\\d+)?$", std::regex::icase);
    return std::regex_match(str, re);
}

static bool isNumber(const char* str) {
    if (!str || *str == '\0') return false;
    static const auto re = std::regex("^[-+]?(?:0|[1-9]\\d*)(?:\\.\\d+)?(?:[eE][+-]?\\d+)?$", std::regex::icase);
    return std::regex_match(str, re);
}

struct ffw::YamlIterator::Ref {
    inline Ref(YAML::Node elem, YAML::Node key): elem(std::move(elem)), key(std::move(key)) {
        
    }

    YAML::Node elem;
    YAML::Node key;
};

ffw::YamlIterator::YamlIterator() : ref(nullptr) {

}

ffw::YamlIterator::YamlIterator(YAML::Node elem) : ref(new Ref(std::move(elem), YAML::Node())) {

}

ffw::YamlIterator::YamlIterator(YAML::Node elem, YAML::Node key): ref(new Ref(std::move(elem), std::move(key))) {

}

std::string ffw::YamlIterator::getKey() const {
    if (ref) return ref->key.IsScalar() ? ref->key.Scalar() : "";
    throw YamlReaderException("empty node");
}

ffw::Node ffw::YamlIterator::get(const bool attrIgnore) {
    if (isNull()) return Node(nullptr);
    if (isObject()) return Node(toObject(attrIgnore));
    if (isArray()) return Node(toArray(attrIgnore));
    if (isFloat()) return Node(toFloat());
    if (isInteger()) return Node(toInteger());
    if (isBoolean()) return Node(toBoolean());
    if (isString()) return Node(toString());
    return Node();
}

bool ffw::YamlIterator::isEmpty() const {
    return !ref || !ref->elem.IsDefined();
}

bool ffw::YamlIterator::isString() const {
    return ref && ref->elem.IsScalar();
}

bool ffw::YamlIterator::isNumber() const {
    return ref && ref->elem.IsScalar() && ::isNumber(ref->elem.Scalar().c_str());
}

bool ffw::YamlIterator::isInteger() const {
    return ref && ref->elem.IsScalar() && ::isInteger(ref->elem.Scalar().c_str());
}

bool ffw::YamlIterator::isFloat() const {
    return ref && ref->elem.IsScalar() && ::isFloat(ref->elem.Scalar().c_str());
}

bool ffw::YamlIterator::isBoolean() const {
    return ref && ref->elem.IsScalar() && ::isBoolean(ref->elem.Scalar().c_str());
}

bool ffw::YamlIterator::isNull() const {
    return ref && ref->elem.IsNull();
}

bool ffw::YamlIterator::isObject() const {
    return ref && ref->elem.IsMap();
}

bool ffw::YamlIterator::isArray() const {
    return ref && ref->elem.IsSequence();
}

std::string ffw::YamlIterator::toString() const {
    if (ref && ref->elem.IsScalar() && !ref->elem.IsNull()) {
        try {
            return ref->elem.as<std::string>();
        } catch (std::exception& e) {
            throw YamlReaderException(e.what());
        }
    }
    throw YamlReaderException("invalid node value");
}

ffw::Node::Integer ffw::YamlIterator::toInteger() const {
    if (isInteger()) {
        try {
            return ref->elem.as<Node::Integer>();
        }
        catch (std::exception& e) {
            throw YamlReaderException(e.what());
        }
    }
    throw YamlReaderException("invalid node value");
}

ffw::Node::Float ffw::YamlIterator::toFloat() const {
    if (isFloat()) {
        try {
            return ref->elem.as<Node::Float>();
        }
        catch (std::exception& e) {
            throw YamlReaderException(e.what());
        }
    }
    throw YamlReaderException("invalid node value");
}

ffw::Node::Boolean ffw::YamlIterator::toBoolean() const {
    if (ref && ref->elem.IsScalar()) {
        try {
            return ref->elem.as<Node::Boolean>();
        }
        catch (std::exception& e) {
            throw YamlReaderException(e.what());
        }
    }
    throw YamlReaderException("invalid node value");
}

ffw::Node::Array ffw::YamlIterator::toArray(const bool attrIgnore) const {
    (void)attrIgnore;
    if (ref && ref->elem.IsSequence()) {
        Node::Array arr;
        arr.reserve(ref->elem.size());
        for (size_t i = 0; i < ref->elem.size(); i++) {
            arr.push_back(YamlIterator(ref->elem[i]).get());
        }
        return arr;
    }
    throw YamlReaderException("invalid node value");
}

ffw::Node::Object ffw::YamlIterator::toObject(const bool attrIgnore) const {
    (void)attrIgnore;
    if (ref && ref->elem.IsMap()) {
        Node::Object obj;
        obj.reserve(ref->elem.size());
        for (auto it = ref->elem.begin(); it != ref->elem.end(); ++it) {
            obj.insert(std::pair<std::string, Node>(it->first.Scalar(), YamlIterator(it->second).get()));
        }
        return obj;
    }
    throw YamlReaderException("invalid node value");
}

struct ffw::YamlReader::Cache {
    YAML::const_iterator it;
    YAML::const_iterator end;
    YAML::Node elem;
    size_t idx = 0;
    bool first = true;
};

ffw::YamlReader::YamlReader(const std::string& src) :YamlReader(src.c_str()) {

}

ffw::YamlReader::YamlReader(const char* src) {
    doc.reset(new YAML::Node());

    try {
        *doc = YAML::Load(src);

        if (doc->IsMap()) {
            cache.push_back({
                doc->begin(),
                doc->end(),
                *doc
            });
        } else if (doc->IsSequence()) {
            cache.push_back({
                YAML::const_iterator(),
                YAML::const_iterator(),
                *doc
            });
        } else {
            throw YamlReaderException("the root must start with a map or a sequence");
        }
    } catch (std::exception& e) {
        throw YamlReaderException(e.what());
    }
}

ffw::YamlReader::YamlReader(YamlReader&& other) NOEXCEPT : doc(nullptr) {
    swap(other);
}

ffw::YamlReader& ffw::YamlReader::operator = (YamlReader&& other) NOEXCEPT {
    if (this != &other) {
        swap(other);
    }
    return *this;
}

void ffw::YamlReader::swap(YamlReader& other) NOEXCEPT {
    std::swap(doc, other.doc);
    std::swap(cache, other.cache);
}

ffw::YamlReader::~YamlReader() {
    close();
}

void ffw::YamlReader::close() {
    doc.reset();
}

bool ffw::YamlReader::isOpen() const {
    return doc && !doc->IsNull();
}

bool ffw::YamlReader::getNext(YamlIterator* value) {
    if (cache.empty()) return false;
    auto& child = cache.back();
    if (child.elem.IsMap()) {
        if (child.it == child.end) return false;

        if (!child.first) {
            ++child.it;

            if (child.it == child.end) return false;
        }
        child.first = false;
        if (value) *value = YamlIterator(child.it->second, child.it->first);

        return true;
    }
    else if (child.elem.IsSequence()) {
        if (child.idx + 1 >= child.elem.size()) return false;

        if (!child.first) {
            ++child.idx;
        }
        child.first = false;
        if (value) *value = YamlIterator(child.elem[child.idx]);

        return true;
    }
    return false;
}

bool ffw::YamlReader::stepInto() {
    if (!cache.empty()) {
        auto& child = cache.back();
        if (child.elem.IsSequence()) {
            if (child.idx >= child.elem.size()) return false;

            auto now = child.elem[child.idx];

            if (now.IsMap()) {
                cache.push_back({
                    now.begin(),
                    now.end(),
                    now
                });
                return true;
            } 
            
            if (now.IsSequence()) {
                cache.push_back({
                    YAML::const_iterator(),
                    YAML::const_iterator(),
                    now
                });
                return true;
            } 
            
            return false;
        } else if (child.elem.IsMap()) {
            if (child.it == child.end) return false;

            auto now = child.it->second;
            
            if (now.IsMap()) {
                cache.push_back({
                    now.begin(),
                    now.end(),
                    now
                });
                return true;
            }
            
            if (now.IsSequence()) {
                cache.push_back({
                    YAML::const_iterator(),
                    YAML::const_iterator(),
                    now
                });
                return true;
            }
            
            return false;
        } 
        
        return false;
    }
    return false;
}

bool ffw::YamlReader::stepOut() {
    if (cache.size() > 1) {
        cache.pop_back();
        return true;
    }
    return false;
}

ffw::Node ffw::decodeYamlFile(const std::string& path) {
    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
    return decodeYaml(str);
}

ffw::Node ffw::decodeYaml(const std::string& json) {
    ffw::YamlReader reader(json);
    ffw::YamlIterator node;

    reader.getNext(&node);
    return node.get();
}

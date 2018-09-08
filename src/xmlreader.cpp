#include <tinyxml2.h>
#include <fstream>
#include <streambuf>
#include <regex>
#include <ffw/data/xmlreader.h>
#include <ffw/data/datareader.h>
#include "xmlhelpers.h"

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

static bool isString(const char* str) {
    if (!str || *str == '\0') return false;
    return *str != '\0';
}

static bool isNull(const char* str) {
    if (!str || *str == '\0') return false;
    static const auto re = std::regex("^null$", std::regex::icase);
    return std::regex_match(str, re);
}

static bool toBoolean(const char* str) {
    if (!str || *str == '\0')
        throw ffw::XmlReaderException("value is empty");
    static const auto reTrue = std::regex("^true$", std::regex::icase);
    static const auto reFalse = std::regex("^false$", std::regex::icase);
    if (std::regex_match(str, reTrue)) return true;
    if (std::regex_match(str, reFalse)) return false;
    throw ffw::XmlReaderException("can not convert to boolean, error: not valid boolean value");
}

static long long toInteger(const char* str) {
    if (!str || *str == '\0')
        throw ffw::XmlReaderException("value is empty");
    try {
        return std::stoll(str);
    }
    catch (std::exception& e) {
        throw ffw::XmlReaderException("can not convert to integer, error: " + std::string(e.what()));
    }
}

static double toFloat(const char* str) {
    if (!str || *str == '\0')
        throw ffw::XmlReaderException("value is empty");
    try {
        return std::stod(str);
    }
    catch (std::exception& e) {
        throw ffw::XmlReaderException("can not convert to floating point, error: " + std::string(e.what()));
    }
}

static std::string toString(const char* str) {
    if (!str || *str == '\0')
        throw ffw::XmlReaderException("value is empty");
    return std::string(str);
}


ffw::XmlIterator::XmlIterator():elem(nullptr),attr(nullptr) {
    
}

ffw::XmlIterator::XmlIterator(const tinyxml2::XMLElement* elem, const tinyxml2::XMLAttribute* attr):elem(elem), attr(attr) {
            
}

bool ffw::XmlIterator::getNextAttribute(XmlIterator* node) {
    if (elem != nullptr) {
        if (attr == nullptr) {
            attr = elem->FirstAttribute();
            if (attr != nullptr) {
                if (node != nullptr)*node = XmlIterator(nullptr, attr);
                return true;
            }
            return false;
        }
        else {
            attr = attr->Next();
            if (attr != nullptr) {
                if (node != nullptr)*node = XmlIterator(nullptr, attr);
                return true;
            }
            return false;
        }
    }
    return false;
}

std::string ffw::XmlIterator::getKey() const {
    if (!elem && attr && attr->Name())return std::string(attr->Name());
    if (elem && elem->Name())return std::string(elem->Name());
    throw XmlReaderException("empty node");
}

ffw::Node ffw::XmlIterator::get(const bool attrIgnore) {
    if (isNull()) return Node(nullptr);
    if (isObject()) return Node(toObject(attrIgnore));
    if (isArray()) return Node(toArray(attrIgnore));
    if (isFloat()) return Node(toFloat());
    if (isInteger()) return Node(toInteger());
    if (isBoolean()) return Node(toBoolean());
    if (isString()) return Node(toString());
    return Node();
}

bool ffw::XmlIterator::isEmpty() const {
    if (elem) {
        if (isObject()) return false;
        if (rawStr() == nullptr || *rawStr() == '\0') return true;
        return !hasAttributes();
    } else if (!elem && attr) {
        return (attr->Value() == nullptr || *attr->Value() == '\0');
    } else {
        return true;
    }
}

bool ffw::XmlIterator::isString() const {
    if (!elem && attr) return ::isString(attr->Value());
    return elem && elem->FirstChildElement() == nullptr && ::isString(rawStr());
}

bool ffw::XmlIterator::isNumber() const {
    if (!elem && attr) return ::isNumber(attr->Value());
    return elem && ::isNumber(rawStr());
}

bool ffw::XmlIterator::isInteger() const {
    if (!elem && attr) return ::isInteger(attr->Value());
    return elem && ::isInteger(rawStr());
}

bool ffw::XmlIterator::isFloat() const {
    if (!elem && attr) return ::isFloat(attr->Value());
    return elem && ::isFloat(rawStr());
}

bool ffw::XmlIterator::isBoolean() const {
    if (!elem && attr) return ::isBoolean(attr->Value());
    return elem && ::isBoolean(rawStr());
}

bool ffw::XmlIterator::isNull() const {
    if (!elem && attr) return ::isNull(attr->Value());
    return elem && ::isNull(rawStr());
}

bool ffw::XmlIterator::isObject() const {
    if (!elem && attr) return false;
    return elem && elem->FirstChildElement() != nullptr;
}

bool ffw::XmlIterator::hasAttributes() const {
    if (elem) return elem->FirstAttribute() != nullptr;
    return false;
}

const char* ffw::XmlIterator::rawStr() const {
    if (!elem && attr) return attr->Value();
    else if (elem) return elem->GetText();
    return nullptr;
}

bool ffw::XmlIterator::isArray() const {
    if (elem) {
        const auto first = elem->NextSiblingElement();
        if (!first) return false;
        return strcmp(elem->Name(), first->Name()) == 0;
    }
    return false;
}

ffw::Node::String ffw::XmlIterator::toString() const {
    if (!elem && attr)return ::toString(attr->Value());
    if (elem)return ::toString(elem->GetText());
    throw XmlReaderException("invalid node value");
}

ffw::Node::Integer ffw::XmlIterator::toInteger() const {
    if (!elem && attr)return ::toInteger(attr->Value());
    if (elem)return ::toInteger(elem->GetText());
    throw XmlReaderException("invalid node value");
}

ffw::Node::Float ffw::XmlIterator::toFloat() const {
    if (!elem && attr)return ::toFloat(attr->Value());
    if (elem)return ::toFloat(elem->GetText());
    throw XmlReaderException("invalid node value");
}

ffw::Node::Boolean ffw::XmlIterator::toBoolean() const {
    if (!elem && attr)return ::toBoolean(attr->Value());
    if (elem)return ::toBoolean(elem->GetText());
    throw XmlReaderException("invalid node value");
}

static ffw::Node toNodeNoArray(const tinyxml2::XMLElement* child, const bool attrIgnore) {
    ffw::XmlIterator node(child);
    
    if (!attrIgnore && child->FirstAttribute() != nullptr) {
        ffw::Node::Object temp;
        const tinyxml2::XMLAttribute* attr = child->FirstAttribute();

        if (attr) temp.insert(std::pair<std::string, ffw::Node>(std::string(attr->Name()), ffw::XmlIterator(nullptr, attr).get(attrIgnore)));
        while ((attr = attr->Next()) != nullptr) {
            temp.insert(std::pair<std::string, ffw::Node>(std::string(attr->Name()), ffw::XmlIterator(nullptr, attr).get(attrIgnore)));
        }
            
        if (node.isEmpty()) return temp;
        else if (node.isNull()) temp.insert(std::pair<std::string, ffw::Node>(std::string("content"), ffw::Node(nullptr)));
        else if (node.isObject()) temp.insert(std::pair<std::string, ffw::Node>(std::string("content"), ffw::Node(node.toObject(attrIgnore))));
        else if (node.isFloat()) temp.insert(std::pair<std::string, ffw::Node>(std::string("content"), ffw::Node(node.toFloat())));
        else if (node.isInteger()) temp.insert(std::pair<std::string, ffw::Node>(std::string("content"), ffw::Node(node.toInteger())));
        else if (node.isBoolean()) temp.insert(std::pair<std::string, ffw::Node>(std::string("content"), ffw::Node(node.toBoolean())));
        else temp.insert(std::pair<std::string, ffw::Node>(std::string("content"), ffw::Node(node.toString())));

        return temp;
    }
    else {

        if (node.isEmpty()) return ffw::Node();
        if (node.isNull()) return ffw::Node(nullptr);
        if (node.isObject()) return ffw::Node(node.toObject(attrIgnore));
        if (node.isFloat()) return ffw::Node(node.toFloat());
        if (node.isInteger()) return ffw::Node(node.toInteger());
        if (node.isBoolean()) return ffw::Node(node.toBoolean());
        return ffw::Node(node.toString());
    }
}

ffw::Node::Array ffw::XmlIterator::toArray(const bool attrIgnore) const {
    if (elem) {
        Node::Array arr = { toNodeNoArray(elem, attrIgnore)};
        const auto* next = elem->NextSiblingElement();
        do {
            if (strcmp(next->Name(), elem->Name()) != 0) break;
            arr.push_back(toNodeNoArray(next, attrIgnore));
        } while ((next = next->NextSiblingElement()) != nullptr);
        return arr;
    }
    throw XmlReaderException("invalid node value");
}

void ffw::XmlIterator::getPair(Node::Object& obj, const tinyxml2::XMLElement* child, const bool attrIgnore) {
    if (!attrIgnore && child->FirstAttribute() != nullptr) {

        ffw::Node::Object temp;
        const tinyxml2::XMLAttribute* attr = child->FirstAttribute();
            
        if (attr) temp.insert(std::pair<std::string, Node>(std::string(attr->Name()), XmlIterator(nullptr, attr).get(attrIgnore)));
        while ((attr = attr->Next()) != nullptr) {
            temp.insert(std::pair<std::string, Node>(std::string(attr->Name()), XmlIterator(nullptr, attr).get(attrIgnore)));
        }
            
        auto content = XmlIterator(child);
        if (!content.isEmpty()) {
            if (content.isObject()) {
                auto childChild = child->FirstChildElement();
                getPair(temp, childChild, attrIgnore);
                while ((childChild = childChild->NextSiblingElement()) != nullptr) {
                    getPair(temp, childChild, attrIgnore);
                }
            }
            else {
                temp.insert(std::pair<std::string, Node>(std::string("content"), content.get(attrIgnore)));
            }
        }

        obj.insert(std::pair<std::string, Node>(std::string(child->Name()), Node(temp)));

        return;
    }
    
    obj.insert(std::pair<std::string, Node>(std::string(child->Name()), XmlIterator(child).get(attrIgnore)));
}

ffw::Node::Object ffw::XmlIterator::toObject(const bool attrIgnore) const {
    if (elem) {
        Node::Object obj;

        if (!attrIgnore && elem->FirstAttribute() != nullptr) {
            ffw::Node::Object content;
            const tinyxml2::XMLAttribute* attr = elem->FirstAttribute();

            if (attr) obj.insert(std::pair<std::string, Node>(std::string(attr->Name()), XmlIterator(nullptr, attr).get(attrIgnore)));
            while ((attr = attr->Next()) != nullptr) {
                obj.insert(std::pair<std::string, Node>(std::string(attr->Name()), XmlIterator(nullptr, attr).get(attrIgnore)));
            }

            auto child = elem->FirstChildElement();
            getPair(obj, child, attrIgnore);

            while ((child = child->NextSiblingElement()) != nullptr) {
                getPair(obj, child, attrIgnore);
            }

        }
        else {
            auto child = elem->FirstChildElement();
            getPair(obj, child, attrIgnore);

            while ((child = child->NextSiblingElement()) != nullptr) {
                getPair(obj, child, attrIgnore);
            }
        }

        return obj;
    }
    throw XmlReaderException("invalid node value");
}

ffw::XmlReader::XmlReader(const std::string& src):XmlReader(src.c_str()) {
}

ffw::XmlReader::XmlReader(const char* src):document(new tinyxml2::XMLDocument), child(nullptr),parent(nullptr) {
    if (!src) throw XmlReaderException("invalid input string");
    const auto err = document->Parse(src);
    if (err != tinyxml2::XMLError::XML_SUCCESS) {
        throw XmlReaderException("failed to parse document, error: " + std::string(getErrorStr(err)) + " at: " + document->ErrorStr());
    }
}

ffw::XmlReader::XmlReader(XmlReader&& other) NOEXCEPT :document(nullptr),child(nullptr),parent(nullptr) {
    swap(other);
}

ffw::XmlReader& ffw::XmlReader::operator = (XmlReader&& other) NOEXCEPT {
    if (this != &other) {
        swap(other);
    }
    return *this;
}

void ffw::XmlReader::swap(XmlReader& other) NOEXCEPT {
    std::swap(cache, other.cache);
    std::swap(document, other.document);
    std::swap(child, other.child);
    std::swap(parent, other.parent);
}

ffw::XmlReader::~XmlReader() {
    close();
}

void ffw::XmlReader::close() {
    document.reset();
}

bool ffw::XmlReader::isOpen() const {
    return (bool)document;
}

std::string ffw::XmlReader::getRootKey() const {
    if (!document || !document->RootElement()) return "";
    return std::string(document->RootElement()->Name());
}

bool ffw::XmlReader::getNext(XmlIterator* value) {
    // Is root? First run?
    if (parent == nullptr) {
        parent = child = document->RootElement();

        // No root element, document is empty
        if (parent == nullptr) {
            return false;
        }

        if (value != nullptr)*value = XmlIterator(child);

        return true;
    }
    else {
        if (child == nullptr) {
            child = parent->FirstChildElement();

            if (child == nullptr) {
                throw XmlReaderException("could not parse next element");
            }
        }
        else {
            const auto test = child->NextSiblingElement();
            if (test != nullptr) {
                child = test;
            }
            else {
                // No next element
                return false;
            }
        }

        if (value != nullptr)*value = XmlIterator(child);
        return true;
    }
}

bool ffw::XmlReader::stepInto() {
    if (child != nullptr) {
        if (child->FirstChildElement()) {
            cache.push_back(Cache(child, parent));

            parent = child->ToElement();
            child = nullptr;

            return true;
        }
        else {
            throw XmlReaderException("cannot step into a node, expected array or object");
        }
    }
    return false;
}

bool ffw::XmlReader::stepOut() {
    if (!cache.empty()) {
        parent = cache.back().parent;
        child = cache.back().child;
        cache.pop_back();
        return true;
    }
    return false;
}

ffw::Node ffw::decodeXmlFile(const std::string& path) {
    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)),
        std::istreambuf_iterator<char>());
    return decodeXml(str);
}

ffw::Node ffw::decodeXml(const std::string& json) {
    ffw::XmlReader reader(json);
    ffw::XmlIterator node;

    reader.getNext(&node);
    ffw::Node::Object obj;
    obj.insert(std::make_pair(node.getKey(), node.get()));
    return obj;
}


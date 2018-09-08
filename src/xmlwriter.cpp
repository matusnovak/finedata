#include <fstream>
#include <tinyxml2.h>
#include <ffw/data/xmlwriter.h>

static const auto NO_ATTRIBUTES = ffw::Node::Object();

ffw::XmlWriter::XmlWriter(const bool header) : printer(new tinyxml2::XMLPrinter) {
    if (header) {
        printer->PushHeader(false, true);
    }
}

ffw::XmlWriter::XmlWriter(XmlWriter&& other) NOEXCEPT : printer(nullptr) {
    swap(other);
}

ffw::XmlWriter& ffw::XmlWriter::operator = (XmlWriter&& other) NOEXCEPT {
    if (this != &other) {
        swap(other);
    }
    return *this;
}

void ffw::XmlWriter::swap(XmlWriter& other) NOEXCEPT {
    std::swap(printer, other.printer);
    std::swap(keys, other.keys);
    std::swap(isArray, other.isArray);
}

ffw::XmlWriter::~XmlWriter() {
    close();
}

void ffw::XmlWriter::close() {
    printer.reset();
}

bool ffw::XmlWriter::isOpen() const {
    return (bool)printer;
}

void ffw::XmlWriter::stepOut() {
    if (keys.empty()) throw XmlWriterException("can not step out any further");

    if (isArray.back()) {
        isArray.pop_back();
        keys.pop_back();
        return;
    }

    printer->CloseElement();
    keys.pop_back();
    isArray.pop_back();
}

#define IS_NOT_ARRAY XmlWriterException("can not add array element without a key into an object")
#define IS_ARRAY XmlWriterException("can not add key value pair element into an array")

void ffw::XmlWriter::add(const std::string& key, const Node& value) {
    add(key, value, NO_ATTRIBUTES);
}

void ffw::XmlWriter::add(const std::string& key, const Node::Integer value) {
    add(key, value, NO_ATTRIBUTES);
}

void ffw::XmlWriter::add(const std::string& key, const Node::Float value) {
    add(key, value, NO_ATTRIBUTES);
}

void ffw::XmlWriter::add(const std::string& key, const Node::Boolean value) {
    add(key, value, NO_ATTRIBUTES);
}

void ffw::XmlWriter::add(const std::string& key, const Node::String& value) {
    add(key, value, NO_ATTRIBUTES);
}

void ffw::XmlWriter::add(const std::string& key, const Node::Null value) {
    add(key, value, NO_ATTRIBUTES);
}

void ffw::XmlWriter::addArray(const std::string& key) {
    addArray(key, NO_ATTRIBUTES);
}

void ffw::XmlWriter::addObject(const std::string& key) {
    addObject(key, NO_ATTRIBUTES);
}

void ffw::XmlWriter::add(const Node& value) {
    add(value, NO_ATTRIBUTES);
}

void ffw::XmlWriter::add(const Node::Integer value) {
    add(value, NO_ATTRIBUTES);
}

void ffw::XmlWriter::add(const Node::Float value) {
    add(value, NO_ATTRIBUTES);
}

void ffw::XmlWriter::add(const Node::Boolean value) {
    add(value, NO_ATTRIBUTES);
}

void ffw::XmlWriter::add(const Node::String& value) {
    add(value, NO_ATTRIBUTES);
}

void ffw::XmlWriter::add(const Node::Null value) {
    add(value, NO_ATTRIBUTES);
}

void ffw::XmlWriter::addArray() {
    addArray(NO_ATTRIBUTES);
}

void ffw::XmlWriter::addObject() {
    addObject(NO_ATTRIBUTES);
}

void ffw::XmlWriter::add(const std::string& key, const Node& value, const Node::Object& attributes) {
    switch (value.getType()) {
    case Node::Type::FLOAT: add(key, value.getAsFloat()); break;
    case Node::Type::INTEGER: add(key, value.getAsInt()); break;
    case Node::Type::BOOLEAN: add(key, value.getAsBool()); break;
    case Node::Type::NULLVAL: add(key, nullptr); break;
    case Node::Type::STRING: add(key, value.getAsString()); break;
    case Node::Type::ARRAY: {
        addArray(key);
        pushAttributes(attributes);
        for (const auto& e : value.getAsArray()) {
            add(e);
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

void ffw::XmlWriter::add(const std::string& key, const Node::Integer value, const Node::Object& attributes) {
    if (isArray.back()) throw IS_ARRAY;
    printer->OpenElement(key.c_str());
    pushAttributes(attributes);
    printer->PushText(std::to_string(value).c_str());
    printer->CloseElement();
}

void ffw::XmlWriter::add(const std::string& key, const Node::Float value, const Node::Object& attributes) {
    if (isArray.back()) throw IS_ARRAY;
    printer->OpenElement(key.c_str());
    pushAttributes(attributes);
    printer->PushText(std::to_string(value).c_str());
    printer->CloseElement();
}

void ffw::XmlWriter::add(const std::string& key, const Node::Boolean value, const Node::Object& attributes) {
    if (isArray.back()) throw IS_ARRAY;
    printer->OpenElement(key.c_str());
    pushAttributes(attributes);
    printer->PushText(value ? "true" : "false");
    printer->CloseElement();
}

void ffw::XmlWriter::add(const std::string& key, const Node::String& value, const Node::Object& attributes) {
    if (isArray.back()) throw IS_ARRAY;
    printer->OpenElement(key.c_str());
    pushAttributes(attributes);
    printer->PushText(value.c_str());
    printer->CloseElement();
}

void ffw::XmlWriter::add(const std::string& key, const Node::Null value, const Node::Object& attributes) {
    if (isArray.back()) throw IS_ARRAY;
    (void)value;
    printer->OpenElement(key.c_str());
    pushAttributes(attributes);
    printer->PushText("null");
    printer->CloseElement();
}

void ffw::XmlWriter::addArray(const std::string& key, const Node::Object& attributes) {
    if (!isArray.empty() && isArray.back()) throw IS_ARRAY;
    (void)attributes;
    keys.push_back(key);
    isArray.push_back(true);
}

void ffw::XmlWriter::addObject(const std::string& key, const Node::Object& attributes) {
    if (!isArray.empty() && isArray.back()) throw IS_ARRAY;
    keys.push_back(key);
    isArray.push_back(false);
    printer->OpenElement(keys.back().c_str());
    pushAttributes(attributes);
}

void ffw::XmlWriter::add(const Node& value, const Node::Object& attributes) {
    switch (value.getType()) {
        case Node::Type::FLOAT: add(value.getAsFloat()); break;
        case Node::Type::INTEGER: add(value.getAsInt()); break;
        case Node::Type::BOOLEAN: add(value.getAsBool()); break;
        case Node::Type::NULLVAL: add(nullptr); break;
        case Node::Type::STRING: add(value.getAsString()); break;
        case Node::Type::ARRAY: {
            if (keys.empty()) throw XmlWriterException("can not create xml root from an array");
            addArray();
            pushAttributes(attributes);
            for (const auto& e : value.getAsArray()) {
                add(e);
            }
            stepOut();
            break;
        }
        case Node::Type::OBJECT: {
            if (keys.empty()) throw XmlWriterException("can not create xml root without a key, use add(std::string, Node) instead");
            addObject();
            pushAttributes(attributes);
            for (const auto& pair : value.getAsObject()) {
                add(pair.first, pair.second);
            }
            stepOut();
            break;
        }
        default: break;
    }
}

void ffw::XmlWriter::add(const Node::Integer value, const Node::Object& attributes) {
    if (!isArray.back()) throw IS_NOT_ARRAY;
    printer->OpenElement(keys.back().c_str());
    pushAttributes(attributes);
    printer->PushText(std::to_string(value).c_str());
    printer->CloseElement();
}

void ffw::XmlWriter::add(const Node::Float value, const Node::Object& attributes) {
    if (!isArray.back()) throw IS_NOT_ARRAY;
    printer->OpenElement(keys.back().c_str());
    pushAttributes(attributes);
    printer->PushText(std::to_string(value).c_str());
    printer->CloseElement();
}

void ffw::XmlWriter::add(const Node::Boolean value, const Node::Object& attributes) {
    if (!isArray.back()) throw IS_NOT_ARRAY;
    printer->OpenElement(keys.back().c_str());
    pushAttributes(attributes);
    printer->PushText(value ? "true" : "false");
    printer->CloseElement();
}

void ffw::XmlWriter::add(const Node::String& value, const Node::Object& attributes) {
    if (!isArray.back()) throw IS_NOT_ARRAY;
    printer->OpenElement(keys.back().c_str());
    pushAttributes(attributes);
    printer->PushText(value.c_str());
    printer->CloseElement();
}

void ffw::XmlWriter::add(const Node::Null value, const Node::Object& attributes) {
    (void)value;
    if (!isArray.back()) throw IS_NOT_ARRAY;
    printer->OpenElement(keys.back().c_str());
    pushAttributes(attributes);
    printer->PushText("null");
    printer->CloseElement();
}

void ffw::XmlWriter::addArray(const Node::Object& attributes) {
    (void)attributes;
    if (!isArray.back()) throw IS_NOT_ARRAY;
    keys.push_back(keys.back());
    isArray.push_back(true);
}

void ffw::XmlWriter::addObject(const Node::Object& attributes) {
    if (!isArray.back()) throw IS_NOT_ARRAY;
    keys.push_back(keys.back());
    isArray.push_back(false);
    printer->OpenElement(keys.back().c_str());
    pushAttributes(attributes);
}

std::string ffw::XmlWriter::str() const {
    if (!printer) throw XmlWriterException("not initialized");
    return std::string(printer->CStr(), printer->CStrSize());
}

void ffw::XmlWriter::pushAttributes(const Node::Object& attributes) const {
    for (const auto& attr : attributes) {
        switch (attr.second.getType()) {
            case Node::Type::FLOAT: printer->PushAttribute(attr.first.c_str(), attr.second.getAsFloat()); break;
            case Node::Type::INTEGER: printer->PushAttribute(attr.first.c_str(), static_cast<unsigned>(attr.second.getAsInt())); break;
            case Node::Type::STRING: printer->PushAttribute(attr.first.c_str(), attr.second.getAsString().c_str()); break;
            case Node::Type::BOOLEAN: printer->PushAttribute(attr.first.c_str(), attr.second.getAsBool() ? "true" : "false"); break;
            case Node::Type::NULLVAL: printer->PushAttribute(attr.first.c_str(), "null"); break;
            default: break;
        }
    }
}

void ffw::encodeXmlFile(const ffw::Node& xml, const std::string& path) {
    const auto str = encodeXml(xml);
    std::fstream output(path, std::ios::out);
    if (!output) throw XmlWriterException("failed to open file");
    output << str;
}

std::string ffw::encodeXml(const ffw::Node& xml) {
    if (!xml.isObject()) throw XmlWriterException("root must be an object");
    const auto& obj = xml.getAsObject();
    ffw::XmlWriter writer;
    writer.add(obj.begin()->first, obj.begin()->second);
    return writer.str();
}

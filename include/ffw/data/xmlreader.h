/* This file is part of FineFramework project */
#ifndef FFW_DATA_XML_READER
#define FFW_DATA_XML_READER

#include "datareader.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace tinyxml2 {
    class XMLElement;
    class XMLDocument;
    class XMLNode;
    class XMLAttribute;
}
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace ffw {
    class XmlReaderException : public DataReaderException {
    public:
        XmlReaderException(const std::string& msg) :DataReaderException(msg) {

        }
    };

    class FFW_API XmlIterator : public DataIterator {
    public:
        XmlIterator();
        XmlIterator(const tinyxml2::XMLElement* elem, const tinyxml2::XMLAttribute* attr = nullptr);
        XmlIterator(const XmlIterator& other) = default;
        XmlIterator& operator = (const XmlIterator& other) = default;
        virtual ~XmlIterator() = default;
        bool getNextAttribute(XmlIterator* node);
        bool hasAttributes() const;
        std::string getKey() const override;
        Node get(bool attrIgnore = false) override;

        bool isEmpty() const override;
        bool isString() const override;
        bool isNumber() const override;
        bool isInteger() const override;
        bool isFloat() const override;
        bool isBoolean() const override;
        bool isNull() const override;
        bool isObject() const override;
        bool isArray() const override;

        Node::String toString() const override;
        Node::Integer toInteger() const override;
        Node::Float toFloat() const override;
        Node::Boolean toBoolean() const override;
        Node::Array toArray(bool attrIgnore = false) const override;
        Node::Object toObject(bool attrIgnore = false) const override;

        operator bool() const override {
            return elem != nullptr;
        }

        const char* rawStr() const;

        friend class XmlReader;
    private:
        static void getPair(Node::Object& obj, const tinyxml2::XMLElement* child, bool attrIgnore);
        const tinyxml2::XMLElement* elem;
        const tinyxml2::XMLAttribute* attr;
    };

    class FFW_API XmlReader: public DataReader {
    public:
        XmlReader(const std::string& src);
        XmlReader(const char* src);
        XmlReader(XmlReader& other) = delete;
        XmlReader(XmlReader&& other) NOEXCEPT;
        XmlReader& operator = (const XmlReader& other) = delete;
        XmlReader& operator = (XmlReader&& other) NOEXCEPT;
        void swap(XmlReader& other) NOEXCEPT;
        virtual ~XmlReader();

        void close() override;
        bool isOpen() const override;

        bool getNext(XmlIterator* value);
        bool stepInto() override;
        bool stepOut() override;

        std::string getRootKey() const;
    private:
        struct Cache {
            const tinyxml2::XMLElement* child = nullptr;
            const tinyxml2::XMLElement* parent = nullptr;
        };
        std::list<Cache> cache;
        std::unique_ptr<tinyxml2::XMLDocument> document;
        const tinyxml2::XMLElement* child;
        const tinyxml2::XMLElement* parent;
    };

    FFW_API Node decodeXmlFile(const std::string& path);
    FFW_API Node decodeXml(const std::string& json);
}

inline void swap(ffw::XmlReader& first, ffw::XmlReader& second) NOEXCEPT {
    first.swap(second);
}

#endif
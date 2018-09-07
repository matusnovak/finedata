/* This file is part of FineFramework project */
#ifndef FFW_DATA_XML_WRITER
#define FFW_DATA_XML_WRITER

#include <list>
#include "datawriter.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace tinyxml2 {
    class XMLPrinter;
}
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace ffw {
    class XmlWriterException : public DataWriterException {
    public:
        XmlWriterException(const std::string& msg) :DataWriterException(msg) {

        }
    };

    class FFW_API XmlWriter : public DataWriter {
    public:
        XmlWriter(bool header = false);
        XmlWriter(XmlWriter& other) = delete;
        XmlWriter(XmlWriter&& other) NOEXCEPT;
        XmlWriter& operator = (const XmlWriter& other) = delete;
        XmlWriter& operator = (XmlWriter&& other) NOEXCEPT;
        void swap(XmlWriter& other) NOEXCEPT;
        virtual ~XmlWriter();

        void close() override;
        Node::Boolean isOpen() const override;
        void stepOut() override;

        void add(const Key& key, const Node& value) override;
        void add(const Key& key, Node::Integer value) override;
        void add(const Key& key, Node::Float value) override;
        void add(const Key& key, Node::Boolean value) override;
        void add(const Key& key, const Node::String& value) override;
        void add(const Key& key, std::nullptr_t value) override;
        void addArray(const Key& key) override;
        void addObject(const Key& key) override;

        void add(const Node& value) override;
        void add(Node::Integer value) override;
        void add(Node::Float value) override;
        void add(Node::Boolean value) override;
        void add(const Node::String& value) override;
        void add(Node::Null value) override;
        void addArray() override;
        void addObject() override;

        void add(const Key& key, const Node& value, const Node::Object& attributes) override;
        void add(const Key& key, Node::Integer value, const Node::Object& attributes) override;
        void add(const Key& key, Node::Float value, const Node::Object& attributes) override;
        void add(const Key& key, Node::Boolean value, const Node::Object& attributes) override;
        void add(const Key& key, const Node::String& value, const Node::Object& attributes) override;
        void add(const Key& key, Node::Null value, const Node::Object& attributes) override;
        void addArray(const Key& key, const Node::Object& attributes) override;
        void addObject(const Key& key, const Node::Object& attributes) override;

        void add(const Node& value, const Node::Object& attributes) override;
        void add(Node::Integer value, const Node::Object& attributes) override;
        void add(Node::Float value, const Node::Object& attributes) override;
        void add(Node::Boolean value, const Node::Object& attributes) override;
        void add(const Node::String& value, const Node::Object& attributes) override;
        void add(Node::Null value, const Node::Object& attributes) override;
        void addArray(const Node::Object& attributes) override;
        void addObject(const Node::Object& attributes) override;

        std::string str() const override;
    private:
        void pushAttributes(const Node::Object& attributes) const;

        std::list<std::string> keys;
        std::list<Node::Boolean> isArray;
        std::unique_ptr<tinyxml2::XMLPrinter> printer;
    };
}

inline void swap(ffw::XmlWriter& first, ffw::XmlWriter& second) NOEXCEPT {
    first.swap(second);
}

#endif
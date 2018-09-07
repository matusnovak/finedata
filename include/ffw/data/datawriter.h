/* This file is part of FineFramework project */
#ifndef FFW_DATA_NODE_WRITER
#define FFW_DATA_NODE_WRITER

#include "node.h"

namespace ffw {
    /**
    * @ingroup data
    */
    class DataWriterException : public std::runtime_error {
    public:
        DataWriterException(const std::string& msg) :std::runtime_error(msg) {

        }
    };
    /**
    * @ingroup data
    */
    class FFW_API DataWriter {
    public:
        typedef std::string Key;
        DataWriter();
        virtual ~DataWriter();

        virtual void close() = 0;
        virtual bool isOpen() const = 0;

        virtual void stepOut() = 0;

        virtual void add(const Key& key, const Node& value) = 0;
        virtual void add(const Key& key, Node::Integer value) = 0;
        virtual void add(const Key& key, Node::Float value) = 0;
        virtual void add(const Key& key, Node::Boolean value) = 0;
        virtual void add(const Key& key, const Node::String& value) = 0;
        virtual void add(const Key& key, Node::Null value) = 0;
        virtual void addArray(const Key& key) = 0;
        virtual void addObject(const Key& key) = 0;

        virtual void add(const Node& value) = 0;
        virtual void add(Node::Integer value) = 0;
        virtual void add(Node::Float value) = 0;
        virtual void add(Node::Boolean value) = 0;
        virtual void add(const Node::String& value) = 0;
        virtual void add(Node::Null value) = 0;
        virtual void addArray() = 0;
        virtual void addObject() = 0;

        virtual void add(const Key& key, const Node& value, const Node::Object& attributes);
        virtual void add(const Key& key, Node::Integer value, const Node::Object& attributes);
        virtual void add(const Key& key, Node::Float value, const Node::Object& attributes);
        virtual void add(const Key& key, Node::Boolean value, const Node::Object& attributes);
        virtual void add(const Key& key, const Node::String& value, const Node::Object& attributes);
        virtual void add(const Key& key, Node::Null value, const Node::Object& attributes);
        virtual void addArray(const Key& key, const Node::Object& attributes);
        virtual void addObject(const Key& key, const Node::Object& attributes);

        virtual void add(const Node& value, const Node::Object& attributes);
        virtual void add(Node::Integer value, const Node::Object& attributes);
        virtual void add(Node::Float value, const Node::Object& attributes);
        virtual void add(Node::Boolean value, const Node::Object& attributes);
        virtual void add(const Node::String& value, const Node::Object& attributes);
        virtual void add(Node::Null value, const Node::Object& attributes);
        virtual void addArray(const Node::Object& attributes);
        virtual void addObject(const Node::Object& attributes);

        virtual std::string str() const = 0;
    };
}

#endif

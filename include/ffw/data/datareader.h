/* This file is part of FineFramework project */
#ifndef FFW_DATA_NODE_READER
#define FFW_DATA_NODE_READER

#include "node.h"

namespace ffw {
    /**
     * @ingroup data
     */
    class DataReaderException: public std::runtime_error {
    public:
        DataReaderException(const std::string& msg):std::runtime_error(msg) {
            
        }
    };
    /**
    * @ingroup data
    */
    class FFW_API DataIterator {
    public:
        DataIterator() = default;
        virtual ~DataIterator() = default;
        virtual std::string getKey() const = 0;
        virtual Node get(bool attrIgnore = false) = 0;

        virtual bool isEmpty() const = 0;
        virtual bool isString() const = 0;
        virtual bool isNumber() const = 0;
        virtual bool isInteger() const = 0;
        virtual bool isFloat() const = 0;
        virtual bool isBoolean() const = 0;
        virtual bool isNull() const = 0;
        virtual bool isObject() const = 0;
        virtual bool isArray() const = 0;

        virtual Node::String toString() const = 0;
        virtual Node::Integer toInteger() const = 0;
        virtual Node::Float toFloat() const = 0;
        virtual Node::Boolean toBoolean() const = 0;
        virtual Node::Array toArray(bool attrIgnore = false) const = 0;
        virtual Node::Object toObject(bool attrIgnore = false) const = 0;

        virtual operator bool() const = 0;
    };
    /**
     * @ingroup data
     */
    class FFW_API DataReader {
    public:
        DataReader();
        virtual ~DataReader();

        virtual void close() = 0;
        virtual bool isOpen() const = 0;

        virtual bool stepInto() = 0;
        virtual bool stepOut() = 0;
    };
}

#endif

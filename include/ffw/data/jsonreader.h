/* This file is part of FineFramework project */
#ifndef FFW_DATA_JSON_READER
#define FFW_DATA_JSON_READER

#include <list>
#include "datareader.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef _json_h_
#else
struct json_object;
struct json_object_iterator;
#endif
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace ffw {
    /**
    * @ingroup data
    */
    class JsonReaderException : public DataReaderException {
    public:
        JsonReaderException(const std::string& msg) :DataReaderException(msg) {

        }
    };
    /**
    * @ingroup data
    */
    class FFW_API JsonIterator : public DataIterator {
    public:
        JsonIterator();
        JsonIterator(json_object* elem, const char* key, int type);
        JsonIterator(const JsonIterator& other) = default;
        JsonIterator& operator = (const JsonIterator& other) = default;
        virtual ~JsonIterator() = default;

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
            return !isEmpty();
        }

        friend class JsonReader;
    private:
        json_object* elem;
        const char* key;
        int type;
    };
    /**
    * @ingroup data
    */
    class FFW_API JsonReader : public DataReader {
    public:
        JsonReader(const std::string& src);
        JsonReader(const char* src);
        JsonReader(JsonReader& other) = delete;
        JsonReader(JsonReader&& other) NOEXCEPT;
        JsonReader& operator = (const JsonReader& other) = delete;
        JsonReader& operator = (JsonReader&& other) NOEXCEPT;
        void swap(JsonReader& other) NOEXCEPT;
        virtual ~JsonReader();

        void close() override;
        bool isOpen() const override;

        bool getNext(JsonIterator* value);
        bool stepInto() override;
        bool stepOut() override;

    private:
        struct json_object_deleter {
            void operator()(json_object* v) const;
        };
        std::unique_ptr<json_object, json_object_deleter> root;

        struct Cache;
        std::list<Cache> cache;
        json_object* now;
    };
    /**
    * @ingroup data
    */
    FFW_API Node decodeJsonFile(const std::string& path);
    /**
    * @ingroup data
    */
    FFW_API Node decodeJson(const std::string& json);
}

inline void swap(ffw::JsonReader& first, ffw::JsonReader& second) NOEXCEPT {
    first.swap(second);
}

#endif
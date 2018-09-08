/* This file is part of FineFramework project */
#ifndef FFW_DATA_JSON_WRITER
#define FFW_DATA_JSON_WRITER

#include <list>
#include "datawriter.h"

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
    class JsonWriterException : public DataWriterException {
    public:
        JsonWriterException(const std::string& msg) :DataWriterException(msg) {

        }
    };
    /**
    * @ingroup data
    */
    class FFW_API JsonWriter : public DataWriter {
    public:
        JsonWriter();
        JsonWriter(JsonWriter& other) = delete;
        JsonWriter(JsonWriter&& other) NOEXCEPT;
        JsonWriter& operator = (const JsonWriter& other) = delete;
        JsonWriter& operator = (JsonWriter&& other) NOEXCEPT;
        void swap(JsonWriter& other) NOEXCEPT;
        virtual ~JsonWriter();

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

        std::string str() const override;
    private:
        struct json_object_deleter {
            void operator()(json_object* v) const;
        };
        std::unique_ptr<json_object, json_object_deleter> root;

        struct Cache;
        std::list<Cache> cache;
    };
    /**
    * @ingroup data
    */
    FFW_API void encodeJsonFile(const ffw::Node& json, const std::string& path);
    /**
    * @ingroup data
    */
    FFW_API std::string encodeJson(const ffw::Node& json);
}

inline void swap(ffw::JsonWriter& first, ffw::JsonWriter& second) NOEXCEPT {
    first.swap(second);
}

#endif
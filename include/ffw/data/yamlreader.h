/* This file is part of FineFramework project */
#ifndef FFW_DATA_YAML_READER
#define FFW_DATA_YAML_READER

#include <list>
#include "datareader.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace YAML {
    class Node;
    class Parser;
}
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace ffw {
    /**
    * @ingroup data
    */
    class YamlReaderException : public DataReaderException {
    public:
        YamlReaderException(const std::string& msg) :DataReaderException(msg) {

        }
    };
    /**
    * @ingroup data
    */
    class FFW_API YamlIterator : public DataIterator {
    public:
        YamlIterator();
        YamlIterator(YAML::Node elem);
        YamlIterator(YAML::Node elem, YAML::Node key);
        YamlIterator(const YamlIterator& other) = default;
        YamlIterator& operator = (const YamlIterator& other) = default;
        virtual ~YamlIterator() = default;

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

        friend class YamlReader;
    private:
        struct Ref;
        std::shared_ptr<Ref> ref;
    };
    /**
    * @ingroup data
    */
    class FFW_API YamlReader : public DataReader {
    public:
        YamlReader(const std::string& src);
        YamlReader(const char* src);
        YamlReader(YamlReader& other) = delete;
        YamlReader(YamlReader&& other) NOEXCEPT;
        YamlReader& operator = (const YamlReader& other) = delete;
        YamlReader& operator = (YamlReader&& other) NOEXCEPT;
        void swap(YamlReader& other) NOEXCEPT;
        virtual ~YamlReader();

        void close() override;
        bool isOpen() const override;

        bool getNext(YamlIterator* value);
        bool stepInto() override;
        bool stepOut() override;

        bool isRootSequence() const;
    private:
        std::unique_ptr<YAML::Node> doc;

        struct Cache;
        std::list<Cache> cache;
    };
    /**
    * @ingroup data
    */
    FFW_API Node decodeYamlFile(const std::string& path);
    /**
    * @ingroup data
    */
    FFW_API Node decodeYaml(const std::string& json);
}

inline void swap(ffw::YamlReader& first, ffw::YamlReader& second) NOEXCEPT {
    first.swap(second);
}

#endif

/* This file is part of FineFramework project */
#ifndef FFW_DATA_YAML_WRITER
#define FFW_DATA_YAML_WRITER

#include "datawriter.h"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace YAML {
    class Emitter;
}
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

namespace ffw {
    class YamlWriterException : public DataWriterException {
    public:
        YamlWriterException(const std::string& msg) :DataWriterException(msg) {

        }
    };

    class FFW_API YamlWriter : public DataWriter {
    public:
        YamlWriter();
        YamlWriter(YamlWriter& other) = delete;
        YamlWriter(YamlWriter&& other) NOEXCEPT;
        YamlWriter& operator = (const YamlWriter& other) = delete;
        YamlWriter& operator = (YamlWriter&& other) NOEXCEPT;
        void swap(YamlWriter& other) NOEXCEPT;
        virtual ~YamlWriter();

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
        std::unique_ptr<YAML::Emitter> doc;
        struct Cache;
        std::list<Cache> cache;
    };
}

inline void swap(ffw::YamlWriter& first, ffw::YamlWriter& second) NOEXCEPT {
    first.swap(second);
}

#endif
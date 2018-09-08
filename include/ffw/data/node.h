/* This file is part of FineFramework project */
#ifndef FFW_DATA_NODE
#define FFW_DATA_NODE

#include <memory>
#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include "config.h"

namespace ffw {
    /**
    * @ingroup data
    */
    class FFW_API Node {
    public:
        typedef long long Integer;
        typedef double Float;
        typedef std::string String;
        typedef bool Boolean;
        typedef std::nullptr_t Null;

        class Object;

        class Array;

        enum class Type {
            OBJECT,
            ARRAY,
            INTEGER,
            FLOAT,
            BOOLEAN,
            STRING,
            NULLVAL,
            EMPTY
        };

        class Content {
        public:
            Content() = default;
            Content(const Content& other) = delete;
            Content& operator = (const Content& other) = delete;
            Content(Content&& other) = delete;
            Content& operator = (Content&& other) = delete;
            virtual ~Content() = default;
            virtual Content* createCopy() = 0;
            virtual Type getType() const = 0;
            virtual bool compare(const Content* other) const = 0;
        };

        template<class T>
        class Data : public Content {
        public:
            /*template<class ... Args>
            Data(Args&&... args) :value(std::forward<Args>(args)...) {
            }*/
            Data(T value):value(std::move(value)) {
            }
            Data(const Data& other) = delete;
            Data& operator = (const Data& other) = delete;
            Data(Data&& other) = delete;
            Data& operator = (Data&& other) = delete;
            virtual ~Data() = default;
            Data* createCopy() override {
                return new Data{ value };
            }
            Type getType() const override;
            T& get() {
                return value;
            }
            const T& get() const {
                return value;
            }
            bool compare(const Content* other) const override {
                return value == static_cast<const Data<T>&>(*other).value;
            }
        private:
            T value;
        };

        inline Node() :content(nullptr) {
        }

        inline Node(const Node& other) : content(nullptr) {
            if (!other.empty()) {
                content.reset(other.content->createCopy());
            }
        }

        inline Node(Node&& other) NOEXCEPT : content(nullptr) {
            std::swap(content, other.content);
        }

        inline Node(const Integer value) : content(nullptr) {
            content.reset(new Data<Integer>{ value });
        }

        inline Node(const unsigned long long value) : content(nullptr) {
            content.reset(new Data<Integer>{ static_cast<Integer>(value) });
        }

        inline Node(const int value) : content(nullptr) {
            content.reset(new Data<Integer>{ static_cast<Integer>(value) });
        }

        inline Node(const Float value) : content(nullptr) {
            content.reset(new Data<Float>{ value });
        }

        inline Node(const bool value) : content(nullptr) {
            content.reset(new Data<Boolean>{ value });
        }

        inline Node(std::string value) : content(nullptr) {
            content.reset(new Data<String>(std::move(value)));
        }

        inline Node(const char* value) : content(nullptr) {
            content.reset(new Data<String>(std::string(value)));
        }

        inline Node(std::nullptr_t value) : content(nullptr) {
            content.reset(new Data<Null>(value));
        }

        inline Node(Object value);

        inline Node(Array value);

        inline Node(std::initializer_list<std::pair<const std::string, Node>> list);

        inline Node(const std::initializer_list<Node>& list);

        inline virtual ~Node() = default;

        inline bool empty() const {
            return (content == nullptr);
        }

        inline void reset() {
            content.reset();
        }

        inline Integer& getAsInt() {
            if (!isInt())throw std::bad_cast();
            return dynamic_cast<Data<Integer>*>(content.get())->get();
        }

        inline Float& getAsFloat() {
            if (!isFloat())throw std::bad_cast();
            return dynamic_cast<Data<Float>*>(content.get())->get();
        }

        inline String& getAsString() {
            if (!isString())throw std::bad_cast();
            return dynamic_cast<Data<String>*>(content.get())->get();
        }

        inline Boolean& getAsBool() {
            if (!isBool())throw std::bad_cast();
            return dynamic_cast<Data<Boolean>*>(content.get())->get();
        }

        inline Array& getAsArray();

        inline Object& getAsObject();

        inline const Integer& getAsInt() const {
            if (!isInt())throw std::bad_cast();
            return dynamic_cast<Data<Integer>*>(content.get())->get();
        }

        inline const Float& getAsFloat() const {
            if (!isFloat())throw std::bad_cast();
            return dynamic_cast<Data<Float>*>(content.get())->get();
        }

        inline const String& getAsString() const {
            if (!isString())throw std::bad_cast();
            return dynamic_cast<Data<String>*>(content.get())->get();
        }

        inline const Boolean& getAsBool() const {
            if (!isBool())throw std::bad_cast();
            return dynamic_cast<Data<Boolean>*>(content.get())->get();
        }

        inline const Array& getAsArray() const;

        inline const Object& getAsObject() const;

        inline void set(const Integer value) {
            *this = value;
        }

        inline void set(const Float value) {
            *this = value;
        }

        inline void set(const Boolean value) {
            *this = value;
        }

        inline void set(const String& value) {
            *this = value;
        }

        inline void set(const Null& value) {
            *this = value;
        }

        inline bool is(const Type type) const {
            if (empty())return false;
            return (content->getType() == type);
        }

        inline bool isBool() const {
            if (empty())return false;
            return content->getType() == Type::BOOLEAN;
        }

        inline bool isString() const {
            if (empty())return false;
            return content->getType() == Type::STRING;
        }

        inline bool isInt() const {
            if (empty())return false;
            return content->getType() == Type::INTEGER;
        }

        inline bool isFloat() const {
            if (empty())return false;
            return content->getType() == Type::FLOAT;
        }

        inline bool isNumber() const {
            if (empty())return false;
            return content->getType() == Type::FLOAT || content->getType() == Type::INTEGER;
        }

        inline bool isObject() const {
            if (empty())return false;
            return content->getType() == Type::OBJECT;
        }

        inline bool isArray() const {
            if (empty())return false;
            return content->getType() == Type::ARRAY;
        }

        inline bool isNull() const {
            if (empty())return false;
            return content->getType() == Type::NULLVAL;
        }

        inline void clear() {
            content.reset();
        }

        inline Type getType() const {
            if (empty())return Type::EMPTY;
            return content->getType();
        }

        inline void swap(Node& other) NOEXCEPT {
            using std::swap;
            swap(content, other.content);
        }

        inline Node& operator = (const Integer value) {
            content.reset(new Data<Integer>(value));
            return *this;
        }

        inline Node& operator = (const int value) {
            content.reset(new Data<Integer>(static_cast<Integer>(value)));
            return *this;
        }

        inline Node& operator = (const Float value) {
            content.reset(new Data<Float>(value));
            return *this;
        }

        inline Node& operator = (const Boolean value) {
            content.reset(new Data<Boolean>(value));
            return *this;
        }

        inline Node& operator = (String value) {
            content.reset(new Data<String>(std::move(value)));
            return *this;
        }

        inline Node& operator = (const char* value) {
            content.reset(new Data<String>(std::string(value)));
            return *this;
        }

        inline Node& operator = (Array value);

        inline Node& operator = (Object value);

        inline Node& operator = (Null value) {
            content.reset(new Data<Null>(value));
            return *this;
        }

        inline Node& operator = (const Node& other) {
            Node(other).swap(*this);
            return *this;
        }

        inline Node& operator = (Node&& other) NOEXCEPT {
            Node(other).swap(*this);
            return *this;
        }

        inline Node& operator [] (const std::string& key);

        inline const Node& operator [] (const std::string& key) const;

        inline Node& operator [] (const size_t n);

        inline const Node& operator [] (const size_t n) const;

        template<class T> inline bool operator == (const T& other) const;

        template<class T> inline bool operator != (const T& other) const {
            return !(*this == other);
        }

        inline bool operator == (const Node& other) const {
            if (!empty() && other.content != nullptr && content->getType() == other.content->getType()) {
                return content->compare(other.content.get());
            }
            else {
                return false;
            }
        }

        inline bool operator != (const Node& other) const {
            if (!empty() && other.content != nullptr && content->getType() == other.content->getType()) {
                return !content->compare(other.content.get());
            }
            else {
                return true;
            }
        }

        template<class T> inline explicit operator T() const {
            return getAs<T>();
        }

    private:
        template<class T> inline T& getAs();
        template<class T> inline const T& getAs() const;
        std::unique_ptr<Content> content;
    };

    class Node::Object: public std::unordered_map<std::string, Node> {
    public:
        inline Object() = default;

        inline Object(const std::initializer_list<std::pair<const std::string, Node>>& list) {
            reserve(list.size());
            for (auto& pair : list) {
                insert(pair);
            }
        }

        inline Object& operator = (const std::initializer_list<std::pair<const std::string, Node>>& list) {
            Object n;
            n.reserve(list.size());
            for (auto& pair : list) {
                n.insert(pair);
            }
            std::swap(*this, n);
            return *this;
        }

        inline bool contains(const std::string& key) const {
            return find(key) != end();
        }

        inline Node& operator [] (const char* key) {
            return at(key);
        }

        inline Node& operator [] (const std::string& key) {
            return at(key);
        }

        inline const Node& operator [] (const char* key) const{
            return at(key);
        }

        inline const Node& operator [] (const std::string& key) const {
            return at(key);
        }
    };
    
    class Node::Array: public std::vector<Node> {
    public:
        using std::vector<Node>::vector;
    };

    inline Node::Node(Node::Object value) : content(nullptr) {
        content.reset(new Data<Object>(std::move(value)));
    }

    inline Node::Node(Node::Array value) : content(nullptr) {
        content.reset(new Data<Array>(std::move(value)));
    }

    inline Node::Node(std::initializer_list<std::pair<const std::string, Node>> list) {
        Object obj(std::forward<std::initializer_list<std::pair<const std::string, Node>>>(list));
        content.reset(new Data<Object>(std::move(obj)));
    }

    inline Node::Node(const std::initializer_list<Node>& list) {
        Array arr(list);
        content.reset(new Data<Array>(std::move(arr)));
    }

    inline Node::Array& Node::getAsArray() {
        if (!isArray())throw std::bad_cast();
        return dynamic_cast<Data<Array>*>(content.get())->get();
    }

    inline Node::Object& Node::getAsObject() {
        if (!isObject())throw std::bad_cast();
        return dynamic_cast<Data<Object>*>(content.get())->get();
    }

    inline const Node::Array& Node::getAsArray() const {
        if (!isArray())throw std::bad_cast();
        return dynamic_cast<Data<Array>*>(content.get())->get();
    }

    inline const Node::Object& Node::getAsObject() const {
        if (!isObject())throw std::bad_cast();
        return dynamic_cast<Data<Object>*>(content.get())->get();
    }

    inline Node& Node::operator = (Array value) {
        content.reset(new Data<Array>(std::move(value)));
        return *this;
    }

    inline Node& Node::operator = (Object value) {
        content.reset(new Data<Object>(std::move(value)));
        return *this;
    }

    inline Node& Node::operator [] (const std::string& key) {
        if (empty() || !isObject())throw std::bad_cast();
        return getAsObject()[key];
    }

    inline const Node& Node::operator [] (const std::string& key) const {
        if (empty() || !isObject())throw std::bad_cast();
        return getAsObject().at(key);
    }

    inline Node& Node::operator [] (const size_t n) {
        if (empty() || !isArray())throw std::bad_cast();
        return getAsArray()[n];
    }

    inline const Node& Node::operator [] (const size_t n) const {
        if (empty() || !isArray())throw std::bad_cast();
        return getAsArray()[n];
    }

    template<typename T>
    inline Node::Type modeTypeFromTemplate() {
        return Node::Type::EMPTY;
    }

    template<>
    inline Node::Type modeTypeFromTemplate<Node::Integer>() {
        return Node::Type::INTEGER;
    }

    template<>
    inline Node::Type modeTypeFromTemplate<Node::Float>() {
        return Node::Type::FLOAT;
    }

    template<>
    inline Node::Type modeTypeFromTemplate<Node::String>() {
        return Node::Type::STRING;
    }

    template<>
    inline Node::Type modeTypeFromTemplate<Node::Boolean>() {
        return Node::Type::BOOLEAN;
    }

    template<>
    inline Node::Type modeTypeFromTemplate<Node::Null>() {
        return Node::Type::NULLVAL;
    }

    template<>
    inline Node::Type modeTypeFromTemplate<Node::Object>() {
        return Node::Type::OBJECT;
    }

    template<>
    inline Node::Type modeTypeFromTemplate<Node::Array>() {
        return Node::Type::ARRAY;
    }

    template<> inline Node::operator int() const {
        return static_cast<int>(getAs<Node::Integer>());
    }

    template<> inline Node::operator long() const {
        return static_cast<long>(getAs<Node::Integer>());
    }

    template<> inline Node::operator float() const {
        return static_cast<float>(getAs<Node::Integer>());
    }

    template<class T> 
    inline T& Node::getAs() {
        if (content == nullptr || content->getType() != modeTypeFromTemplate<typename std::remove_const<T>::type>()) {
            throw std::bad_cast();
        }
        return static_cast<Data<T>*>(content.get())->get();
    }

    template<class T> 
    inline const T& Node::getAs() const {
        if (content == nullptr || content->getType() != modeTypeFromTemplate<typename std::remove_const<T>::type>()) {
            throw std::bad_cast();
        }
        return static_cast<const Data<T>*>(content.get())->get();
    }

    template<class T>
    inline Node::Type Node::Data<T>::getType() const {
        return modeTypeFromTemplate<T>();
    }

    template<class T> inline bool Node::operator == (const T& other) const {
        if (!empty() && content->getType() == modeTypeFromTemplate<typename std::remove_const<T>::type>()) {
            return dynamic_cast<const Data<T>*>(content.get())->get() == other;
        }
        return false;
    }

    template<> inline bool Node::operator == (const float& other) const {
        if (!empty() && content->getType() == modeTypeFromTemplate<Float>()) {
            return dynamic_cast<const Data<Float>*>(content.get())->get() == static_cast<Float>(other);
        }
        return false;
    }

    template<> inline bool Node::operator == (const int& other) const {
        if (!empty() && content->getType() == modeTypeFromTemplate<Integer>()) {
            return dynamic_cast<const Data<Integer>*>(content.get())->get() == static_cast<Integer>(other);
        }
        return false;
    }

    template<> inline bool Node::operator == (const long& other) const {
        if (!empty() && content->getType() == modeTypeFromTemplate<Integer>()) {
            return dynamic_cast<const Data<Integer>*>(content.get())->get() == static_cast<Integer>(other);
        }
        return false;
    }
}

#endif

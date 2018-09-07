
class Formatter
{
public:
    Formatter() {}
    ~Formatter() {}

    template <typename Type>
    Formatter & operator << (const Type & value)
    {
        stream_ << value;
        return *this;
    }

    std::string str() const { return stream_.str(); }
    operator std::string() const { return stream_.str(); }

    enum ConvertToString
    {
        to_str
    };
    std::string operator >> (ConvertToString) { return stream_.str(); }

private:
    std::stringstream stream_;

    Formatter(const Formatter &);
    Formatter & operator = (Formatter &);
};

static void compare(const ffw::Node& a, const ffw::Node& b) {
    if (a.empty() && b.empty())return;
    else if (a.empty() || b.empty())return;

    if (a.isBool() && b.isBool()) {
        if (a.getAsBool() == b.getAsBool())return;
        else {
            throw std::runtime_error(Formatter() << "Boolean mismatch: " << a.getAsBool() << " vs " << b.getAsBool());
        }
    }

    if (a.isInt() && b.isInt()) {
        if (a.getAsInt() == b.getAsInt())return;
        else {
            throw std::runtime_error(Formatter() << "Integer mismatch: " << a.getAsInt() << " vs " << b.getAsInt());
        }
    }

    if (a.isFloat() && b.isFloat()) {
        if (std::abs(a.getAsFloat() - b.getAsFloat()) < 0.0001f)return;
        else {
            throw std::runtime_error(Formatter() << "Float mismatch: " << a.getAsFloat() << " vs " << b.getAsFloat());
        }
    }

    if (a.isString() && b.isString()) {
        if (a.getAsString() == b.getAsString())return;
        else {
            throw std::runtime_error(Formatter() << "String mismatch: " << a.getAsString() << " vs " << b.getAsString());
        }
    }

    if (a.isObject() && b.isObject()) {
        auto ao = a.getAsObject();
        auto bo = b.getAsObject();

        // Check if both have same amount of pairs
        if (ao.size() != bo.size()) {
            throw std::runtime_error(Formatter() << "Objects do not have same size! " << ao.size() << " vs " << bo.size());
        }

        for (const auto pair : ao) {
            // Check if both have key
            if (bo.find(pair.first) == bo.end()) {
                throw std::runtime_error(Formatter() << "Key \"" << pair.first << "\" is missing!");
            }

            try {
                compare(pair.second, bo.at(pair.first));
            } catch (std::exception& e) {
                throw std::runtime_error(Formatter() << "Key \"" << pair.first << "\" error: " << e.what());
            }
        }

        return;
    }

    if (a.isArray() && b.isArray()) {
        auto ao = a.getAsArray();
        auto bo = b.getAsArray();

        // Check if both have same amount of pairs
        if (ao.size() != bo.size()) {
            throw std::runtime_error(Formatter() << "Arrays do not have same size! " << ao.size() << " vs " << bo.size());
        }

        for (size_t i = 0; i < ao.size(); i++) {
            try {
                compare(ao[i], bo[i]);
            }
            catch (std::exception& e) {
                throw std::runtime_error(Formatter() << "index " << i << " error: " << e.what());
            }
        }

        return;
    }
}
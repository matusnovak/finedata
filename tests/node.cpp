#include <ffw/data/node.h>
#define CATCH_CONFIG_MAIN
#include "./catch.hpp"

TEST_CASE("Testing Node from template", "ffw::Node") {
    REQUIRE(ffw::modeTypeFromTemplate<long long>() == ffw::Node::Type::INTEGER);
    REQUIRE(ffw::modeTypeFromTemplate<double>() == ffw::Node::Type::FLOAT);
    REQUIRE(ffw::modeTypeFromTemplate<bool>() == ffw::Node::Type::BOOLEAN);
    REQUIRE(ffw::modeTypeFromTemplate<std::string>() == ffw::Node::Type::STRING);
    REQUIRE(ffw::modeTypeFromTemplate<ffw::Node::Object>() == ffw::Node::Type::OBJECT);
    REQUIRE(ffw::modeTypeFromTemplate<ffw::Node::Array>() == ffw::Node::Type::ARRAY);
    REQUIRE(ffw::modeTypeFromTemplate<std::nullptr_t>() == ffw::Node::Type::NULLVAL);
}

TEST_CASE("Testing Any", "ffw::Node") {
    ffw::Node var[8];

    var[0] = 123;
    var[1] = 123.4f;
    var[2] = 123.4;
    var[3] = 'A';
    var[4] = (const char*)"Abc";
    var[5] = true;
    var[6] = false;
    var[7] = std::string("Abc");

    REQUIRE(var[0].isInt());
    REQUIRE(var[1].isFloat());
    REQUIRE(var[2].isFloat());
    REQUIRE(var[3].isInt());
    REQUIRE(var[4].isString());
    REQUIRE(var[5].isBool());
    REQUIRE(var[6].isBool());
    REQUIRE(var[7].isString());

    // Reassign
    var[7] = 123;
    var[6] = 123.4f;
    var[5] = 123.4;
    var[4] = 'A';
    var[3] = (const char*)"Abc";
    var[2] = true;
    var[1] = false;
    var[0] = std::string("Abc");

    REQUIRE(var[0].isString());
    REQUIRE(var[1].isBool());
    REQUIRE(var[2].isBool());
    REQUIRE(var[3].isString());
    REQUIRE(var[4].isInt());
    REQUIRE(var[5].isFloat());
    REQUIRE(var[6].isFloat());
    REQUIRE(var[7].isInt());

    REQUIRE((std::string)var[0] == "Abc");
    REQUIRE((bool)var[1] == false);
    REQUIRE((bool)var[2] == true);
    REQUIRE((std::string)var[3] != "DDD");
    REQUIRE((long)var[4] == 'A');
    REQUIRE((double)var[5] == Approx(123.4));
    REQUIRE((double)var[6] == Approx(123.4f));
    REQUIRE((long)var[7] == 123);

    //REQUIRE(var[1] == false);
    //REQUIRE(var[2] == true);
    REQUIRE(var[4] != 'A');
    REQUIRE(var[4] == long('A'));
    REQUIRE(var[5] == 123.4);
    REQUIRE(var[6] == 123.4f);
    REQUIRE(var[7] == 123);

    ffw::Node a = 42;
    ffw::Node b = a;

    REQUIRE((long)a == 42);
    REQUIRE((long)b == 42);

    REQUIRE(a == b);

    a = 41;
    REQUIRE(a != b);

    ffw::Node c = std::string("Hello");
    REQUIRE(c == std::string("Hello"));
}

TEST_CASE("Testing Any Conversion", "ffw::Node") {
    ffw::Node a;
    REQUIRE(a.isFloat() == false);
    REQUIRE(a.isInt() == false);
    REQUIRE(a.isNumber() == false);

    a = float(1.23f);
    REQUIRE(a.isFloat() == true);
    REQUIRE(a.isInt() == false);
    REQUIRE(a.isNumber() == true);
    REQUIRE_THROWS(a.getAsInt());
    REQUIRE(a.getAsFloat() == Approx(1.23f));

    a = double(1.23);
    REQUIRE(a.isFloat() == true);
    REQUIRE(a.isInt() == false);
    REQUIRE(a.isNumber() == true);
    REQUIRE_THROWS(a.getAsInt() == 1);
    REQUIRE(a.getAsFloat() == Approx(1.23f));

    a = 123;
    REQUIRE(a.isFloat() == false);
    REQUIRE(a.isInt() == true);
    REQUIRE(a.isNumber() == true);
    REQUIRE(a.getAsInt() == 123);
    REQUIRE_THROWS(a.getAsFloat());
}

TEST_CASE("Testing Any Object", "ffw::Node::Object") {
    ffw::Node::Object o;
    REQUIRE(o.size() == 0);

    o = { { "hello", 123 } };
    REQUIRE(o.size() == 1);

    ffw::Node copy;
    REQUIRE_NOTHROW(copy = o["hello"]);
    REQUIRE(123 == (long)o["hello"]);

    o.insert({ "world", 42.0f });
    REQUIRE(o.size() == 2);

    REQUIRE_NOTHROW(copy = o["world"]);
    REQUIRE(42.0f == (double)o["world"]);

    REQUIRE_THROWS(copy = o["error"]);

    ffw::Node::Object obj = {
        { "first", 10 },
        { "second", 42.1 },
        { "third", std::string("Hello World!") },
        { "fourth", 0x12345678 },
        { "fifth", true },
        { "sixth", 88.99 }
    };

    REQUIRE(obj.size() == 6);

    const auto& first = obj["first"];
    const auto& second = obj["second"];
    const auto& third = obj["third"];
    const auto& fourth = obj["fourth"];
    const auto& fifth = obj["fifth"];
    const auto& sixth = obj["sixth"];

    REQUIRE(first.isInt() == true);
    REQUIRE(second.isFloat() == true);
    REQUIRE(third.isString() == true);
    REQUIRE(fourth.isInt() == true);
    REQUIRE(fifth.isBool() == true);
    REQUIRE(sixth.isFloat() == true);

    obj["first"].getAsInt() = 42;

    REQUIRE((long)obj["first"] == 42);
    REQUIRE(obj["first"] == 42);
    REQUIRE(obj["first"] != 42.0f);

    obj["first"] = 55;

    REQUIRE(obj["first"] == 55);

    ffw::Node::Object big = {
        { "first", ffw::Node::Object{ { "hello", 42 },{ "world", true } }
        },
        { "second", ffw::Node::Object{ { "who", "doctor" },{ "world", false } }
        },
    };

    const auto& hello = big["first"]["hello"];
    REQUIRE(hello.isInt() == true);
    REQUIRE(42 == (long)hello);

    const auto& who = big["second"]["who"];
    REQUIRE(who.isString() == true);

    std::string test = (std::string)who;

    REQUIRE(big["second"]["who"] == std::string("doctor"));
    REQUIRE(test == "doctor");
}

TEST_CASE("Testing Any Array", "ffw::Node::Array") {
    ffw::Node::Array a;
    REQUIRE(a.size() == 0);

    a = { "hello", 123, 42.0f, true };

    REQUIRE(a.size() == 4);

    REQUIRE(a[0].isString() == true);
    REQUIRE(a[1].isInt() == true);
    REQUIRE(a[2].isFloat() == true);
    REQUIRE(a[3].isBool() == true);

    REQUIRE((bool)a[3] == true);

    a[3] = false;

    REQUIRE(a[3].isBool() == true);

    REQUIRE((bool)a[3] == false);
}

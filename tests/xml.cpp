#include <ffw/data/xmlreader.h>
#include <ffw/data/xmlwriter.h>

#define CATCH_CONFIG_MAIN
#include "./catch.hpp"
#include "compare.h"

TEST_CASE("Decode XML types", "ffw::XmlReader") {
    ffw::XmlReader xml(
        "<menu id=\"file\" value=\"\">"
            "<popup>"
                "<menuitem value=\"New\" onclick=\"CreateNewDoc()\" />"
                "<menuitem value=\"Open\" onclick=\"OpenDoc()\" />"
                "<menuitem value=\"Close\" onclick=\"CloseDoc()\" />"
            "</popup>"
            "<values>"
                "<integer attr=\"123\">123</integer>"
                "<float attr=\"+0.123e-1\">+0.123e-1</float>"
                "<boolean attr=\"true\">true</boolean>"
                "<null attr=\"null\">null</null>"
            "</values>"
        "</menu>"
    );

    ffw::XmlIterator node, attr;
    REQUIRE(!node);

    REQUIRE(xml.getNext(&node));
    REQUIRE(node);
    REQUIRE(node.getKey() == "menu");

    CHECK(node.isObject() == true);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == false);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);

    {
        REQUIRE(node.getNextAttribute(&attr) == true);
        REQUIRE(attr.getKey() == "id");

        CHECK(attr.isObject() == false);
        CHECK(attr.isEmpty() == false);
        CHECK(attr.isString() == true);
        CHECK(attr.isNumber() == false);
        CHECK(attr.isInteger() == false);
        CHECK(attr.isFloat() == false);
        CHECK(attr.isBoolean() == false);
        CHECK(attr.isNull() == false);

        REQUIRE(node.getNextAttribute(&attr) == true);
        REQUIRE(attr.getKey() == "value");

        CHECK(attr.isObject() == false);
        CHECK(attr.isEmpty() == true);
        CHECK(attr.isString() == false);
        CHECK(attr.isNumber() == false);
        CHECK(attr.isInteger() == false);
        CHECK(attr.isFloat() == false);
        CHECK(attr.isBoolean() == false);
        CHECK(attr.isNull() == false);
    }

    // Stepping into <menu>
    REQUIRE(xml.stepInto());

    // Get popup=
    REQUIRE(xml.getNext(&node) == true);
    REQUIRE(node.getKey() == "popup");

    CHECK(node.isObject() == true);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == false);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);
    CHECK(node.hasAttributes() == false);

    {
        REQUIRE(xml.stepInto() == true);

        REQUIRE(xml.getNext(&node) == true);
        REQUIRE(node.getKey() == "menuitem");

        CHECK(node.isObject() == false);
        CHECK(node.isEmpty() == true);
        CHECK(node.isString() == false);
        CHECK(node.isNumber() == false);
        CHECK(node.isInteger() == false);
        CHECK(node.isFloat() == false);
        CHECK(node.isBoolean() == false);
        CHECK(node.isNull() == false);
        CHECK(node.hasAttributes() == true);

        {
            REQUIRE(node.getNextAttribute(&attr) == true);
            REQUIRE(attr.getKey() == "value");

            CHECK(attr.isObject() == false);
            CHECK(attr.isEmpty() == false);
            CHECK(attr.isString() == true);
            CHECK(attr.isNumber() == false);
            CHECK(attr.isInteger() == false);
            CHECK(attr.isFloat() == false);
            CHECK(attr.isBoolean() == false);
            CHECK(attr.isNull() == false);

            REQUIRE(node.getNextAttribute(&attr) == true);
            REQUIRE(attr.getKey() == "onclick");

            CHECK(attr.isObject() == false);
            CHECK(attr.isEmpty() == false);
            CHECK(attr.isString() == true);
            CHECK(attr.isNumber() == false);
            CHECK(attr.isInteger() == false);
            CHECK(attr.isFloat() == false);
            CHECK(attr.isBoolean() == false);
            CHECK(attr.isNull() == false);
        }

        REQUIRE(xml.getNext(&node) == true);
        REQUIRE(node.getKey() == "menuitem");

        REQUIRE(xml.getNext(&node) == true);
        REQUIRE(node.getKey() == "menuitem");

        REQUIRE(xml.getNext(&node) == false);

        REQUIRE(xml.stepOut());
    }

    // Get <values>
    REQUIRE(xml.getNext(&node) == true);

    CHECK(node.isObject() == true);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == false);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);

    {
        REQUIRE(xml.stepInto() == true);

        REQUIRE(xml.getNext(&node) == true);
        REQUIRE(node.getKey() == "integer");
        REQUIRE(node.getNextAttribute(&attr) == true);
        REQUIRE(attr.getKey() == "attr");

        CHECK(node.isObject() == false);
        CHECK(node.isEmpty() == false);
        CHECK(node.isString() == true);
        CHECK(node.isNumber() == true);
        CHECK(node.isInteger() == true);
        CHECK(node.isFloat() == false);
        CHECK(node.isBoolean() == false);
        CHECK(node.isNull() == false);

        CHECK(attr.isObject() == false);
        CHECK(attr.isEmpty() == false);
        CHECK(attr.isString() == true);
        CHECK(attr.isNumber() == true);
        CHECK(attr.isInteger() == true);
        CHECK(attr.isFloat() == false);
        CHECK(attr.isBoolean() == false);
        CHECK(attr.isNull() == false);

        REQUIRE(xml.getNext(&node) == true);
        REQUIRE(node.getKey() == "float");
        REQUIRE(node.getNextAttribute(&attr) == true);
        REQUIRE(attr.getKey() == "attr");

        CHECK(node.isObject() == false);
        CHECK(node.isEmpty() == false);
        CHECK(node.isString() == true);
        CHECK(node.isNumber() == true);
        CHECK(node.isInteger() == false);
        CHECK(node.isFloat() == true);
        CHECK(node.isBoolean() == false);
        CHECK(node.isNull() == false);

        CHECK(attr.isObject() == false);
        CHECK(attr.isEmpty() == false);
        CHECK(attr.isString() == true);
        CHECK(attr.isNumber() == true);
        CHECK(attr.isInteger() == false);
        CHECK(attr.isFloat() == true);
        CHECK(attr.isBoolean() == false);
        CHECK(attr.isNull() == false);

        REQUIRE(xml.getNext(&node) == true);
        REQUIRE(node.getKey() == "boolean");
        REQUIRE(node.getNextAttribute(&attr) == true);
        REQUIRE(attr.getKey() == "attr");

        CHECK(node.isObject() == false);
        CHECK(node.isEmpty() == false);
        CHECK(node.isString() == true);
        CHECK(node.isNumber() == false);
        CHECK(node.isInteger() == false);
        CHECK(node.isFloat() == false);
        CHECK(node.isBoolean() == true);
        CHECK(node.isNull() == false);

        CHECK(attr.isObject() == false);
        CHECK(attr.isEmpty() == false);
        CHECK(attr.isString() == true);
        CHECK(attr.isNumber() == false);
        CHECK(attr.isInteger() == false);
        CHECK(attr.isFloat() == false);
        CHECK(attr.isBoolean() == true);
        CHECK(attr.isNull() == false);

        REQUIRE(xml.getNext(&node) == true);
        REQUIRE(node.getKey() == "null");
        REQUIRE(node.getNextAttribute(&attr) == true);
        REQUIRE(attr.getKey() == "attr");

        CHECK(node.isObject() == false);
        CHECK(node.isEmpty() == false);
        CHECK(node.isString() == true);
        CHECK(node.isNumber() == false);
        CHECK(node.isInteger() == false);
        CHECK(node.isFloat() == false);
        CHECK(node.isBoolean() == false);
        CHECK(node.isNull() == true);

        CHECK(attr.isObject() == false);
        CHECK(attr.isEmpty() == false);
        CHECK(attr.isString() == true);
        CHECK(attr.isNumber() == false);
        CHECK(attr.isInteger() == false);
        CHECK(attr.isFloat() == false);
        CHECK(attr.isBoolean() == false);
        CHECK(attr.isNull() == true);

        REQUIRE(xml.getNext(&node) == false);

        // Back to <menu>
        REQUIRE(xml.stepOut() == true);
    }

    REQUIRE(xml.getNext(&node) == false);

    // Back to root
    REQUIRE(xml.stepOut() == true);

    // Cannot get more back
    REQUIRE(xml.stepOut() == false);
}

TEST_CASE("Decode XML values", "ffw::XmlReader") {
    ffw::XmlReader xml(
        "<menu id=\"file\" value=\"\">"
            "<popup>"
                "<menuitem value=\"New\" onclick=\"CreateNewDoc()\" />"
                "<menuitem value=\"Open\" onclick=\"OpenDoc()\" />"
                "<menuitem value=\"Close\" onclick=\"CloseDoc()\" />"
            "</popup>"
            "<values>"
                "<integer attr=\"123\">123</integer>"
                "<float attr=\"+0.123e-1\">+0.123e-1</float>"
                "<boolean attr=\"true\">true</boolean>"
                "<null attr=\"null\">null</null>"
            "</values>"
        "</menu>"
    );

    ffw::XmlIterator node, attr;
    REQUIRE(!node);

    REQUIRE(xml.getNext(&node));
    REQUIRE(xml.stepInto());
    REQUIRE(node);
    REQUIRE(node.getKey() == "menu");
    REQUIRE(node.isObject() == true);
    REQUIRE(node.isArray() == false);

    REQUIRE(xml.getNext(&node)); // popup
    REQUIRE(node.isObject() == true);
    REQUIRE(node.isArray() == false);
    REQUIRE(xml.stepInto());

    REQUIRE(xml.getNext(&node)); // menuitem
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == true);
    REQUIRE(node.getKey() == "menuitem");

    REQUIRE(node.getNextAttribute(&attr)); REQUIRE(node.getNextAttribute(&attr));
    REQUIRE(attr.toString() == "CreateNewDoc()");
    REQUIRE(attr.get().isString());
    REQUIRE(attr.get().getAsString() == "CreateNewDoc()");
    REQUIRE(attr.isObject() == false);
    REQUIRE(attr.isArray() == false);

    REQUIRE(xml.stepOut());
    REQUIRE(xml.getNext(&node)); // values
    REQUIRE(node.isObject() == true);
    REQUIRE(node.isArray() == false);
    REQUIRE(xml.stepInto());

    REQUIRE(xml.getNext(&node)); // integer
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == false);
    REQUIRE(node.getKey() == "integer");
    REQUIRE(node.toString() == "123");
    REQUIRE(node.toInteger() == 123);
    REQUIRE(node.toFloat() == Approx(123));
    REQUIRE_THROWS(node.toBoolean());

    REQUIRE(xml.getNext(&node)); // float
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == false);
    REQUIRE(node.getKey() == "float");
    REQUIRE(node.toString() == "+0.123e-1");
    REQUIRE(node.toInteger() == 0);
    REQUIRE(node.toFloat() == Approx(+0.123e-1));
    REQUIRE_THROWS(node.toBoolean());

    REQUIRE(xml.getNext(&node)); // boolean
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == false);
    REQUIRE(node.getKey() == "boolean");
    REQUIRE(node.toString() == "true");
    REQUIRE_THROWS(node.toInteger());
    REQUIRE_THROWS(node.toFloat());
    REQUIRE(node.toBoolean() == true);

    REQUIRE(xml.getNext(&node)); // null
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == false);
    REQUIRE(node.getKey() == "null");
    REQUIRE(node.toString() == "null");
    REQUIRE(node.isNull() == true);
    REQUIRE_THROWS(node.toInteger());
    REQUIRE_THROWS(node.toFloat());
    REQUIRE_THROWS(node.toBoolean());
}

TEST_CASE("Decode XML toArray ignore attributes", "ffw::XmlReader") {
    ffw::XmlReader xml(
        "<menu id=\"file\" value=\"\">"
            "<popup>"
                "<menuitem value=\"New\" onclick=\"CreateNewDoc()\">hello</menuitem>"
                "<menuitem value=\"Open\" onclick=\"OpenDoc()\">123</menuitem>"
                "<menuitem value=\"Close\" onclick=\"CloseDoc()\">false</menuitem>"
            "</popup>"
        "</menu>"
    );

    ffw::XmlIterator node, attr;

    REQUIRE(xml.getNext(&node)); // menu
    REQUIRE(xml.stepInto());
    REQUIRE(xml.getNext(&node)); // values
    REQUIRE(xml.stepInto());
    REQUIRE(xml.getNext(&node)); // first menuitem
    
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == true);

    ffw::Node::Array arr;
    REQUIRE_NOTHROW(arr = node.toArray(true));

    REQUIRE(arr.size() == 3);
    REQUIRE(arr[0].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[1].getType() == ffw::Node::Type::INTEGER);
    REQUIRE(arr[2].getType() == ffw::Node::Type::BOOLEAN);
    REQUIRE(arr[0] == std::string("hello"));
    REQUIRE(arr[1] == 123);
    REQUIRE(arr[2] == false);
}

TEST_CASE("Decode XML toArray", "ffw::XmlReader") {
    ffw::XmlReader xml(
        "<menu id=\"file\" value=\"\">"
        "<popup>"
        "<menuitem value=\"New\" onclick=\"CreateNewDoc()\">hello</menuitem>"
        "<menuitem value=\"Open\" onclick=\"OpenDoc()\">123</menuitem>"
        "<menuitem value=\"Close\" onclick=\"CloseDoc()\">false</menuitem>"
        "</popup>"
        "</menu>"
    );

    ffw::XmlIterator node, attr;

    REQUIRE(xml.getNext(&node)); // menu
    REQUIRE(xml.stepInto());
    REQUIRE(xml.getNext(&node)); // values
    REQUIRE(xml.stepInto());
    REQUIRE(xml.getNext(&node)); // first menuitem

    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == true);

    ffw::Node::Array arr;
    REQUIRE_NOTHROW(arr = node.toArray());

    REQUIRE(arr.size() == 3);
    REQUIRE(arr[0].getType() == ffw::Node::Type::OBJECT);
    REQUIRE(arr[1].getType() == ffw::Node::Type::OBJECT);
    REQUIRE(arr[2].getType() == ffw::Node::Type::OBJECT);

    REQUIRE(arr[0]["value"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[1]["value"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[2]["value"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[0]["value"] == std::string("New"));
    REQUIRE(arr[1]["value"] == std::string("Open"));
    REQUIRE(arr[2]["value"] == std::string("Close"));

    REQUIRE(arr[0]["onclick"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[1]["onclick"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[2]["onclick"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[0]["onclick"] == std::string("CreateNewDoc()"));
    REQUIRE(arr[1]["onclick"] == std::string("OpenDoc()"));
    REQUIRE(arr[2]["onclick"] == std::string("CloseDoc()"));

    REQUIRE(arr[0]["content"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[1]["content"].getType() == ffw::Node::Type::INTEGER);
    REQUIRE(arr[2]["content"].getType() == ffw::Node::Type::BOOLEAN);
    REQUIRE(arr[0]["content"] == std::string("hello"));
    REQUIRE(arr[1]["content"] == 123);
    REQUIRE(arr[2]["content"] == false);
}

TEST_CASE("Decode XML toArray empty nodes", "ffw::XmlReader") {
    ffw::XmlReader xml(
        "<menu id=\"file\" value=\"\">"
        "<popup>"
        "<menuitem value=\"New\" onclick=\"CreateNewDoc()\" />"
        "<menuitem value=\"Open\" onclick=\"OpenDoc()\" />"
        "<menuitem value=\"Close\" onclick=\"CloseDoc()\" />"
        "</popup>"
        "</menu>"
    );

    ffw::XmlIterator node, attr;

    REQUIRE(xml.getNext(&node)); // menu
    REQUIRE(xml.stepInto());
    REQUIRE(xml.getNext(&node)); // values
    REQUIRE(xml.stepInto());
    REQUIRE(xml.getNext(&node)); // first menuitem

    REQUIRE(node.getKey() == "menuitem");
    REQUIRE(node.hasAttributes() == true);
    REQUIRE(node.isEmpty() == true);
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == true);

    ffw::Node::Array arr;
    REQUIRE_NOTHROW(arr = node.toArray());

    REQUIRE(arr.size() == 3);
    REQUIRE(arr[0].getType() == ffw::Node::Type::OBJECT);
    REQUIRE(arr[1].getType() == ffw::Node::Type::OBJECT);
    REQUIRE(arr[2].getType() == ffw::Node::Type::OBJECT);

    REQUIRE(arr[0]["value"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[1]["value"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[2]["value"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[0]["value"] == std::string("New"));
    REQUIRE(arr[1]["value"] == std::string("Open"));
    REQUIRE(arr[2]["value"] == std::string("Close"));

    REQUIRE(arr[0]["onclick"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[1]["onclick"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[2]["onclick"].getType() == ffw::Node::Type::STRING);
    REQUIRE(arr[0]["onclick"] == std::string("CreateNewDoc()"));
    REQUIRE(arr[1]["onclick"] == std::string("OpenDoc()"));
    REQUIRE(arr[2]["onclick"] == std::string("CloseDoc()"));
}

TEST_CASE("Decode XML toObject ignore attributes", "ffw::XmlReader") {
    ffw::XmlReader xml(
        "<menu id=\"file\" value=\"\">"
        "<values>"
        "<integer attr=\"123\">123</integer>"
        "<float attr=\"+0.123e-1\">+0.123e-1</float>"
        "<boolean attr=\"true\">true</boolean>"
        "<null attr=\"null\">null</null>"
        "</values>"
        "</menu>"
    );

    ffw::XmlIterator node, attr;

    REQUIRE(xml.getNext(&node)); // menu
    REQUIRE(xml.stepInto());

    ffw::Node::Object obj;
    REQUIRE_NOTHROW(obj = node.toObject(true));

    REQUIRE(obj.size() == 1);

    REQUIRE(obj["values"].isObject());

    REQUIRE(obj["values"].getAsObject().size() == 4);
    REQUIRE(obj["values"]["integer"].getAsInt() == 123);
    REQUIRE(obj["values"]["float"].getAsFloat() == Approx(+0.123e-1));
    REQUIRE(obj["values"]["boolean"].getAsBool() == true);
    REQUIRE(obj["values"]["null"].isNull());
}

TEST_CASE("Decode XML toObject", "ffw::XmlReader") {
    ffw::XmlReader xml(
        "<menu id=\"file\" value=\"File\">"
        "<values>"
        "<integer attr=\"123\">123</integer>"
        "<float attr=\"+0.123e-1\">+0.123e-1</float>"
        "<boolean attr=\"true\">true</boolean>"
        "<null attr=\"null\">null</null>"
        "</values>"
        "</menu>"
    );

    ffw::XmlIterator node, attr;

    REQUIRE(xml.getNext(&node)); // menu
    REQUIRE(xml.stepInto());

    ffw::Node::Object obj;
    REQUIRE_NOTHROW(obj = node.toObject());

    REQUIRE(obj.size() == 3);
    REQUIRE(obj["id"].getAsString() == std::string("file"));
    REQUIRE(obj["value"].getAsString() == std::string("File"));
    REQUIRE(obj["values"].isObject() == true);

    REQUIRE(obj["values"].getAsObject().size() == 4);
    REQUIRE(obj["values"]["integer"].isObject() == true);
    REQUIRE(obj["values"]["float"].isObject() == true);
    REQUIRE(obj["values"]["boolean"].isObject() == true);
    REQUIRE(obj["values"]["null"].isObject() == true);

    REQUIRE(obj["values"]["integer"]["content"].getAsInt() == 123);
    REQUIRE(obj["values"]["float"]["content"].getAsFloat() == Approx(+0.123e-1));
    REQUIRE(obj["values"]["boolean"]["content"].getAsBool() == true);
    REQUIRE(obj["values"]["null"]["content"].isNull());

    REQUIRE(obj["values"]["integer"]["attr"].getAsInt() == 123);
    REQUIRE(obj["values"]["float"]["attr"].getAsFloat() == Approx(+0.123e-1));
    REQUIRE(obj["values"]["boolean"]["attr"].getAsBool() == true);
    REQUIRE(obj["values"]["null"]["attr"].isNull());
}

TEST_CASE("Decode XML #1", "ffw::XmlReader") {
    ffw::XmlReader xml(
        "<menu id=\"file\" value=\"File\">"
        "<popup>"
        "<menuitem value=\"New\" onclick=\"CreateNewDoc()\" />"
        "<menuitem value=\"Open\" onclick=\"OpenDoc()\" />"
        "<menuitem value=\"Close\" onclick=\"CloseDoc()\" />"
        "</popup>"
        "</menu>"
    );

    ffw::XmlIterator node, attr;
    REQUIRE(xml.getNext(&node)); // menu
    REQUIRE(xml.stepInto());
    ffw::Node::Object obj;
    REQUIRE_NOTHROW(obj = node.toObject());

    static ffw::Node::Object example = { 
        {"menu", ffw::Node::Object{
            {"id", "file",},
            {"value", "File",},
            {"popup", ffw::Node::Object{
                {"menuitem", ffw::Node::Array{
                    ffw::Node::Object{{"value", "New"}, {"onclick", "CreateNewDoc()"}},
                    ffw::Node::Object{{"value", "Open"}, {"onclick", "OpenDoc()"}},
                    ffw::Node::Object{{"value", "Close"}, {"onclick", "CloseDoc()"}}
                }},
            }},
        }}
    };

    REQUIRE_NOTHROW(compare(obj, example["menu"]));
}

TEST_CASE("Decode XML #2", "ffw::XmlReader") {
    ffw::XmlReader xml(
        "<widget>"
        "<debug>on</debug>"
        "<window title=\"Sample Konfabulator Widget\">"
            "<name>main_window</name>"
            "<width>500</width>"
            "<height>500</height>"
        "</window>"
        "<image src=\"Images/Sun.png\" name=\"sun1\">"
            "<hOffset>250</hOffset>"
            "<vOffset>250</vOffset>"
            "<alignment>center</alignment>"
        "</image>"
        "<text data=\"Click Here\" size=\"36\" style=\"bold\">"
            "<name>text1</name>"
            "<hOffset>250</hOffset>"
            "<vOffset>100</vOffset>"
            "<alignment>center</alignment>"
            "<onMouseUp>"
                "sun1.opacity = (sun1.opacity / 100) * 90;"
            "</onMouseUp>"
        "</text>"
        "</widget>"
    );

    ffw::XmlIterator node, attr;
    REQUIRE(xml.getNext(&node)); // menu
    REQUIRE(xml.stepInto());
    ffw::Node::Object obj;
    REQUIRE_NOTHROW(obj = node.toObject());

    static ffw::Node::Object example = {
        {"widget", ffw::Node::Object{
            {"debug", "on"},
            {"window", ffw::Node::Object{
                {"title", "Sample Konfabulator Widget"},
                {"name", "main_window"},
                {"width", 500},
                {"height", 500}
            }},
            {"image", ffw::Node::Object{
                {"src", "Images/Sun.png"},
                {"name", "sun1"},
                {"hOffset", 250},
                {"vOffset", 250},
                {"alignment", "center"}
            }},
            {"text", ffw::Node::Object{
                {"data", "Click Here"},
                {"size", 36},
                {"style", "bold"},
                {"name", "text1"},
                {"hOffset", 250},
                {"vOffset", 100},
                {"alignment", "center"},
                {"onMouseUp", "sun1.opacity = (sun1.opacity / 100) * 90;"}
            }},
        }},
    };

    REQUIRE_NOTHROW(compare(obj, example["widget"]));
}

TEST_CASE("Decode XML #3", "ffw::XmlReader") {
    ffw::XmlReader xml(
        "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"
        "<web-app>"
        "<servlet>"
        "<servlet-name>cofaxCDS</servlet-name>"
        "<servlet-class>org.cofax.cds.CDSServlet</servlet-class>"
        "<init-param>"
        "<configGlossary:installationAt>Philadelphia, PA</configGlossary:installationAt>"
        "<configGlossary:adminEmail>ksm@pobox.com</configGlossary:adminEmail>"
        "<configGlossary:poweredBy>Cofax</configGlossary:poweredBy>"
        "<configGlossary:poweredByIcon>/images/cofax.gif</configGlossary:poweredByIcon>"
        "<configGlossary:staticPath>/content/static</configGlossary:staticPath>"
        "<templateProcessorClass>org.cofax.WysiwygTemplate</templateProcessorClass>"
        "<templateLoaderClass>org.cofax.FilesTemplateLoader</templateLoaderClass>"
        "<templatePath>templates</templatePath>"
        "<templateOverridePath></templateOverridePath>"
        "<defaultListTemplate>listTemplate.htm</defaultListTemplate>"
        "<defaultFileTemplate>articleTemplate.htm</defaultFileTemplate>"
        "<useJSP>false</useJSP>"
        "<jspListTemplate>listTemplate.jsp</jspListTemplate>"
        "<jspFileTemplate>articleTemplate.jsp</jspFileTemplate>"
        "<cachePackageTagsTrack>200</cachePackageTagsTrack>"
        "<cachePackageTagsStore>200</cachePackageTagsStore>"
        "<cachePackageTagsRefresh>60</cachePackageTagsRefresh>"
        "<cacheTemplatesTrack>100</cacheTemplatesTrack>"
        "<cacheTemplatesStore>50</cacheTemplatesStore>"
        "<cacheTemplatesRefresh>15</cacheTemplatesRefresh>"
        "<cachePagesTrack>200</cachePagesTrack>"
        "<cachePagesStore>100</cachePagesStore>"
        "<cachePagesRefresh>10</cachePagesRefresh>"
        "<cachePagesDirtyRead>10</cachePagesDirtyRead>"
        "<searchEngineListTemplate>forSearchEnginesList.htm</searchEngineListTemplate>"
        "<searchEngineFileTemplate>forSearchEngines.htm</searchEngineFileTemplate>"
        "<searchEngineRobotsDb>WEB-INF/robots.db</searchEngineRobotsDb>"
        "<useDataStore>true</useDataStore>"
        "<dataStoreClass>org.cofax.SqlDataStore</dataStoreClass>"
        "<redirectionClass>org.cofax.SqlRedirection</redirectionClass>"
        "<dataStoreName>cofax</dataStoreName>"
        "<dataStoreDriver>com.microsoft.jdbc.sqlserver.SQLServerDriver</dataStoreDriver>"
        "<dataStoreUrl>jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon</dataStoreUrl>"
        "<dataStoreUser>sa</dataStoreUser>"
        "<dataStorePassword>dataStoreTestQuery</dataStorePassword>"
        "<dataStoreTestQuery>SET NOCOUNT ON;select test='test';</dataStoreTestQuery>"
        "<dataStoreLogFile>/usr/local/tomcat/logs/datastore.log</dataStoreLogFile>"
        "<dataStoreInitConns>10</dataStoreInitConns>"
        "<dataStoreMaxConns>100</dataStoreMaxConns>"
        "<dataStoreConnUsageLimit>100</dataStoreConnUsageLimit>"
        "<dataStoreLogLevel>debug</dataStoreLogLevel>"
        "<maxUrlLength>500</maxUrlLength>"
        "</init-param>"
        "</servlet>"
        "<servlet>"
        "<servlet-name>cofaxEmail</servlet-name>"
        "<servlet-class>org.cofax.cds.EmailServlet</servlet-class>"
        "<init-param>"
        "<mailHost>mail1</mailHost>"
        "<mailHostOverride>mail2</mailHostOverride>"
        "</init-param>"
        "</servlet>"
        "<servlet>"
        "<servlet-name>cofaxAdmin</servlet-name>"
        "<servlet-class>org.cofax.cds.AdminServlet</servlet-class>"
        "</servlet>"
        "<servlet>"
        "<servlet-name>fileServlet</servlet-name>"
        "<servlet-class>org.cofax.cds.FileServlet</servlet-class>"
        "</servlet>"
        "<servlet>"
        "<servlet-name>cofaxTools</servlet-name>"
        "<servlet-class>org.cofax.cms.CofaxToolsServlet</servlet-class>"
        "<init-param>"
        "<templatePath>toolstemplates/</templatePath>"
        "<log>1</log>"
        "<logLocation>/usr/local/tomcat/logs/CofaxTools.log</logLocation>"
        "<logMaxSize></logMaxSize>"
        "<dataLog>1</dataLog>"
        "<dataLogLocation>/usr/local/tomcat/logs/dataLog.log</dataLogLocation>"
        "<dataLogMaxSize></dataLogMaxSize>"
        "<removePageCache>/content/admin/remove?cache=pages&amp;id=</removePageCache>"
        "<removeTemplateCache>/content/admin/remove?cache=templates&amp;id=</removeTemplateCache>"
        "<fileTransferFolder>/usr/local/tomcat/webapps/content/fileTransferFolder</fileTransferFolder>"
        "<lookInContext>1</lookInContext>"
        "<adminGroupID>4</adminGroupID>"
        "<betaServer>true</betaServer>"
        "</init-param>"
        "</servlet>"
        "<servlet-mapping>"
        "<cofaxCDS>/</cofaxCDS>"
        "<cofaxEmail>/cofaxutil/aemail/*</cofaxEmail>"
        "<cofaxAdmin>/admin/*</cofaxAdmin>"
        "<fileServlet>/static/*</fileServlet>"
        "<cofaxTools>/tools/*</cofaxTools>"
        "</servlet-mapping>"
        "<taglib>"
        "<taglib-uri>cofax.tld</taglib-uri>"
        "<taglib-location>/WEB-INF/tlds/cofax.tld</taglib-location>"
        "</taglib>"
        "</web-app>"
    );

    ffw::XmlIterator node, attr;
    REQUIRE(xml.getNext(&node)); // menu
    ffw::Node::Object obj;
    REQUIRE_NOTHROW(obj = node.toObject());

    static ffw::Node::Object example = {
        {"web-app", ffw::Node::Object{
            {"servlet", ffw::Node::Array{
                ffw::Node::Object{
                    {"servlet-name", "cofaxCDS"},
                    {"servlet-class", "org.cofax.cds.CDSServlet"},
                    {"init-param", ffw::Node::Object{
                        {"configGlossary:installationAt", "Philadelphia, PA"},
                        {"configGlossary:adminEmail", "ksm@pobox.com"},
                        {"configGlossary:poweredBy", "Cofax"},
                        {"configGlossary:poweredByIcon", "/images/cofax.gif"},
                        {"configGlossary:staticPath", "/content/static"},
                        {"templateProcessorClass", "org.cofax.WysiwygTemplate"},
                        {"templateLoaderClass", "org.cofax.FilesTemplateLoader"},
                        {"templatePath", "templates"},
                        {"templateOverridePath", ""},
                        {"defaultListTemplate", "listTemplate.htm"},
                        {"defaultFileTemplate", "articleTemplate.htm"},
                        {"useJSP", false},
                        {"jspListTemplate", "listTemplate.jsp"},
                        {"jspFileTemplate", "articleTemplate.jsp"},
                        {"cachePackageTagsTrack", 200},
                        {"cachePackageTagsStore", 200},
                        {"cachePackageTagsRefresh", 60},
                        {"cacheTemplatesTrack", 100},
                        {"cacheTemplatesStore", 50},
                        {"cacheTemplatesRefresh", 15},
                        {"cachePagesTrack", 200},
                        {"cachePagesStore", 100},
                        {"cachePagesRefresh", 10},
                        {"cachePagesDirtyRead", 10},
                        {"searchEngineListTemplate", "forSearchEnginesList.htm"},
                        {"searchEngineFileTemplate", "forSearchEngines.htm"},
                        {"searchEngineRobotsDb", "WEB-INF/robots.db"},
                        {"useDataStore", true},
                        {"dataStoreClass", "org.cofax.SqlDataStore"},
                        {"redirectionClass", "org.cofax.SqlRedirection"},
                        {"dataStoreName", "cofax"},
                        {"dataStoreDriver", "com.microsoft.jdbc.sqlserver.SQLServerDriver"},
                        {"dataStoreUrl", "jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon"},
                        {"dataStoreUser", "sa"},
                        {"dataStorePassword", "dataStoreTestQuery"},
                        {"dataStoreTestQuery", "SET NOCOUNT ON;select test='test';"},
                        {"dataStoreLogFile", "/usr/local/tomcat/logs/datastore.log"},
                        {"dataStoreInitConns", 10},
                        {"dataStoreMaxConns", 100},
                        {"dataStoreConnUsageLimit", 100},
                        {"dataStoreLogLevel", "debug"},
                        {"maxUrlLength", 500}
                    }}
                },
                ffw::Node::Object{
                    {"servlet-name", "cofaxEmail"},
                    {"servlet-class", "org.cofax.cds.EmailServlet"},
                    {"init-param", ffw::Node::Object{
                        {"mailHost", "mail1"},
                        {"mailHostOverride", "mail2"}
                    }}
                },
                ffw::Node::Object{
                    {"servlet-name", "cofaxAdmin"},
                    {"servlet-class", "org.cofax.cds.AdminServlet"}
                },
                ffw::Node::Object{
                    {"servlet-name", "fileServlet"},
                    {"servlet-class", "org.cofax.cds.FileServlet"}
                },
                ffw::Node::Object{
                    {"servlet-name", "cofaxTools"},
                    {"servlet-class", "org.cofax.cms.CofaxToolsServlet"},
                    {"init-param", ffw::Node::Object{
                        {"templatePath", "toolstemplates/"},
                        {"log", 1},
                        {"logLocation", "/usr/local/tomcat/logs/CofaxTools.log"},
                        {"logMaxSize", ""},
                        {"dataLog", 1},
                        {"dataLogLocation", "/usr/local/tomcat/logs/dataLog.log"},
                        {"dataLogMaxSize", ""},
                        {"removePageCache", "/content/admin/remove?cache=pages&id="},
                        {"removeTemplateCache", "/content/admin/remove?cache=templates&id="},
                        {"fileTransferFolder", "/usr/local/tomcat/webapps/content/fileTransferFolder"},
                        {"lookInContext", 1},
                        {"adminGroupID", 4},
                        {"betaServer", true}
                    }}
                }
            }},
            {"servlet-mapping", ffw::Node::Object{
                {"cofaxCDS", "/"},
                {"cofaxEmail", "/cofaxutil/aemail/*"},
                {"cofaxAdmin", "/admin/*"},
                {"fileServlet", "/static/*"},
                {"cofaxTools", "/tools/*"}
            }},
            {"taglib", ffw::Node::Object{
                {"taglib-uri", "cofax.tld"},
                {"taglib-location", "/WEB-INF/tlds/cofax.tld"}
            }}
        }}
    };

    REQUIRE_NOTHROW(compare(obj, example["web-app"]));

    ffw::XmlWriter writer(true);
    writer.add("web-app", example["web-app"]);

    ffw::XmlReader tester(writer.str());

    REQUIRE(tester.getNext(&node)); // menu
    REQUIRE_NOTHROW(obj = node.toObject());

    REQUIRE_NOTHROW(compare(obj, example["web-app"]));
}

TEST_CASE("Decode XML escaped character #1", "ffw::XmlReader") {
    ffw::XmlReader xml(
        "<document>"
        "<basic>&#x0021;&#x0022;&#x0023;&#x0024;</basic>"
        "<greek>&#x0391;&#x0392;&#x0393;&#x0394;&#x0395;&#x0396;&#x0397;&#x0398;</greek>"
        "<cyrilic>&#x0410;&#x0411;&#x0412;&#x0413;&#x0414;&#x0415;&#x0416;&#x0417;</cyrilic>"
        "<arabic>&#x06AF;&#x06AE;&#x06AD;&#x06AC;&#x06AB;&#x06AA;</arabic>"
        "<oriya>&#x0B05;&#x0B06;&#x0B07;&#x0B08;&#x0B09;&#x0B0A;&#x0B0B;</oriya>"
        "<lao>&#x0E81;&#x0E82;&#x0E84;&#x0E87;&#x0E88;&#x0E8A;&#x0E8D;</lao>"
        "<math>&#x2200;&#x2201;&#x2202;&#x2203;&#x2204;&#x2205;&#x2206;&#x2207;</math>"
        "<cjk>&#xF900;&#xF901;&#xF902;&#xF903;&#xF904;&#xF905;&#xF906;</cjk>"
        "</document>"
    );

    ffw::XmlIterator node, attr;
    REQUIRE(xml.getNext(&node)); // menu

    ffw::Node::Object obj;
    REQUIRE_NOTHROW(obj = node.toObject());

    for (const auto& pair : obj) {
        std::cout << "pair: " << pair.first << " value type: " << (int)pair.second.getType() << std::endl;
    }

    // https://r12a.github.io/apps/conversion/ was used for conversion
    REQUIRE(obj["basic"].getAsString() == "\x21\x22\x23\x24");
    REQUIRE(obj["greek"].getAsString() == "\xCE\x91\xCE\x92\xCE\x93\xCE\x94\xCE\x95\xCE\x96\xCE\x97\xCE\x98");
    REQUIRE(obj["cyrilic"].getAsString() == "\xD0\x90\xD0\x91\xD0\x92\xD0\x93\xD0\x94\xD0\x95\xD0\x96\xD0\x97");
    REQUIRE(obj["arabic"].getAsString() == "\xDA\xAF\xDA\xAE\xDA\xAD\xDA\xAC\xDA\xAB\xDA\xAA");
    REQUIRE(obj["oriya"].getAsString() == "\xE0\xAC\x85\xE0\xAC\x86\xE0\xAC\x87\xE0\xAC\x88\xE0\xAC\x89\xE0\xAC\x8A\xE0\xAC\x8B");
    REQUIRE(obj["lao"].getAsString() == "\xE0\xBA\x81\xE0\xBA\x82\xE0\xBA\x84\xE0\xBA\x87\xE0\xBA\x88\xE0\xBA\x8A\xE0\xBA\x8D");
    REQUIRE(obj["math"].getAsString() == "\xE2\x88\x80\xE2\x88\x81\xE2\x88\x82\xE2\x88\x83\xE2\x88\x84\xE2\x88\x85\xE2\x88\x86\xE2\x88\x87");
    REQUIRE(obj["cjk"].getAsString() == "\xEF\xA4\x80\xEF\xA4\x81\xEF\xA4\x82\xEF\xA4\x83\xEF\xA4\x84\xEF\xA4\x85\xEF\xA4\x86");
}

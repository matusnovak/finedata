#include <ffw/data/jsonreader.h>
#include <ffw/data/jsonwriter.h>

#define CATCH_CONFIG_MAIN
#include "./catch.hpp"
#include "compare.h"

TEST_CASE("Decode XML types", "ffw::JsonReader") {
    ffw::JsonReader json(
        "{"
        "    \"string\": \"Hello World!\","
        "    \"integer\": -123,"
        "    \"float\": 0.123,"
        "    \"null\": null,"
        "    \"object\": {\"first\": 10, \"second\": 20},"
        "    \"array\": [10, 20],"
        "    \"boolean\": true"
        "}"
    );

    ffw::JsonIterator node;
    REQUIRE(!node);

    REQUIRE(json.getNext(&node));
    REQUIRE(node);
    REQUIRE(node.getKey() == "");
    REQUIRE(json.getNext(&node) == false);
    REQUIRE(json.stepInto());

    CHECK(node.isObject() == true);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == false);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);

    // String
    REQUIRE(json.getNext(&node) == true);
    REQUIRE(node.getKey() == "string");
    
    REQUIRE(node.isEmpty() == false);
    REQUIRE(node.isString() == true);
    REQUIRE(node.isNumber() == false);
    REQUIRE(node.isInteger() == false);
    REQUIRE(node.isFloat() == false);
    REQUIRE(node.isBoolean() == false);
    REQUIRE(node.isNull() == false);
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == false);

    REQUIRE_THROWS(node.toInteger());
    REQUIRE_THROWS(node.toFloat());
    REQUIRE_THROWS(node.toObject());
    REQUIRE_THROWS(node.toArray());
    REQUIRE_THROWS(node.toBoolean());
    REQUIRE_NOTHROW(node.toString());

    REQUIRE(node.toString() == "Hello World!");

    // integer
    REQUIRE(json.getNext(&node) == true);
    REQUIRE(node.getKey() == "integer");

    REQUIRE(node.isEmpty() == false);
    REQUIRE(node.isString() == false);
    REQUIRE(node.isNumber() == true);
    REQUIRE(node.isInteger() == true);
    REQUIRE(node.isFloat() == false);
    REQUIRE(node.isBoolean() == false);
    REQUIRE(node.isNull() == false);
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == false);

    REQUIRE_NOTHROW(node.toInteger());
    REQUIRE_THROWS(node.toFloat());
    REQUIRE_THROWS(node.toObject());
    REQUIRE_THROWS(node.toArray());
    REQUIRE_THROWS(node.toBoolean());
    REQUIRE_THROWS(node.toString());

    REQUIRE(node.toInteger() == -123);

    // float
    REQUIRE(json.getNext(&node) == true);
    REQUIRE(node.getKey() == "float");

    REQUIRE(node.isEmpty() == false);
    REQUIRE(node.isString() == false);
    REQUIRE(node.isNumber() == true);
    REQUIRE(node.isInteger() == false);
    REQUIRE(node.isFloat() == true);
    REQUIRE(node.isBoolean() == false);
    REQUIRE(node.isNull() == false);
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == false);

    REQUIRE_THROWS(node.toInteger());
    REQUIRE_NOTHROW(node.toFloat());
    REQUIRE_THROWS(node.toObject());
    REQUIRE_THROWS(node.toArray());
    REQUIRE_THROWS(node.toBoolean());
    REQUIRE_THROWS(node.toString());

    REQUIRE(node.toFloat() == Approx(0.123));

    // null
    REQUIRE(json.getNext(&node) == true);
    REQUIRE(node.getKey() == "null");

    REQUIRE(node.isEmpty() == false);
    REQUIRE(node.isString() == false);
    REQUIRE(node.isNumber() == false);
    REQUIRE(node.isInteger() == false);
    REQUIRE(node.isFloat() == false);
    REQUIRE(node.isBoolean() == false);
    REQUIRE(node.isNull() == true);
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == false);

    REQUIRE_THROWS(node.toInteger());
    REQUIRE_THROWS(node.toFloat());
    REQUIRE_THROWS(node.toObject());
    REQUIRE_THROWS(node.toArray());
    REQUIRE_THROWS(node.toBoolean());
    REQUIRE_THROWS(node.toString());

    // object
    REQUIRE(json.getNext(&node) == true);
    REQUIRE(node.getKey() == "object");

    REQUIRE(node.isEmpty() == false);
    REQUIRE(node.isString() == false);
    REQUIRE(node.isNumber() == false);
    REQUIRE(node.isInteger() == false);
    REQUIRE(node.isFloat() == false);
    REQUIRE(node.isBoolean() == false);
    REQUIRE(node.isNull() == false);
    REQUIRE(node.isObject() == true);
    REQUIRE(node.isArray() == false);

    REQUIRE_THROWS(node.toInteger());
    REQUIRE_THROWS(node.toFloat());
    REQUIRE_NOTHROW(node.toObject());
    REQUIRE_THROWS(node.toArray());
    REQUIRE_THROWS(node.toBoolean());
    REQUIRE_THROWS(node.toString());

    // Object -> first,second
    {
        REQUIRE(json.stepInto());

        // First
        REQUIRE(json.getNext(&node) == true);
        REQUIRE(node.getKey() == "first");
        REQUIRE(node.isInteger() == true);
        REQUIRE(node.toInteger() == 10);

        // Second
        REQUIRE(json.getNext(&node) == true);
        REQUIRE(node.getKey() == "second");
        REQUIRE(node.isInteger() == true);
        REQUIRE(node.toInteger() == 20);

        REQUIRE(json.stepOut());
    }

    // array
    REQUIRE(json.getNext(&node) == true);
    REQUIRE(node.getKey() == "array");

    REQUIRE(node.isEmpty() == false);
    REQUIRE(node.isString() == false);
    REQUIRE(node.isNumber() == false);
    REQUIRE(node.isInteger() == false);
    REQUIRE(node.isFloat() == false);
    REQUIRE(node.isBoolean() == false);
    REQUIRE(node.isNull() == false);
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == true);

    REQUIRE_THROWS(node.toInteger());
    REQUIRE_THROWS(node.toFloat());
    REQUIRE_THROWS(node.toObject());
    REQUIRE_NOTHROW(node.toArray());
    REQUIRE_THROWS(node.toBoolean());
    REQUIRE_THROWS(node.toString());

    // Array -> 0,1
    {
        REQUIRE(json.stepInto());

        // 0
        REQUIRE(json.getNext(&node) == true);
        REQUIRE(node.isInteger() == true);
        REQUIRE(node.toInteger() == 10);

        // 1
        REQUIRE(json.getNext(&node) == true);
        REQUIRE(node.isInteger() == true);
        REQUIRE(node.toInteger() == 20);

        REQUIRE(json.stepOut());
    }

    // boolean
    REQUIRE(json.getNext(&node) == true);
    REQUIRE(node.getKey() == "boolean");

    REQUIRE(node.isEmpty() == false);
    REQUIRE(node.isString() == false);
    REQUIRE(node.isNumber() == false);
    REQUIRE(node.isInteger() == false);
    REQUIRE(node.isFloat() == false);
    REQUIRE(node.isBoolean() == true);
    REQUIRE(node.isNull() == false);
    REQUIRE(node.isObject() == false);
    REQUIRE(node.isArray() == false);

    REQUIRE_THROWS(node.toInteger());
    REQUIRE_THROWS(node.toFloat());
    REQUIRE_THROWS(node.toObject());
    REQUIRE_THROWS(node.toArray());
    REQUIRE_NOTHROW(node.toBoolean());
    REQUIRE_THROWS(node.toString());

    REQUIRE(node.toBoolean() == true);

    // no next
    REQUIRE(json.getNext(&node) == false);
}

TEST_CASE("Decode JSON #1", "ffw::JsonReader") {
    ffw::JsonReader json (
        "{"
        "    \"string\": \"Hello World!\","
        "    \"integer\": 123,"
        "    \"float\": -0.123,"
        "    \"boolean\": true,"
        "    \"null\": null,"
        "    \"object\": {\"first\": 10, \"second\": 20},"
        "    \"array\": [10, 20]"
        "}"
    );

    ffw::JsonIterator node;
    ffw::Node obj;
    REQUIRE(!node);

    REQUIRE(json.getNext(&node));
    REQUIRE(node);
    REQUIRE(node.getKey() == "");
    REQUIRE(node.isObject() == true);
    REQUIRE_NOTHROW(obj = node.get());

    REQUIRE(obj.getAsObject().size() == 7);
    REQUIRE(obj["string"].getAsString() == "Hello World!");
    REQUIRE(obj["integer"].getAsInt() == 123);
    REQUIRE(obj["float"].getAsFloat() == Approx(-0.123));
    REQUIRE(obj["boolean"].getAsBool() == true);
    REQUIRE(obj["null"].isNull() == true);
    REQUIRE(obj["object"].getAsObject().size() == 2);
    REQUIRE(obj["object"]["first"].getAsInt() == 10);
    REQUIRE(obj["object"]["second"].getAsInt() == 20);
    REQUIRE(obj["array"].getAsArray().size() == 2);
    REQUIRE(obj["array"][0].getAsInt() == 10);
    REQUIRE(obj["array"][1].getAsInt() == 20);
}

TEST_CASE("Decode and encode JSON #1", "ffw::JsonReader") {
    ffw::JsonReader json(
        "{\"menu\": {"
        "  \"id\": \"file\","
        "  \"value\": \"File\","
        "  \"popup\": {"
        "	\"menuitem\": ["
        "	  {\"value\": \"New\", \"onclick\": \"CreateNewDoc()\"},"
        "	  {\"value\": \"Open\", \"onclick\": \"OpenDoc()\"},"
        "	  {\"value\": \"Close\", \"onclick\": \"CloseDoc()\"}"
        "	]"
        "  }"
        "}}"
    );

    static ffw::Node::Object example = {{
        "menu", ffw::Node::Object{
            {"id", "file",},
            {"value", "File",},
            {"popup", ffw::Node::Object{
                {"menuitem", ffw::Node::Array{
                    ffw::Node::Object{{"value", "New"}, {"onclick", "CreateNewDoc()"}},
                    ffw::Node::Object{{"value", "Open"}, {"onclick", "OpenDoc()"}},
                    ffw::Node::Object{{"value", "Close"}, {"onclick", "CloseDoc()"}}
                }},
            }},
        }
    }};

    ffw::JsonIterator node;
    ffw::Node obj;
    REQUIRE(!node);

    REQUIRE(json.getNext(&node));
    REQUIRE(node);
    REQUIRE(node.getKey() == "");
    REQUIRE(node.isObject() == true);
    REQUIRE_NOTHROW(obj = node.get());

    REQUIRE_NOTHROW(compare(obj, example));

    ffw::JsonWriter writer;
    writer.add(example);

    ffw::JsonReader tester(writer.str());

    REQUIRE(tester.getNext(&node)); // menu
    REQUIRE_NOTHROW(obj = node.toObject());

    REQUIRE_NOTHROW(compare(obj, example));
}

TEST_CASE("Decode and encode JSON #2", "ffw::JsonReader") {
    ffw::JsonReader json(
        "{\"widget\": {\n"
        "	\"debug\": \"on\",\n"
        "	\"window\": {\n"
        "		\"title\": \"Sample Konfabulator Widget\",\n"
        "		\"name\": \"main_window\",\n"
        "		\"width\": 500,\n"
        "		\"height\": 500\n"
        "	},\n"
        "	\"image\": { \n"
        "		\"src\": \"Images/Sun.png\",\n"
        "		\"name\": \"sun1\",\n"
        "		\"hOffset\": 250,\n"
        "		\"vOffset\": 250,\n"
        "		\"alignment\": \"center\"\n"
        "	},\n"
        "	\"text\": {\n"
        "		\"data\": \"Click Here\",\n"
        "		\"size\": 36,\n"
        "		\"style\": \"bold\",\n"
        "		\"name\": \"text1\",\n"
        "		\"hOffset\": 250,\n"
        "		\"vOffset\": 100,\n"
        "		\"alignment\": \"center\",\n"
        "		\"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"\n"
        "	}\n"
        "}}  \n"
    );

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

    ffw::JsonIterator node;
    ffw::Node obj;
    REQUIRE(!node);

    REQUIRE(json.getNext(&node));
    REQUIRE(node);
    REQUIRE(node.getKey() == "");
    REQUIRE(node.isObject() == true);
    REQUIRE_NOTHROW(obj = node.get());

    REQUIRE_NOTHROW(compare(obj, example));

    ffw::JsonWriter writer;
    writer.add(example);

    ffw::JsonReader tester(writer.str());

    REQUIRE(tester.getNext(&node)); // menu
    REQUIRE_NOTHROW(obj = node.toObject());

    REQUIRE_NOTHROW(compare(obj, example));
}

TEST_CASE("Decode and encode JSON #3", "ffw::JsonReader") {
    ffw::JsonReader json(
        "{\"web-app\": {\r\n"
        "  \"servlet\": [   \r\n"
        "    {\r\n"
        "      \"servlet-name\": \"cofaxCDS\",\r\n"
        "      \"servlet-class\": \"org.cofax.cds.CDSServlet\",\r\n"
        "      \"init-param\": {\r\n"
        "        \"configGlossary:installationAt\": \"Philadelphia, PA\",\r\n"
        "        \"configGlossary:adminEmail\": \"ksm@pobox.com\",\r\n"
        "        \"configGlossary:poweredBy\": \"Cofax\",\r\n"
        "        \"configGlossary:poweredByIcon\": \"/images/cofax.gif\",\r\n"
        "        \"configGlossary:staticPath\": \"/content/static\",\r\n"
        "        \"templateProcessorClass\": \"org.cofax.WysiwygTemplate\",\r\n"
        "        \"templateLoaderClass\": \"org.cofax.FilesTemplateLoader\",\r\n"
        "        \"templatePath\": \"templates\",\r\n"
        "        \"templateOverridePath\": \"\",\r\n"
        "        \"defaultListTemplate\": \"listTemplate.htm\",\r\n"
        "        \"defaultFileTemplate\": \"articleTemplate.htm\",\r\n"
        "        \"useJSP\": false,\r\n"
        "        \"jspListTemplate\": \"listTemplate.jsp\",\r\n"
        "        \"jspFileTemplate\": \"articleTemplate.jsp\",\r\n"
        "        \"cachePackageTagsTrack\": 200,\r\n"
        "        \"cachePackageTagsStore\": 200,\r\n"
        "        \"cachePackageTagsRefresh\": 60,\r\n"
        "        \"cacheTemplatesTrack\": 100,\r\n"
        "        \"cacheTemplatesStore\": 50,\r\n"
        "        \"cacheTemplatesRefresh\": 15,\r\n"
        "        \"cachePagesTrack\": 200,\r\n"
        "        \"cachePagesStore\": 100,\r\n"
        "        \"cachePagesRefresh\": 10,\r\n"
        "        \"cachePagesDirtyRead\": 10,\r\n"
        "        \"searchEngineListTemplate\": \"forSearchEnginesList.htm\",\r\n"
        "        \"searchEngineFileTemplate\": \"forSearchEngines.htm\",\r\n"
        "        \"searchEngineRobotsDb\": \"WEB-INF/robots.db\",\r\n"
        "        \"useDataStore\": true,\r\n"
        "        \"dataStoreClass\": \"org.cofax.SqlDataStore\",\r\n"
        "        \"redirectionClass\": \"org.cofax.SqlRedirection\",\r\n"
        "        \"dataStoreName\": \"cofax\",\r\n"
        "        \"dataStoreDriver\": \"com.microsoft.jdbc.sqlserver.SQLServerDriver\",\r\n"
        "        \"dataStoreUrl\": \"jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon\",\r\n"
        "        \"dataStoreUser\": \"sa\",\r\n"
        "        \"dataStorePassword\": \"dataStoreTestQuery\",\r\n"
        "        \"dataStoreTestQuery\": \"SET NOCOUNT ON;select test='test';\",\r\n"
        "        \"dataStoreLogFile\": \"/usr/local/tomcat/logs/datastore.log\",\r\n"
        "        \"dataStoreInitConns\": 10,\r\n"
        "        \"dataStoreMaxConns\": 100,\r\n"
        "        \"dataStoreConnUsageLimit\": 100,\r\n"
        "        \"dataStoreLogLevel\": \"debug\",\r\n"
        "        \"maxUrlLength\": 500}},\r\n"
        "    {\r\n"
        "      \"servlet-name\": \"cofaxEmail\",\r\n"
        "      \"servlet-class\": \"org.cofax.cds.EmailServlet\",\r\n"
        "      \"init-param\": {\r\n"
        "      \"mailHost\": \"mail1\",\r\n"
        "      \"mailHostOverride\": \"mail2\"}},\r\n"
        "    {\r\n"
        "      \"servlet-name\": \"cofaxAdmin\",\r\n"
        "      \"servlet-class\": \"org.cofax.cds.AdminServlet\"},\r\n"
        " \r\n"
        "    {\r\n"
        "      \"servlet-name\": \"fileServlet\",\r\n"
        "      \"servlet-class\": \"org.cofax.cds.FileServlet\"},\r\n"
        "    {\r\n"
        "      \"servlet-name\": \"cofaxTools\",\r\n"
        "      \"servlet-class\": \"org.cofax.cms.CofaxToolsServlet\",\r\n"
        "      \"init-param\": {\r\n"
        "        \"templatePath\": \"toolstemplates/\",\r\n"
        "        \"log\": 1,\r\n"
        "        \"logLocation\": \"/usr/local/tomcat/logs/CofaxTools.log\",\r\n"
        "        \"logMaxSize\": \"\",\r\n"
        "        \"dataLog\": 1,\r\n"
        "        \"dataLogLocation\": \"/usr/local/tomcat/logs/dataLog.log\",\r\n"
        "        \"dataLogMaxSize\": \"\",\r\n"
        "        \"removePageCache\": \"/content/admin/remove?cache=pages&id=\",\r\n"
        "        \"removeTemplateCache\": \"/content/admin/remove?cache=templates&id=\",\r\n"
        "        \"fileTransferFolder\": \"/usr/local/tomcat/webapps/content/fileTransferFolder\",\r\n"
        "        \"lookInContext\": 1,\r\n"
        "        \"adminGroupID\": 4,\r\n"
        "        \"betaServer\": true}}],\r\n"
        "  \"servlet-mapping\": {\r\n"
        "    \"cofaxCDS\": \"/\",\r\n"
        "    \"cofaxEmail\": \"/cofaxutil/aemail/*\",\r\n"
        "    \"cofaxAdmin\": \"/admin/*\",\r\n"
        "    \"fileServlet\": \"/static/*\",\r\n"
        "    \"cofaxTools\": \"/tools/*\"},\r\n"
        " \r\n"
        "  \"taglib\": {\r\n"
        "    \"taglib-uri\": \"cofax.tld\",\r\n"
        "    \"taglib-location\": \"/WEB-INF/tlds/cofax.tld\"}}}\r\n"
    );

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

    ffw::JsonIterator node;
    ffw::Node obj;
    REQUIRE(!node);

    REQUIRE(json.getNext(&node));
    REQUIRE(node);
    REQUIRE(node.getKey() == "");
    REQUIRE(node.isObject() == true);
    REQUIRE_NOTHROW(obj = node.get());

    REQUIRE_NOTHROW(compare(obj, example));

    ffw::JsonWriter writer;
    writer.add(example);

    ffw::JsonReader tester(writer.str());

    REQUIRE(tester.getNext(&node)); // menu
    REQUIRE_NOTHROW(obj = node.toObject());

    REQUIRE_NOTHROW(compare(obj, example));
}

TEST_CASE("Decode JSON escaped character #1", "ffw::JsonReader") {
    ffw::JsonReader json(
        "{"
        "\"basic\": \"\\u0021\\u0022\\u0023\\u0024\","
        "\"greek\": \"\\u0391\\u0392\\u0393\\u0394\\u0395\\u0396\\u0397\\u0398\","
        "\"cyrilic\": \"\\u0410\\u0411\\u0412\\u0413\\u0414\\u0415\\u0416\\u0417\","
        "\"arabic\": \"\\u06AF\\u06AE\\u06AD\\u06AC\\u06AB\\u06AA\","
        "\"oriya\": \"\\u0B05\\u0B06\\u0B07\\u0B08\\u0B09\\u0B0A\\u0B0B\","
        "\"lao\": \"\\u0E81\\u0E82\\u0E84\\u0E87\\u0E88\\u0E8A\\u0E8D\","
        "\"math\": \"\\u2200\\u2201\\u2202\\u2203\\u2204\\u2205\\u2206\\u2207\","
        "\"cjk\": \"\\uF900\\uF901\\uF902\\uF903\\uF904\\uF905\\uF906\""
        "}"
    );

    ffw::JsonIterator node;
    ffw::Node obj;
    REQUIRE(!node);

    REQUIRE(json.getNext(&node));
    REQUIRE(node);
    REQUIRE(node.getKey() == "");
    REQUIRE(node.isObject() == true);
    REQUIRE_NOTHROW(obj = node.get());

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

TEST_CASE("Decode and encode JSON quick and dirty", "ffw::JsonReader") {
    const auto obj = ffw::decodeJson(
        "{\"menu\": {"
        "  \"id\": \"file\","
        "  \"value\": \"File\","
        "  \"popup\": {"
        "	\"menuitem\": ["
        "	  {\"value\": \"New\", \"onclick\": \"CreateNewDoc()\"},"
        "	  {\"value\": \"Open\", \"onclick\": \"OpenDoc()\"},"
        "	  {\"value\": \"Close\", \"onclick\": \"CloseDoc()\"}"
        "	]"
        "  }"
        "}}"
    );

    static ffw::Node::Object example = {{
        "menu", ffw::Node::Object{
            {"id", "file",},
            {"value", "File",},
            {"popup", ffw::Node::Object{
                {"menuitem", ffw::Node::Array{
                    ffw::Node::Object{{"value", "New"}, {"onclick", "CreateNewDoc()"}},
                    ffw::Node::Object{{"value", "Open"}, {"onclick", "OpenDoc()"}},
                    ffw::Node::Object{{"value", "Close"}, {"onclick", "CloseDoc()"}}
                }},
            }},
        }
    }};

    REQUIRE_NOTHROW(compare(obj, example));

    const auto testStr = ffw::encodeJson(example);
    const auto obj2 = ffw::decodeJson(testStr);

    REQUIRE_NOTHROW(compare(obj2, example));
}

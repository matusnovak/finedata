#include <ffw/data/yamlreader.h>
#include <ffw/data/yamlwriter.h>

#define CATCH_CONFIG_MAIN
#include "./catch.hpp"
#include "compare.h"

TEST_CASE("Decode YAML types", "ffw::YamlReader") {
    ffw::YamlReader yaml(
        "menu:\n"
        "  id: 'file'\n"
        "  value: ''\n"
        "  popup:\n"
        "     menuitem:\n"
        "       - value: 'New'\n"
        "         onclick: 'CreateNewDoc()'\n"
        "       - value: 'Open'\n"
        "         onclick: 'OpenDoc()'\n"
        "       - value: 'Close'\n"
        "         onclick: 'CloseDoc()'\n"
        "  values:\n"
        "    integer: 123\n"
        "    float: 0.123\n"
        "    boolean: true\n"
        "    nullval: null\n"
        ""
    );

    ffw::YamlIterator node;
    REQUIRE(!node);

    REQUIRE(yaml.getNext(&node));
    REQUIRE(node);
    REQUIRE(node.getKey() == "menu");

    CHECK(node.isObject() == true);
    CHECK(node.isArray() == false);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == false);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);

    REQUIRE(yaml.stepInto() == true);
    REQUIRE(yaml.getNext(&node));
    REQUIRE(node.getKey() == "id");

    CHECK(node.isObject() == false);
    CHECK(node.isArray() == false);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == true);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);
    REQUIRE(node.toString() == "file");

    REQUIRE(yaml.getNext(&node));
    REQUIRE(node.getKey() == "value");

    CHECK(node.isObject() == false);
    CHECK(node.isArray() == false);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == true);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);
    REQUIRE(node.toString() == "");

    REQUIRE(yaml.getNext(&node));
    REQUIRE(node.getKey() == "popup");

    CHECK(node.isObject() == true);
    CHECK(node.isArray() == false);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == false);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);

    REQUIRE(yaml.stepInto() == true);
    REQUIRE(yaml.getNext(&node));
    REQUIRE(node.getKey() == "menuitem");

    CHECK(node.isObject() == false);
    CHECK(node.isArray() == true);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == false);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);

    REQUIRE(yaml.stepInto() == true);
    REQUIRE(yaml.getNext(&node));
    REQUIRE(node.getKey() == "");

    CHECK(node.isObject() == true);
    CHECK(node.isArray() == false);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == false);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);

    REQUIRE(yaml.stepInto() == true);
    REQUIRE(yaml.getNext(&node));
    REQUIRE(node.getKey() == "value");

    CHECK(node.isObject() == false);
    CHECK(node.isArray() == false);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == true);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);
    REQUIRE(node.toString() == "New");

    REQUIRE(yaml.getNext(&node));
    REQUIRE(node.getKey() == "onclick");

    CHECK(node.isObject() == false);
    CHECK(node.isArray() == false);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == true);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);
    REQUIRE(node.toString() == "CreateNewDoc()");

    REQUIRE(yaml.getNext(&node) == false);

    REQUIRE(yaml.stepOut()); // step out into array (into menuitem)
    REQUIRE(yaml.getNext(&node) == true); // second
    REQUIRE(yaml.getNext(&node) == true); // third
    REQUIRE(yaml.getNext(&node) == false); // no more elements in array

    REQUIRE(yaml.stepOut()); // step out into popup
    REQUIRE(yaml.getNext(&node) == false); // no more elements in popup
    REQUIRE(yaml.stepOut()); // step out into menu
    REQUIRE(yaml.getNext(&node) == true); // we have more elements

    REQUIRE(node.getKey() == "values");

    CHECK(node.isObject() == true);
    CHECK(node.isArray() == false);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == false);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);

    REQUIRE(yaml.stepInto() == true);
    REQUIRE(yaml.getNext(&node));
    REQUIRE(node.getKey() == "integer");

    CHECK(node.isObject() == false);
    CHECK(node.isArray() == false);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == true);
    CHECK(node.isNumber() == true);
    CHECK(node.isInteger() == true);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);
    REQUIRE(node.toInteger() == 123);
    REQUIRE(node.toString() == "123");
    REQUIRE_THROWS(node.toFloat());
    REQUIRE_THROWS(node.toBoolean());
    REQUIRE_THROWS(node.toObject());
    REQUIRE_THROWS(node.toArray());

    REQUIRE(yaml.getNext(&node));
    REQUIRE(node.getKey() == "float");

    CHECK(node.isObject() == false);
    CHECK(node.isArray() == false);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == true);
    CHECK(node.isNumber() == true);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == true);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == false);
    REQUIRE_THROWS(node.toInteger());
    REQUIRE(node.toString() == "0.123");
    REQUIRE(node.toFloat() == Approx(0.123));
    REQUIRE_THROWS(node.toBoolean());
    REQUIRE_THROWS(node.toObject());
    REQUIRE_THROWS(node.toArray());

    REQUIRE(yaml.getNext(&node));
    REQUIRE(node.getKey() == "boolean");

    CHECK(node.isObject() == false);
    CHECK(node.isArray() == false);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == true);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == true);
    CHECK(node.isNull() == false);
    REQUIRE_THROWS(node.toInteger());
    REQUIRE(node.toString() == "true");
    REQUIRE_THROWS(node.toFloat());
    REQUIRE(node.toBoolean() == true);
    REQUIRE_THROWS(node.toObject());
    REQUIRE_THROWS(node.toArray());

    REQUIRE(yaml.getNext(&node));
    REQUIRE(node.getKey() == "nullval");

    CHECK(node.isObject() == false);
    CHECK(node.isArray() == false);
    CHECK(node.isEmpty() == false);
    CHECK(node.isString() == false);
    CHECK(node.isNumber() == false);
    CHECK(node.isInteger() == false);
    CHECK(node.isFloat() == false);
    CHECK(node.isBoolean() == false);
    CHECK(node.isNull() == true);
    REQUIRE_THROWS(node.toInteger());
    REQUIRE_THROWS(node.toString());
    REQUIRE_THROWS(node.toFloat());
    REQUIRE_THROWS(node.toBoolean());
    REQUIRE_THROWS(node.toObject());
    REQUIRE_THROWS(node.toArray());

    REQUIRE(yaml.getNext(&node) == false);
    REQUIRE(yaml.stepOut()); // step out into menu
    REQUIRE(yaml.getNext(&node) == false);
    REQUIRE(yaml.stepOut()); // step out into root
    REQUIRE(yaml.getNext(&node) == false);
    REQUIRE(yaml.stepOut() == false); // nothing more
}

TEST_CASE("Decode and encode YAML #1", "ffw::YamlReader") {
    ffw::YamlReader yaml(
        "menu:\n"
        "  # Some comment\n"
        "  id: 'file'\n"
        "  value: 'File'\n"
        "  popup:\n"
        "     menuitem:\n"
        "       - value: 'New'\n"
        "         onclick: 'CreateNewDoc()'\n"
        "       - value: 'Open'\n"
        "         onclick: 'OpenDoc()'\n"
        "       - value: 'Close'\n"
        "         onclick: 'CloseDoc()'\n"
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

    ffw::YamlIterator node;
    ffw::Node obj;
    REQUIRE(!node);

    REQUIRE(yaml.getNext(&node));
    REQUIRE(node);
    REQUIRE(node.getKey() == "menu");
    REQUIRE(node.isObject() == true);
    REQUIRE_NOTHROW(obj = node.get());

    REQUIRE_NOTHROW(compare(obj, example["menu"]));

    ffw::YamlWriter writer;
    writer.add(example);

    ffw::YamlReader tester(writer.str());

    REQUIRE(tester.getNext(&node)); // menu
    REQUIRE_NOTHROW(obj = node.toObject());

    REQUIRE_NOTHROW(compare(obj, example["menu"]));
}


TEST_CASE("Decode and encode YAML #2", "ffw::YamlReader") {
    ffw::YamlReader yaml(
        "widget:\n"
        "  debug: 'on'\n"
        "  window:\n"
        "    title: 'Sample Konfabulator Widget'\n"
        "    name: 'main_window'\n"
        "    width: '500'\n"
        "    height: '500'\n"
        "  image:\n"
        "    src: 'Images/Sun.png'\n"
        "    name: 'sun1'\n"
        "    hOffset: 250\n"
        "    vOffset: 250\n"
        "    alignment: 'center'\n"
        "  text:\n"
        "    data: 'Click Here'\n"
        "    size: 36\n"
        "    style: 'bold'\n"
        "    name: 'text1'\n"
        "    hOffset: 250\n"
        "    vOffset: 100\n"
        "    alignment: 'center'\n"
        "    onMouseUp: 'sun1.opacity = (sun1.opacity / 100) * 90;'\n"
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

    ffw::YamlIterator node;
    ffw::Node obj;
    REQUIRE(!node);

    REQUIRE(yaml.getNext(&node));
    REQUIRE(node);
    REQUIRE(node.getKey() == "widget");
    REQUIRE(node.isObject() == true);
    REQUIRE_NOTHROW(obj = node.get());

    REQUIRE_NOTHROW(compare(obj, example["widget"]));

    ffw::YamlWriter writer;
    writer.add(example);

    ffw::YamlReader tester(writer.str());

    REQUIRE(tester.getNext(&node));
    REQUIRE_NOTHROW(obj = node.toObject());

    REQUIRE_NOTHROW(compare(obj, example["widget"]));
}

TEST_CASE("Decode and encode YAML #3", "ffw::YamlReader") {
    ffw::YamlReader json(
        "web-app:\n"
        "  servlet:\n"
        "  - servlet-name: cofaxCDS\n"
        "    servlet-class: org.cofax.cds.CDSServlet\n"
        "    init-param:\n"
        "      configGlossary:installationAt: Philadelphia, PA\n"
        "      configGlossary:adminEmail: ksm@pobox.com\n"
        "      configGlossary:poweredBy: Cofax\n"
        "      configGlossary:poweredByIcon: \"/images/cofax.gif\"\n"
        "      configGlossary:staticPath: \"/content/static\"\n"
        "      templateProcessorClass: org.cofax.WysiwygTemplate\n"
        "      templateLoaderClass: org.cofax.FilesTemplateLoader\n"
        "      templatePath: templates\n"
        "      templateOverridePath: ''\n"
        "      defaultListTemplate: listTemplate.htm\n"
        "      defaultFileTemplate: articleTemplate.htm\n"
        "      useJSP: false\n"
        "      jspListTemplate: listTemplate.jsp\n"
        "      jspFileTemplate: articleTemplate.jsp\n"
        "      cachePackageTagsTrack: 200\n"
        "      cachePackageTagsStore: 200\n"
        "      cachePackageTagsRefresh: 60\n"
        "      cacheTemplatesTrack: 100\n"
        "      cacheTemplatesStore: 50\n"
        "      cacheTemplatesRefresh: 15\n"
        "      cachePagesTrack: 200\n"
        "      cachePagesStore: 100\n"
        "      cachePagesRefresh: 10\n"
        "      cachePagesDirtyRead: 10\n"
        "      searchEngineListTemplate: forSearchEnginesList.htm\n"
        "      searchEngineFileTemplate: forSearchEngines.htm\n"
        "      searchEngineRobotsDb: WEB-INF/robots.db\n"
        "      useDataStore: true\n"
        "      dataStoreClass: org.cofax.SqlDataStore\n"
        "      redirectionClass: org.cofax.SqlRedirection\n"
        "      dataStoreName: cofax\n"
        "      dataStoreDriver: com.microsoft.jdbc.sqlserver.SQLServerDriver\n"
        "      dataStoreUrl: jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon\n"
        "      dataStoreUser: sa\n"
        "      dataStorePassword: dataStoreTestQuery\n"
        "      dataStoreTestQuery: SET NOCOUNT ON;select test='test';\n"
        "      dataStoreLogFile: \"/usr/local/tomcat/logs/datastore.log\"\n"
        "      dataStoreInitConns: 10\n"
        "      dataStoreMaxConns: 100\n"
        "      dataStoreConnUsageLimit: 100\n"
        "      dataStoreLogLevel: debug\n"
        "      maxUrlLength: 500\n"
        "  - servlet-name: cofaxEmail\n"
        "    servlet-class: org.cofax.cds.EmailServlet\n"
        "    init-param:\n"
        "      mailHost: mail1\n"
        "      mailHostOverride: mail2\n"
        "  - servlet-name: cofaxAdmin\n"
        "    servlet-class: org.cofax.cds.AdminServlet\n"
        "  - servlet-name: fileServlet\n"
        "    servlet-class: org.cofax.cds.FileServlet\n"
        "  - servlet-name: cofaxTools\n"
        "    servlet-class: org.cofax.cms.CofaxToolsServlet\n"
        "    init-param:\n"
        "      templatePath: toolstemplates/\n"
        "      log: 1\n"
        "      logLocation: \"/usr/local/tomcat/logs/CofaxTools.log\"\n"
        "      logMaxSize: ''\n"
        "      dataLog: 1\n"
        "      dataLogLocation: \"/usr/local/tomcat/logs/dataLog.log\"\n"
        "      dataLogMaxSize: ''\n"
        "      removePageCache: \"/content/admin/remove?cache=pages&id=\"\n"
        "      removeTemplateCache: \"/content/admin/remove?cache=templates&id=\"\n"
        "      fileTransferFolder: \"/usr/local/tomcat/webapps/content/fileTransferFolder\"\n"
        "      lookInContext: 1\n"
        "      adminGroupID: 4\n"
        "      betaServer: true\n"
        "  servlet-mapping:\n"
        "    cofaxCDS: \"/\"\n"
        "    cofaxEmail: \"/cofaxutil/aemail/*\"\n"
        "    cofaxAdmin: \"/admin/*\"\n"
        "    fileServlet: \"/static/*\"\n"
        "    cofaxTools: \"/tools/*\"\n"
        "  taglib:\n"
        "    taglib-uri: cofax.tld\n"
        "    taglib-location: \"/WEB-INF/tlds/cofax.tld\"\n"
        "\n"
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

    ffw::YamlIterator node;
    ffw::Node obj;
    REQUIRE(!node);

    REQUIRE(json.getNext(&node));
    REQUIRE(node);
    REQUIRE(node.getKey() == "web-app");
    REQUIRE(node.isObject() == true);
    REQUIRE_NOTHROW(obj = node.get());

    REQUIRE_NOTHROW(compare(obj, example["web-app"]));

    ffw::YamlWriter writer;
    writer.add(example);

    ffw::YamlReader tester(writer.str());

    REQUIRE(tester.getNext(&node));
    REQUIRE_NOTHROW(obj = node.toObject());

    REQUIRE_NOTHROW(compare(obj, example["web-app"]));
}
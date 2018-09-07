/**
 * @defgroup data data - XML, JSON, YAML, CSV, and INI serialisers and deserialisers
 * @brief Read and write XML, JSON, YAML, CSV, and INI files using universal API
 * @details Uses third party libraries
 * [tinyxml-2](https://github.com/leethomason/tinyxml2),
 * [parson](https://github.com/kgabis/parson) and
 * [yaml-cpp](https://github.com/jbeder/yaml-cpp)
 */
#ifndef FFW_DATA_MODULE
#define FFW_DATA_MODULE
#include "data/jsonreader.h"
#include "data/jsonwriter.h"
#include "data/datareader.h"
#include "data/datawriter.h"
#include "data/xmlreader.h"
#include "data/xmlwriter.h"
#include "data/yamlreader.h"
#include "data/yamlwriter.h"
#endif
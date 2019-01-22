# FineData

|         | CircleCI | TravisCI | AppVeyor |
|---------|----------|----------|----------|
| master  | [![CircleCI](https://circleci.com/gh/matusnovak/finedata/tree/master.svg?style=svg)](https://circleci.com/gh/matusnovak/finedata/tree/master) | [![Build Status](https://travis-ci.org/matusnovak/finedata.svg?branch=master)](https://travis-ci.org/matusnovak/finedata) | [![Build status](https://ci.appveyor.com/api/projects/status/7ptts9y3grin6jqs/branch/master?svg=true)](https://ci.appveyor.com/project/matusnovak/finedata/branch/master) |
| devel  | [![CircleCI](https://circleci.com/gh/matusnovak/finedata/tree/devel.svg?style=svg)](https://circleci.com/gh/matusnovak/finedata/tree/devel) | [![Build Status](https://travis-ci.org/matusnovak/finedata.svg?branch=devel)](https://travis-ci.org/matusnovak/finedata) | [![Build status](https://ci.appveyor.com/api/projects/status/7ptts9y3grin6jqs/branch/devel?svg=true)](https://ci.appveyor.com/project/matusnovak/finedata/branch/devel) |

*The following compilers are tested in the CI above: Visual Studio 2015, Visual Studio 2015 Win64, Visual Studio 2017, Visual Studio 2017 Win64, MinGW-w64 i686, MinGW-w64 x86_64, Linux GCC 5.5.0, Linux GCC 6.4.0, Linux GCC 5.5.0, Linux GCC 7.3.0, and OSX Clang 3.7*

## Introduction 

The FineData is a standalone library that provides the basic API wrappers around reading and writing JSON, YAML, and XML files using a universal API. Meaning, the same piece of code that you can use to load a JSON file can be used to load a YAML file. The library provides a unified ffw::DataReader and ffw::DataWriter classes that can give you an ffw::DataIterator. This iterator can be used to go through the data structure of YAML, JSON, and XML file. The iterator can also perform checks on the current node, such as isString(), isFloat(), isNumber(), isArray(), etc. The library also offers a ffw::Node data structure. This is very similar to how JavaScript variables behave. This ffw::Node can be any of the following types: string, bool, int64, double, nullptr, array, and a map/object. You can the ffw::DataReader to convert the given node, or an entire structure, into the ffw::Node. This same ffw::Node can be used for all (JSON, YAML, and XML) data types. Sure, this is not the perfect design, but allows you to read any of the tree data types (or write to) and not worry about handling three different spaghetti code to handle all types. Just one simple unified interface.

Please keep in mind, that the reading and writing XML through ffw::Node is not 100% compatible with the entire XML standard. Due to the reasons that there is not an easy way to spot an array of items in XML, or everything is a string! This library tries to do the best, but I guarantee you there will be some weird behavior. Anyway, you should not use XML in 2018! JSON and YAML are better.

This library also bundles all third party libraries statically. Meaning, you only have to link the `.lib`  file (or `.so` on Linux). No need to manually download the libraries (GLFW, FreeType2, etc.), and spend an entire weekend figuring out how to plug it all together, and going through outdated or complicated instructions for some libraries. You either download the pre-built FineData from the GitHub releases, or build it manually (which is super easy), and you are good to go!

**API Documentation is provided at <http://matusnovak.github.io/finedata/>**

## Dependencies

All dependencies listed here are already included as a git submodule and will be statically linked to the finedata library. Therefore, no need to compile them manually, nor link them manually! Everything is automated via CMake.

* [tinyxml2](https://github.com/leethomason/tinyxml2.git) - For parsing XML files
* [json-c](https://github.com/json-c/json-c.git) - For parsing JSON files
* [yaml-cpp](https://github.com/jbeder/yaml-cpp.git) - For parsing YAML files

## TODO

* More examples, documentation, and tutorials
* More unit tests
* Add CSV and INI data structures support

## Quick example

Create a rendering window and draw a rectangle:

```cpp
#include <ffw/data.h>
#include <iostream>

int main(int argc, char *argv[]) {
    try {
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
        ); // throws on error

        ffw::JsonIterator node;

        // Get the root object
        json.getNext(&node); // Get the root object

        // Returns true because the root is an object/map
        node.isObject();

        // Steps into the last json.getNext()
        json.stepInto();

        // Gets the first item from the object
        json.getNext(&node);

        // Returns "string"
        node.getKey();

        // Returns "Hello World!"
        node.toString();

        /* after going through all items in the object */
        // Returns false because there is no other item in the object!
        json.getNext(&node); 

        // Steps out into the root. We are done now!
        // If there was an object within an object (or array within an array)
        // you can use this function to step out into the parent.
        json.stepOut();
    } catch (ffw::JsonReaderException& e) {
        std::cerr << e.what() << std::endl;
    }
}
```

An alternative approach with shorter quick and dirty API:

```cpp
#include <ffw/data.h>
#include <iostream>

int main(int argc, char *argv[]) {
    try {
        ffw::Node json = ffw::decodeJson(
            "{"
            "    \"string\": \"Hello World!\","
            "    \"integer\": -123,"
            "    \"float\": 0.123,"
            "    \"null\": null,"
            "    \"object\": {\"first\": 10, \"second\": 20},"
            "    \"array\": [10, 20],"
            "    \"boolean\": true"
            "}"
        ); // throws on error

        // "json" now contains the root object (and all children)

        // Returns true because the root is not an array
        json.isObject();

        // Cast to object (no actual conversion is done)
        // ffw::Node::Object is actually std::unordered_map<std::string, ffw::Node>
        const ffw::Node::Object& root = json.getAsObject();

        // Returns 7
        root.size();

        // Returns the "integer" item
        const ffw::Node& item = root.at("integer");

        // Returns true
        item.isInteger();

        // Get the real value
        uint64_t i = item.getAsInt();

    } catch (ffw::JsonReaderException& e) {
        std::cerr << e.what() << std::endl;
    }
}
```

## Requirements

One of the following compilers:

* (Windows) Visual Studio 2015 32-bit or 64-bit
* (Windows) Visual Studio 2017 32-bit or 64-bit
* (Windows) MinGW-w64 i686
* (Windows) MinGW-w64 x86_64
* (Linux) GCC 4.9 or newer
* (OSX) Clang 3.7 or newer

## Compiling FineData

You can [use one of the automatically built releases from GitHub Releases](https://github.com/matusnovak/finedata/releases) or compile it manually by following the [Install Guide](./INSTALL.md) (Don't worry, it's easy). 

## Using FineData

### Windows

First, compile the library (see section above), next to use the finedata in your project, simply include the `C:/path/to/finedata/include` and link the `finedata.lib`. You will also need the `finedata.dll` in order to run the application. Simply copy the DLL into the executable folder. You can find the `.lib` and `.dll` files in the cmake build folder.

### Linux/OSX

First, compile the library (see section above), next to use the finedata in your project, simply include the `~/finedata/include` (or `/usr/local/include` if you have installed it) and link the `finedata.so`. You can find the `.so` files in the cmake build folder (or at the installed path provided by CMAKE_INSTALL_PREFIX).

If you are planning to distribute your application with FineData, you will need to copy the `finedata.so` and include it within your application. For example, the most simple way, copy it into the executable folder (same as in Windows).

## Documentation

Documentation can be found at <http://matusnovak.github.io/finedata/>

## Examples

All examples are located in the examples folder.

## Contributing

Feel free to post a question in GitHub issues. Pull requests are welcome! 

## Contact

Feel free to contact me on my email (see my GitHub profile).

## License

<https://en.wikipedia.org/wiki/MIT_License>

```
The MIT License (MIT)

Copyright (c) 2018 Matus Novak

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
cmake_minimum_required(VERSION 3.1)
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

ExternalProject_Add(YAMLCPP
  DOWNLOAD_COMMAND ""
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/yaml-cpp
  CMAKE_CACHE_ARGS "-DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true"
  CMAKE_ARGS -DBUILD_GMOCK=OFF -DYAML_BUILD_CONTRIB=OFF -DYAML_BUILD_TESTS=OFF -DYAML_BUILD_TOOLS=OFF -Dgtest_build_examples=OFF -Dgtest_build_samples=OFF -DBUILD_SHARED_LIBS=OFF  -DCMAKE_C_FLAGS="-fPIC"
  BUILD_COMMAND cmake --build . --target "yaml-cpp" --config MinSizeRel && cmake --build . --target "yaml-cpp" --config Debug
  INSTALL_COMMAND ""
  TEST_COMMAND ""
)

if(MSVC)
  set(JSONC_LIBRARY optimized ${CMAKE_BINARY_DIR}/YAMLCPP-prefix/src/YAMLCPP-build/MinSizeRel/libyaml-cppmd.lib debug ${CMAKE_BINARY_DIR}/YAMLCPP-prefix/src/YAMLCPP-build/Debug/libyaml-cppmdd.lib)
else(MSVC)
  set(JSONC_LIBRARY ${CMAKE_BINARY_DIR}/YAMLCPP-prefix/src/YAMLCPP-build/libyaml-cpp.a)
endif(MSVC)

ExternalProject_Add(JSONC
  DOWNLOAD_COMMAND ""
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/libs/json-c
  CMAKE_CACHE_ARGS "-DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true"
  CMAKE_ARGS -DBUILD_SHARED_LIBS=OFF -DENABLE_RDRAND=OFF -DENABLE_THREADING=OFF -DCMAKE_C_FLAGS="-fPIC"
  BUILD_COMMAND cmake --build . --target "json-c" --config MinSizeRel
  INSTALL_COMMAND ""
  TEST_COMMAND ""
)

if(MSVC)
  set(YAMLCPP_LIBRARY ${CMAKE_BINARY_DIR}/JSONC-prefix/src/JSONC-build/MinSizeRel/json-c.lib)
else(MSVC)
  set(YAMLCPP_LIBRARY ${CMAKE_BINARY_DIR}/JSONC-prefix/src/JSONC-build/libjson-c.a)
endif(MSVC)

set(TINYXML2_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/tinyxml2)
set(JSONC_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/json-c ${CMAKE_BINARY_DIR}/JSONC-prefix/src/JSONC-build)
set(YAMLCPP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libs/yaml-cpp/include)

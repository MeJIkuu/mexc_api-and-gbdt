cmake_minimum_required(VERSION 3.5)

project(mexc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(mbedtls-3.6.0 URL https://github.com/Mbed-TLS/mbedtls/releases/download/v3.6.0/mbedtls-3.6.0.tar.bz2)
FetchContent_MakeAvailable(mbedtls-3.6.0)

include_directories("rapidjson-0.11/include")

option(ENABLE_TESTING OFF)
option(ENABLE_PROGRAMS OFF)

add_library(gbdt
    gbdt/gbdt.cc
    gbdt/gbdt.h
    gbdt/json.cc
    gbdt/json.h
    gbdt/lm.cc
    gbdt/lm.h
    gbdt/lm-scorer.cc
    gbdt/lm-scorer.h
    gbdt/lm-util.h
    gbdt/node.cc
    gbdt/node.h
    gbdt/param.cc
    gbdt/param.h
    gbdt/x.cc
    gbdt/x.h
    gbdt/sample.cc
    gbdt/sample.h)

add_executable(mexc
    main.cpp
    flags/flags.h
    mexc/mexc.hpp)

target_link_libraries(mexc curl nlohmann_json::nlohmann_json mbedtls gbdt)

include(GNUInstallDirs)
install(TARGETS mexc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

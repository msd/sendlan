cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)

project(SendLAN VERSION 0.0.1 LANGUAGES CXX C)

find_package(Boost REQUIRED COMPONENTS system regex locale)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "install location" FORCE)
    set("${CMAKE_INSTALL_PREFIX}/bin")
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(TEST_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test")

set ( DEBUG_BUILD TRUE CACHE BOOL "enable extra debugging tests & messages" )
set ( BUILD_TESTS FALSE CACHE BOOL "whether to build the tests" )

add_library(common INTERFACE)
target_compile_features(common INTERFACE cxx_std_23)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (NOT CMAKE_HOST_WIN32 OR CYGWIN)
        target_link_libraries(common INTERFACE
            "-fsanitize=address" # not avaiable on MSYS/MINGW
        )
    endif()
    target_compile_options(common INTERFACE 
        "-Wshadow" # alternatively use either -Wshadow-compatible OR -Wshadow-compatible-local
        "-fdiagnostics-color" # add color to output, only for MSYS/MINGW
    )
    target_link_libraries(common INTERFACE "-static-libgcc")
    # link_libraries("-static-libstdc++") # causes linking errors, sometimes
    if (DEBUG_BUILD)
        target_link_libraries(common INTERFACE "-ggdb3")
    endif()
endif ()
# TODO IF MSVC -> add_compile_options("/WX /Wall")

include_directories("${CMAKE_SOURCE_DIR}")

if (BUILD_TESTS)
    find_package(Catch2 3 REQUIRED)
    
    add_executable(test_endian test_endian.cpp )
    target_link_libraries(test_endian PUBLIC common Catch2::Catch2WithMain)
    
    add_executable(test test.cpp)
    target_link_libraries(test PUBLIC common)
    
    add_executable(test_view test_view.cpp)
    target_link_libraries(test_view PUBLIC common)
    target_link_libraries(test_view PUBLIC Boost::system Boost::regex Boost::locale)
    target_link_libraries(test_view PUBLIC "ws2_32" "mswsock")

    set_target_properties(
        test test_endian test_view
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${TEST_OUTPUT_DIRECTORY}"
    )
endif()

add_executable(server server.cpp)
target_link_libraries(server PUBLIC common)
target_link_libraries(server PUBLIC Boost::system Boost::regex Boost::locale)
target_link_libraries(server PUBLIC "ws2_32" "mswsock")

add_executable(client client.cpp)
target_link_libraries(client PUBLIC common)
target_link_libraries(client PUBLIC Boost::system Boost::regex Boost::locale)
target_link_libraries(client PUBLIC "ws2_32" "mswsock")

if (DEBUG_BUILD)
    add_compile_definitions(SENDLAN_PROJECT_DEBUG)
endif()

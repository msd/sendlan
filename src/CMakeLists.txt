cmake_minimum_required(VERSION 3.24.0 FATAL_ERROR)

project(SendLAN VERSION 0.0.1 LANGUAGES CXX C)

find_package(Boost REQUIRED COMPONENTS system regex locale)

set ( DEBUG_BUILD TRUE CACHE BOOL "enable extra debugging tests & messages" )
set ( BUILD_TESTS FALSE CACHE BOOL "whether to build the tests" )

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if (NOT MINGW)
        add_compile_options(
            "-fsanitize=address" # not avaiable on MSYS/MINGW
        )
    endif()
    add_compile_options(
        "-Wshadow" # alternatively use either -Wshadow-compatible OR -Wshadow-compatible-local
        "-fdiagnostics-color" # add color to output, only for MSYS/MINGW
    )
    link_libraries("-static-libgcc")
    # link_libraries("-static-libstdc++") # causes linking errors, sometimes
endif ()
# TODO IF MSVC -> add_compile_options("/WX /Wall")

if (BUILD_TESTS)
    find_package(Catch2 REQUIRED)
    add_executable(tests tests.cpp hashing.cpp utils.cpp)
    target_link_libraries(tests PUBLIC Catch2::Catch2)
endif()

include_directories("${CMAKE_SOURCE_DIR}")

add_executable(server server.cpp)
target_compile_features(server PUBLIC cxx_std_23)
target_link_libraries(server PUBLIC Boost::system Boost::regex Boost::locale)
target_link_libraries(server PUBLIC "ws2_32" "mswsock")

add_executable(client client.cpp)
target_compile_features(client PUBLIC cxx_std_23)
target_link_libraries(client PUBLIC Boost::system Boost::regex Boost::locale)
target_link_libraries(client PUBLIC "ws2_32" "mswsock")

add_executable(test test.cpp)
target_compile_features(test PUBLIC cxx_std_23)

if (DEBUG_BUILD)
    add_compile_definitions(SENDLAN_PROJECT_DEBUG)
endif()

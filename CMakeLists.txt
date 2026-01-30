cmake_minimum_required(VERSION 3.28)
project(ReteNeurale)

add_compile_definitions(ROOT_DIR="${CMAKE_SOURCE_DIR}")

include(CTest)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_compile_options(-fno-omit-frame-pointer -Wall -Wextra -Wpedantic -Werror -Wconversion)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    add_compile_definitions(_GLIBCXX_ASSERTIONS)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_definitions(_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()

set(RETE_SOURCES 
    rete_neurale/pattern.cpp 
    rete_neurale/matrix.cpp 
    rete_neurale/acquisition.cpp
)

add_executable(ReteNeurale main.cpp ${RETE_SOURCES})
target_link_libraries(ReteNeurale sfml-graphics sfml-window sfml-system)

if (BUILD_TESTING)
    add_executable(ReteNeuraleTest 
        test/test_runner.cpp 
        test/pattern_test.cpp 
        test/acquisition_test.cpp 
        test/matrix_test.cpp 
        ${RETE_SOURCES}
    )

    target_include_directories(ReteNeuraleTest PRIVATE test rete_neurale)
    target_link_libraries(ReteNeuraleTest sfml-graphics sfml-window sfml-system)
    add_test(NAME ReteNeuraleTest COMMAND ReteNeuraleTest)
endif()
cmake_minimum_required(VERSION 3.28)
project(ReteNeurale)

# abilita il supporto per i test, tra cui l'opzione BUILD_TESTING usata sotto
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++20, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED) #cerca la libreria sfml se no è finita

# abilita warning
string(APPEND CMAKE_CXX_FLAGS " -fno-omit-frame-pointer -Wall -Wextra")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_ASSERTIONS")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  string(APPEND CMAKE_CXX_FLAGS " -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE")
endif()

# abilita l'address sanitizer e l'undefined-behaviour sanitizer in debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined")

add_executable(ReteNeurale pattern.cpp main.cpp acquisition.cpp recall.cpp train.cpp)

target_link_libraries(ReteNeurale sfml-graphics sfml-window sfml-system)

# se il testing e' abilitato...
#   per disabilitare il testing, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
if (BUILD_TESTING)

  # aggiungi l'eseguibile sample.t
  add_executable (ReteNeuraleTest.t pattern_test.cpp recall_test.cpp train_test.cpp acquisition_test.cpp)
  # aggiungi l'eseguibile sample.t alla lista dei test
  add_test(NAME ReteNeuraleTest.t COMMAND ReteNeuraleTest.t)

endif()
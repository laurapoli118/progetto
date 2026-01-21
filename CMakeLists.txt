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

set(RETE_SOURCES rete_neurale/pattern.cpp rete_neurale/matrix.cpp rete_neurale/acquisition.cpp)

add_executable(ReteNeurale main.cpp ${RETE_SOURCES})
target_compile_definitions(ReteNeurale PRIVATE ROOT_DIR=\"${CMAKE_SOURCE_DIR}\") # questo dovrebbe rendere il percorso per le immagini assoluto

target_link_libraries(ReteNeurale sfml-graphics sfml-window sfml-system)

# se il testing e' abilitato...
#   per disabilitare il testing, passare -DBUILD_TESTING=OFF a cmake durante la fase di configurazione
if (BUILD_TESTING)

  # aggiungi l'eseguibile sample.t
  add_executable (ReteNeuraleTest test/test_runner.cpp test/pattern_test.cpp test/acquisition_test.cpp test/matrix_test.cpp ${RETE_SOURCES})
  # aggiungi l'eseguibile sample.t alla lista dei test
  target_include_directories(ReteNeuraleTest PRIVATE test rete_neurale)
  target_compile_definitions(ReteNeuraleTest PRIVATE ROOT_DIR=\"${CMAKE_SOURCE_DIR}\")
  target_link_libraries(ReteNeuraleTest sfml-graphics sfml-window sfml-system)
  add_test(NAME ReteNeuraleTest COMMAND ReteNeuraleTest)

endif()
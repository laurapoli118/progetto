# CMake generated Testfile for 
# Source directory: /home/amasini/masi/progetto/progetto
# Build directory: /home/amasini/masi/progetto/progetto/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test([=[ReteNeuraleTest.t]=] "/home/amasini/masi/progetto/progetto/build/Debug/ReteNeuraleTest.t")
  set_tests_properties([=[ReteNeuraleTest.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/amasini/masi/progetto/progetto/CMakeLists.txt;40;add_test;/home/amasini/masi/progetto/progetto/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test([=[ReteNeuraleTest.t]=] "/home/amasini/masi/progetto/progetto/build/Release/ReteNeuraleTest.t")
  set_tests_properties([=[ReteNeuraleTest.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/amasini/masi/progetto/progetto/CMakeLists.txt;40;add_test;/home/amasini/masi/progetto/progetto/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test([=[ReteNeuraleTest.t]=] "/home/amasini/masi/progetto/progetto/build/RelWithDebInfo/ReteNeuraleTest.t")
  set_tests_properties([=[ReteNeuraleTest.t]=] PROPERTIES  _BACKTRACE_TRIPLES "/home/amasini/masi/progetto/progetto/CMakeLists.txt;40;add_test;/home/amasini/masi/progetto/progetto/CMakeLists.txt;0;")
else()
  add_test([=[ReteNeuraleTest.t]=] NOT_AVAILABLE)
endif()

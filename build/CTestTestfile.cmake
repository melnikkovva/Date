# CMake generated Testfile for 
# Source directory: C:/Users/melni/Desktop/cppcourse/Date/Date
# Build directory: C:/Users/melni/Desktop/cppcourse/Date/Date/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(CDateTests "C:/Users/melni/Desktop/cppcourse/Date/Date/build/Debug/CDateTests.exe")
  set_tests_properties(CDateTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melni/Desktop/cppcourse/Date/Date/CMakeLists.txt;15;add_test;C:/Users/melni/Desktop/cppcourse/Date/Date/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(CDateTests "C:/Users/melni/Desktop/cppcourse/Date/Date/build/Release/CDateTests.exe")
  set_tests_properties(CDateTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melni/Desktop/cppcourse/Date/Date/CMakeLists.txt;15;add_test;C:/Users/melni/Desktop/cppcourse/Date/Date/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(CDateTests "C:/Users/melni/Desktop/cppcourse/Date/Date/build/MinSizeRel/CDateTests.exe")
  set_tests_properties(CDateTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melni/Desktop/cppcourse/Date/Date/CMakeLists.txt;15;add_test;C:/Users/melni/Desktop/cppcourse/Date/Date/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(CDateTests "C:/Users/melni/Desktop/cppcourse/Date/Date/build/RelWithDebInfo/CDateTests.exe")
  set_tests_properties(CDateTests PROPERTIES  _BACKTRACE_TRIPLES "C:/Users/melni/Desktop/cppcourse/Date/Date/CMakeLists.txt;15;add_test;C:/Users/melni/Desktop/cppcourse/Date/Date/CMakeLists.txt;0;")
else()
  add_test(CDateTests NOT_AVAILABLE)
endif()

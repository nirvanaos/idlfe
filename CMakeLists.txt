cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0010 NEW)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

project(idlfe CXX)
add_subdirectory(src)
add_subdirectory(example)

set_target_properties(idlfe PROPERTIES PUBLIC_HEADER "src/include/IDL_FrontEnd.h")
install(DIRECTORY "src/include/AST" "src/include/BE" TYPE INCLUDE)
install(TARGETS idlfe decNumber)

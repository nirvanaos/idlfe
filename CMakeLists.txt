cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0010 NEW)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

project(idlfe CXX)
add_subdirectory(src)
add_subdirectory(example)

install(TARGETS idlfe decNumber LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*")

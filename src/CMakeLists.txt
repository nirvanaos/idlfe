include(FetchContent)

FetchContent_Declare(
    decNumber
    GIT_REPOSITORY https://github.com/nirvanaos/decNumber.cmake.git
)

FetchContent_Declare(
    SafeInt
    GIT_REPOSITORY https://github.com/dcleblanc/SafeInt.git
)

FetchContent_MakeAvailable(decNumber SafeInt)

find_package(BISON)
find_package(FLEX)

#[===[
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
]===]

BISON_TARGET(MyParser FE/yacc.yy ${CMAKE_CURRENT_SOURCE_DIR}/FE/yacc.tab.cpp
    DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/FE/yacc.tab.h)

FLEX_TARGET(MyScanner FE/scanner.ll ${CMAKE_CURRENT_SOURCE_DIR}/FE/scanner.flex.cpp
    COMPILE_FLAGS "--wincompat --c++")

set(sources 
    IDL_FrontEnd.cpp
    simplecpp/simplecpp.cpp)
add_library(idlfe STATIC ${sources})
target_compile_features(idlfe PUBLIC cxx_std_17)
if(MSVC)
    target_compile_options(idlfe PUBLIC "/Zc:__cplusplus")
endif()
target_include_directories(idlfe PUBLIC include PRIVATE FE ${decNumber_SOURCE_DIR} ${safeint_SOURCE_DIR} ${FLEX_INCLUDE_DIRS})

target_sources(idlfe PRIVATE
    FE/Driver.cpp
    FE/scanner.flex.cpp
    FE/yacc.tab.cpp)

add_subdirectory(AST)
add_subdirectory(BE)

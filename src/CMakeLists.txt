include(FetchContent)

FetchContent_Declare(
    decNumber
    GIT_REPOSITORY https://github.com/nirvanaos/decNumber.cmake.git
    GIT_SHALLOW true
)

FetchContent_Declare(
    SafeInt
    GIT_REPOSITORY https://github.com/dcleblanc/SafeInt.git
    GIT_SHALLOW true
)

FetchContent_MakeAvailable(decNumber)

# use fetch content rather than makeavailable because safeint only includes unconditional test targets
FetchContent_Populate(SafeInt)

find_package(BISON)
find_package(FLEX)

#[===[
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()
]===]

BISON_TARGET(MyParser FE/yacc.yy ${CMAKE_CURRENT_BINARY_DIR}/FE/yacc.tab.cpp
    DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/FE/yacc.tab.h)

if(CMAKE_HOST_WIN32)
set(FLEX_FLAGS "--wincompat")
else()
set(FLEX_FLAGS "")
endif()

FLEX_TARGET(MyScanner FE/scanner.ll ${CMAKE_CURRENT_BINARY_DIR}/FE/scanner.flex.cpp
    COMPILE_FLAGS ${FLEX_FLAGS})

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_library(idlfe STATIC 
    IDL_FrontEnd.cpp
    simplecpp/simplecpp.cpp
    FE/Driver.cpp
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS})

target_compile_features(idlfe PUBLIC cxx_std_17)
if(MSVC)
    target_compile_options(idlfe PRIVATE "/Zc:__cplusplus" "-D__STDC_VERSION__=199901L")
endif()

target_include_directories(idlfe PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

include_directories(
    FE
    ${decNumber_SOURCE_DIR}
    ${safeint_SOURCE_DIR}
    ${FLEX_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}/FE)

add_subdirectory(AST)
add_subdirectory(BE)

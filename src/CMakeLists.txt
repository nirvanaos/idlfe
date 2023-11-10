include(FetchContent)

FetchContent_Declare(
    decNumber
    GIT_REPOSITORY https://github.com/nirvanaos/decNumber.cmake.git
)

FetchContent_Declare(
    SafeInt
    GIT_REPOSITORY https://github.com/dcleblanc/SafeInt.git
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

BISON_TARGET(MyParser FE/yacc.yy ${CMAKE_CURRENT_SOURCE_DIR}/FE/yacc.tab.cpp
    DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/FE/yacc.tab.h)

if(WIN32)
set(FLEX_FLAGS "--wincompat")
else()
set(FLEX_FLAGS "")
endif()

FLEX_TARGET(MyScanner FE/scanner.ll ${CMAKE_CURRENT_SOURCE_DIR}/FE/scanner.flex.cpp
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
    target_compile_options(idlfe PUBLIC "/Zc:__cplusplus" "-D__STDC_VERSION__=199901L")
endif()
target_include_directories(idlfe PUBLIC include PRIVATE FE ${decNumber_SOURCE_DIR} ${safeint_SOURCE_DIR} ${FLEX_INCLUDE_DIRS})

add_subdirectory(AST)
add_subdirectory(BE)

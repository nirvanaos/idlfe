include(FetchContent)
FetchContent_Declare(
    decNumber
    GIT_REPOSITORY https://github.com/nirvanaos/decNumber.cmake.git
)

FetchContent_MakeAvailable(decNumber)

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

set(sources 
    IDL_FrontEnd.cpp
    simplecpp/simplecpp.cpp)
add_library(idlfe STATIC ${sources})
target_compile_features(idlfe PUBLIC cxx_std_17)
if(MSVC)
    target_compile_options(idlfe PUBLIC "/Zc:__cplusplus")
endif()
target_include_directories(idlfe PUBLIC include PRIVATE FE ${decNumber_SOURCE_DIR} ${CMAKE_INCLUDE_PATH})

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/FE/scanner.flex.cpp
    COMMAND win_flex.exe --outfile=scanner.flex.cpp --wincompat --c++ scanner.ll
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/FE
    VERBATIM
    DEPENDS FE/scanner.ll)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/FE/yacc.tab.cpp ${CMAKE_CURRENT_SOURCE_DIR}/FE/yacc.tab.h
    COMMAND win_bison.exe --output=yacc.tab.cpp --defines=yacc.tab.h yacc.yy
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/FE
    VERBATIM
    DEPENDS FE/yacc.yy)

target_sources(idlfe PRIVATE
    FE/Driver.cpp
    FE/scanner.flex.cpp
    FE/yacc.tab.cpp)

add_subdirectory(AST)
add_subdirectory(BE)

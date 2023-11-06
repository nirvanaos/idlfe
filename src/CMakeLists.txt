find_package(safeint REQUIRED)

set(sources 
    IDL_FrontEnd.cpp
    simplecpp/simplecpp.cpp)
add_library(idlfe STATIC ${sources})
target_compile_features(idlfe PUBLIC cxx_std_17)
target_include_directories(idlfe PUBLIC include PRIVATE FE ${safeint_INCLUDE_DIR})

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
    FE/scanner.flex.cpp
    FE/yacc.tab.cpp)

#add_subdirectory(FE)
add_subdirectory(AST)
add_subdirectory(BE)

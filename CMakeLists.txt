cmake_minimum_required(VERSION 3.20)
cmake_policy(SET CMP0010 NEW)
set(CMAKE_CONFIGURATION_TYPES Debug Release)

project(idlfe CXX)
add_subdirectory(src)
add_subdirectory(example)

install(TARGETS idlfe decNumber EXPORT idlfe_targets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h*")

# Generate and install *-targets.cmake 
install(EXPORT idlfe_targets
        FILE idlfe-targets.cmake
        NAMESPACE idlfe::
        DESTINATION share/idlfe)

# Generate the config file in the current binary dir (this ensures it's not placed directly in source)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/idlfe-config.cmake"
"include(\"\${CMAKE_CURRENT_LIST_DIR}/idlfe-targets.cmake\")\n"
)

# Install the generated config file
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/idlfe-config.cmake"
        DESTINATION share/idlfe)

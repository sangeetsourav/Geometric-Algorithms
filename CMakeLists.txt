# CMakeList.txt : CMake project for geometric_algorithms, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.14)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("geometric_algorithms")

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add source to this project's executable.
add_executable (geometric_algorithms "geometric_algorithms.cpp" "line_sweep.h"  "delauney.h" "common.h")

find_package(spdlog CONFIG REQUIRED)
    target_link_libraries(geometric_algorithms PRIVATE spdlog::spdlog_header_only)

# Copy data files after building the target
add_custom_command(TARGET geometric_algorithms POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/input
    $<TARGET_FILE_DIR:geometric_algorithms>/input
    
    COMMENT "Copying all data files to executable directory")

# TODO: Add tests and install targets if needed.

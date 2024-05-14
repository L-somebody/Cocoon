cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Pine
        VERSION 0.1
        DESCRIPTION "pine"
        LANGUAGES C CXX
        )

# Check if the user make the project in the correct file path.
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" PATH_TO_CMAKELISTS_TXT) # Transport file path to CMake format path, ensure style consistency across platforms
if(EXISTS "${PATH_TO_CMAKELISTS_TXT}")
    message(FATAL_ERROR "Run CMake from a build subdirectory! \"mkdir build ; cd build ; cmake .. \" \
    Some junk files were created in this folder (CMakeCache.txt, CMakeFiles); you should delete those.")
endif()

# Expected directory structure
set(PINE_BUILD_SUPPORT_DIR "${CMAKE_SOURCE_DIR}/build_support")
set(PINE_CLANG_SEARCH_PATH "/usr/local/bin" "/usr/bin" "/usr/local/opt/llvm/bin" "/usr/local/opt/llvm@8/bin" "/usr/local/Cellar/llvm/8.0.1/bin")

# Clang-format
if(NOT DEFINED CLANG_FORMAT_BIN)
    find_program(CLANG_FORMAT_BIN
            NAMES clang-format clang-format-8
            HINTS ${PINE_CLANG_SEARCH_PATH})
endif()
if("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
    message(WARNING "Pine/main couldn't find clang-format.")
else()
    message(STATUS "Pine/main found clang-format at ${CLANG_FORMAT_BIN}")
endif()

######################################################################################################################
# DEPENDENCIES CHECK
######################################################################################################################

# Clang-tiny
if (NOT DEFINED CLANG_TIDY_BIN)
    find_program(CLANG_TIDY_BIN
            NAMES clang-tidy clang-tidy-8
            HINTS ${PINE_CLANG_SEARCH_PATH})
endif ()
set(CLANG_TIDY_BIN "/usr/bin/clang-tidy")
if ("${CLANG_TIDY_BIN}" STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
    message(WARNING "Pine/main couldn't find clang-tidy.")
else ()
    set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
    message(STATUS "Pine/main found clang-tidy at ${CLANG_TIDY_BIN}")
endif ()

# Cpplint
find_program(CPPLINT_BIN
        NAMES cpplint cpplint.py
        HINTS ${PINE_BUILD_SUPPORT_DIR})
if ("${CPPLINT_BIN}" STREQUAL "CPPLINT_BIN-NOTFOUND")
    message(WARNING "Pine/main couldn't find cpplint.")
else ()
    message(STATUS "Pine/main found cpplint at ${CPPLINT_BIN}")
endif ()

######################################################################################################################
# COMPILER SETUP
######################################################################################################################

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra -std=c++17 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fPIC")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fPIC")

set(GCC_COVERAGE_LINK_FLAGS "-fPIC")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

# Output directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # static lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # shared lib
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # exe

# Includes
set(PINE_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(PINE_TEST_INCLUDE_DIR ${PROJECT_TEST_DIR}/test/include)
include_directories(${PINE_SRC_INCLUDE_DIR} ${PINE_TEST_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(test)

######################################################################################################################
# MAKE TARGETS
######################################################################################################################
string(CONCAT PINE_FORMAT_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/src,"
        "${CMAKE_CURRENT_SOURCE_DIR}/test,"
        )

# Find Python3
find_package(Python3 COMPONENTS Interpreter)

# runs clang format and updates files in place
# "make format"
add_custom_target(
        format 
        COMMAND ${Python3_EXECUTABLE} ${PINE_BUILD_SUPPORT_DIR}/run_clang_format.py
        ${CLANG_FORMAT_BIN}
        ${PINE_BUILD_SUPPORT_DIR}/clang_format_exclusions.txt
        --source_dirs
        ${PINE_FORMAT_DIRS}
        --fix
        --quiet
        )

# runs clang format and exits with a non-zero exit code if any files need to be reformatted
# "make check-format"
add_custom_target(
        check-format 
        COMMAND ${Python3_EXECUTABLE} ${PINE_BUILD_SUPPORT_DIR}/run_clang_format.py
        ${CLANG_FORMAT_BIN}
        ${PINE_BUILD_SUPPORT_DIR}/clang_format_exclusions.txt
        --source_dirs
        ${PINE_FORMAT_DIRS}
        --quiet
        )

# "make cpplint"
file(GLOB_RECURSE PINE_LINT_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp")

add_custom_target(cpplint echo '${PINE_LINT_FILES}' | xargs -n12 -P8
        COMMAND ${Python3_EXECUTABLE} ${CPPLINT_BIN}
        --verbose=2 --quiet
        --linelength=120
        --filter=-legal/copyright,-build/include_subdir,-readability/casting
        )

# "make clang-tidy"
add_custom_target(
        clang-tidy
        COMMAND ${Python3_EXECUTABLE} ${PINE_BUILD_SUPPORT_DIR}/run_clang_tidy.py  # run LLVM's clang-tidy script
        -clang-tidy-binary ${CLANG_TIDY_BIN}         # using our clang-tidy binary
        -p ${CMAKE_BINARY_DIR}                       # using cmake's generated compile commands
        )
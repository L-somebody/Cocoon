# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lcy/MyProjects/Test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lcy/MyProjects/Test/build

# Utility rule file for clang-tidy.

# Include the progress variables for this target.
include CMakeFiles/clang-tidy.dir/progress.make

CMakeFiles/clang-tidy:
	/usr/bin/python3.8 /home/lcy/MyProjects/Test/build_support/run_clang_tidy.py -clang-tidy-binary /usr/bin/clang-tidy -p /home/lcy/MyProjects/Test/build

clang-tidy: CMakeFiles/clang-tidy
clang-tidy: CMakeFiles/clang-tidy.dir/build.make

.PHONY : clang-tidy

# Rule to build all files generated by this target.
CMakeFiles/clang-tidy.dir/build: clang-tidy

.PHONY : CMakeFiles/clang-tidy.dir/build

CMakeFiles/clang-tidy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clang-tidy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clang-tidy.dir/clean

CMakeFiles/clang-tidy.dir/depend:
	cd /home/lcy/MyProjects/Test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lcy/MyProjects/Test /home/lcy/MyProjects/Test /home/lcy/MyProjects/Test/build /home/lcy/MyProjects/Test/build /home/lcy/MyProjects/Test/build/CMakeFiles/clang-tidy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/clang-tidy.dir/depend

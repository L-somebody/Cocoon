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

# Include any dependencies generated for this target.
include test/CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/client.dir/flags.make

test/CMakeFiles/client.dir/client.cpp.o: test/CMakeFiles/client.dir/flags.make
test/CMakeFiles/client.dir/client.cpp.o: ../test/client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lcy/MyProjects/Test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/client.dir/client.cpp.o"
	cd /home/lcy/MyProjects/Test/build/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/client.dir/client.cpp.o -c /home/lcy/MyProjects/Test/test/client.cpp

test/CMakeFiles/client.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client.dir/client.cpp.i"
	cd /home/lcy/MyProjects/Test/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lcy/MyProjects/Test/test/client.cpp > CMakeFiles/client.dir/client.cpp.i

test/CMakeFiles/client.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client.dir/client.cpp.s"
	cd /home/lcy/MyProjects/Test/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lcy/MyProjects/Test/test/client.cpp -o CMakeFiles/client.dir/client.cpp.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/client.cpp.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

bin/client: test/CMakeFiles/client.dir/client.cpp.o
bin/client: test/CMakeFiles/client.dir/build.make
bin/client: lib/libpine_shared.so
bin/client: test/CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lcy/MyProjects/Test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/client"
	cd /home/lcy/MyProjects/Test/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/client.dir/build: bin/client

.PHONY : test/CMakeFiles/client.dir/build

test/CMakeFiles/client.dir/clean:
	cd /home/lcy/MyProjects/Test/build/test && $(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/client.dir/clean

test/CMakeFiles/client.dir/depend:
	cd /home/lcy/MyProjects/Test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lcy/MyProjects/Test /home/lcy/MyProjects/Test/test /home/lcy/MyProjects/Test/build /home/lcy/MyProjects/Test/build/test /home/lcy/MyProjects/Test/build/test/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/client.dir/depend


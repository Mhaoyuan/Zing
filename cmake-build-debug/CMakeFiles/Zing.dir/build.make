# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /home/genius/Downloads/clion-2018.1.6/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/genius/Downloads/clion-2018.1.6/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/genius/Zing

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/genius/Zing/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Zing.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Zing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Zing.dir/flags.make

CMakeFiles/Zing.dir/main.cpp.o: CMakeFiles/Zing.dir/flags.make
CMakeFiles/Zing.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/genius/Zing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Zing.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Zing.dir/main.cpp.o -c /home/genius/Zing/main.cpp

CMakeFiles/Zing.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Zing.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/genius/Zing/main.cpp > CMakeFiles/Zing.dir/main.cpp.i

CMakeFiles/Zing.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Zing.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/genius/Zing/main.cpp -o CMakeFiles/Zing.dir/main.cpp.s

CMakeFiles/Zing.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/Zing.dir/main.cpp.o.requires

CMakeFiles/Zing.dir/main.cpp.o.provides: CMakeFiles/Zing.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Zing.dir/build.make CMakeFiles/Zing.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/Zing.dir/main.cpp.o.provides

CMakeFiles/Zing.dir/main.cpp.o.provides.build: CMakeFiles/Zing.dir/main.cpp.o


CMakeFiles/Zing.dir/util.cpp.o: CMakeFiles/Zing.dir/flags.make
CMakeFiles/Zing.dir/util.cpp.o: ../util.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/genius/Zing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Zing.dir/util.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Zing.dir/util.cpp.o -c /home/genius/Zing/util.cpp

CMakeFiles/Zing.dir/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Zing.dir/util.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/genius/Zing/util.cpp > CMakeFiles/Zing.dir/util.cpp.i

CMakeFiles/Zing.dir/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Zing.dir/util.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/genius/Zing/util.cpp -o CMakeFiles/Zing.dir/util.cpp.s

CMakeFiles/Zing.dir/util.cpp.o.requires:

.PHONY : CMakeFiles/Zing.dir/util.cpp.o.requires

CMakeFiles/Zing.dir/util.cpp.o.provides: CMakeFiles/Zing.dir/util.cpp.o.requires
	$(MAKE) -f CMakeFiles/Zing.dir/build.make CMakeFiles/Zing.dir/util.cpp.o.provides.build
.PHONY : CMakeFiles/Zing.dir/util.cpp.o.provides

CMakeFiles/Zing.dir/util.cpp.o.provides.build: CMakeFiles/Zing.dir/util.cpp.o


# Object files for target Zing
Zing_OBJECTS = \
"CMakeFiles/Zing.dir/main.cpp.o" \
"CMakeFiles/Zing.dir/util.cpp.o"

# External object files for target Zing
Zing_EXTERNAL_OBJECTS =

Zing: CMakeFiles/Zing.dir/main.cpp.o
Zing: CMakeFiles/Zing.dir/util.cpp.o
Zing: CMakeFiles/Zing.dir/build.make
Zing: CMakeFiles/Zing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/genius/Zing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable Zing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Zing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Zing.dir/build: Zing

.PHONY : CMakeFiles/Zing.dir/build

CMakeFiles/Zing.dir/requires: CMakeFiles/Zing.dir/main.cpp.o.requires
CMakeFiles/Zing.dir/requires: CMakeFiles/Zing.dir/util.cpp.o.requires

.PHONY : CMakeFiles/Zing.dir/requires

CMakeFiles/Zing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Zing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Zing.dir/clean

CMakeFiles/Zing.dir/depend:
	cd /home/genius/Zing/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/genius/Zing /home/genius/Zing /home/genius/Zing/cmake-build-debug /home/genius/Zing/cmake-build-debug /home/genius/Zing/cmake-build-debug/CMakeFiles/Zing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Zing.dir/depend


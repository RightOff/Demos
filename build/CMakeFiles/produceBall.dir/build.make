# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/clh/workspace/cppcode/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/clh/workspace/cppcode/test/build

# Include any dependencies generated for this target.
include CMakeFiles/produceBall.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/produceBall.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/produceBall.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/produceBall.dir/flags.make

CMakeFiles/produceBall.dir/produceBall.cpp.o: CMakeFiles/produceBall.dir/flags.make
CMakeFiles/produceBall.dir/produceBall.cpp.o: /home/clh/workspace/cppcode/test/produceBall.cpp
CMakeFiles/produceBall.dir/produceBall.cpp.o: CMakeFiles/produceBall.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/clh/workspace/cppcode/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/produceBall.dir/produceBall.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/produceBall.dir/produceBall.cpp.o -MF CMakeFiles/produceBall.dir/produceBall.cpp.o.d -o CMakeFiles/produceBall.dir/produceBall.cpp.o -c /home/clh/workspace/cppcode/test/produceBall.cpp

CMakeFiles/produceBall.dir/produceBall.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/produceBall.dir/produceBall.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/clh/workspace/cppcode/test/produceBall.cpp > CMakeFiles/produceBall.dir/produceBall.cpp.i

CMakeFiles/produceBall.dir/produceBall.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/produceBall.dir/produceBall.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/clh/workspace/cppcode/test/produceBall.cpp -o CMakeFiles/produceBall.dir/produceBall.cpp.s

# Object files for target produceBall
produceBall_OBJECTS = \
"CMakeFiles/produceBall.dir/produceBall.cpp.o"

# External object files for target produceBall
produceBall_EXTERNAL_OBJECTS =

produceBall: CMakeFiles/produceBall.dir/produceBall.cpp.o
produceBall: CMakeFiles/produceBall.dir/build.make
produceBall: CMakeFiles/produceBall.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/clh/workspace/cppcode/test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable produceBall"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/produceBall.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/produceBall.dir/build: produceBall
.PHONY : CMakeFiles/produceBall.dir/build

CMakeFiles/produceBall.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/produceBall.dir/cmake_clean.cmake
.PHONY : CMakeFiles/produceBall.dir/clean

CMakeFiles/produceBall.dir/depend:
	cd /home/clh/workspace/cppcode/test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/clh/workspace/cppcode/test /home/clh/workspace/cppcode/test /home/clh/workspace/cppcode/test/build /home/clh/workspace/cppcode/test/build /home/clh/workspace/cppcode/test/build/CMakeFiles/produceBall.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/produceBall.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

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
CMAKE_COMMAND = /usr/local/lib/python3.8/dist-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /usr/local/lib/python3.8/dist-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/clh/Demos/ONNX-Runtime-Inference/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/clh/Demos/ONNX-Runtime-Inference/src/build

# Include any dependencies generated for this target.
include CMakeFiles/inference.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/inference.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/inference.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/inference.dir/flags.make

CMakeFiles/inference.dir/inference.cpp.o: CMakeFiles/inference.dir/flags.make
CMakeFiles/inference.dir/inference.cpp.o: /root/clh/Demos/ONNX-Runtime-Inference/src/inference.cpp
CMakeFiles/inference.dir/inference.cpp.o: CMakeFiles/inference.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/clh/Demos/ONNX-Runtime-Inference/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/inference.dir/inference.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/inference.dir/inference.cpp.o -MF CMakeFiles/inference.dir/inference.cpp.o.d -o CMakeFiles/inference.dir/inference.cpp.o -c /root/clh/Demos/ONNX-Runtime-Inference/src/inference.cpp

CMakeFiles/inference.dir/inference.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/inference.dir/inference.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/clh/Demos/ONNX-Runtime-Inference/src/inference.cpp > CMakeFiles/inference.dir/inference.cpp.i

CMakeFiles/inference.dir/inference.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/inference.dir/inference.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/clh/Demos/ONNX-Runtime-Inference/src/inference.cpp -o CMakeFiles/inference.dir/inference.cpp.s

# Object files for target inference
inference_OBJECTS = \
"CMakeFiles/inference.dir/inference.cpp.o"

# External object files for target inference
inference_EXTERNAL_OBJECTS =

inference: CMakeFiles/inference.dir/inference.cpp.o
inference: CMakeFiles/inference.dir/build.make
inference: CMakeFiles/inference.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/clh/Demos/ONNX-Runtime-Inference/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable inference"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/inference.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/inference.dir/build: inference
.PHONY : CMakeFiles/inference.dir/build

CMakeFiles/inference.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/inference.dir/cmake_clean.cmake
.PHONY : CMakeFiles/inference.dir/clean

CMakeFiles/inference.dir/depend:
	cd /root/clh/Demos/ONNX-Runtime-Inference/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/clh/Demos/ONNX-Runtime-Inference/src /root/clh/Demos/ONNX-Runtime-Inference/src /root/clh/Demos/ONNX-Runtime-Inference/src/build /root/clh/Demos/ONNX-Runtime-Inference/src/build /root/clh/Demos/ONNX-Runtime-Inference/src/build/CMakeFiles/inference.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/inference.dir/depend


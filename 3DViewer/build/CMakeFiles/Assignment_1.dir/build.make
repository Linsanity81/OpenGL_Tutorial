# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer/build

# Include any dependencies generated for this target.
include CMakeFiles/Assignment_1.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Assignment_1.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Assignment_1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Assignment_1.dir/flags.make

CMakeFiles/Assignment_1.dir/src/main.cpp.o: CMakeFiles/Assignment_1.dir/flags.make
CMakeFiles/Assignment_1.dir/src/main.cpp.o: /Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer/src/main.cpp
CMakeFiles/Assignment_1.dir/src/main.cpp.o: CMakeFiles/Assignment_1.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Assignment_1.dir/src/main.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Assignment_1.dir/src/main.cpp.o -MF CMakeFiles/Assignment_1.dir/src/main.cpp.o.d -o CMakeFiles/Assignment_1.dir/src/main.cpp.o -c /Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer/src/main.cpp

CMakeFiles/Assignment_1.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Assignment_1.dir/src/main.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer/src/main.cpp > CMakeFiles/Assignment_1.dir/src/main.cpp.i

CMakeFiles/Assignment_1.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Assignment_1.dir/src/main.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer/src/main.cpp -o CMakeFiles/Assignment_1.dir/src/main.cpp.s

# Object files for target Assignment_1
Assignment_1_OBJECTS = \
"CMakeFiles/Assignment_1.dir/src/main.cpp.o"

# External object files for target Assignment_1
Assignment_1_EXTERNAL_OBJECTS =

Assignment_1: CMakeFiles/Assignment_1.dir/src/main.cpp.o
Assignment_1: CMakeFiles/Assignment_1.dir/build.make
Assignment_1: /opt/homebrew/lib/libglfw.dylib
Assignment_1: libGLAD.a
Assignment_1: /opt/homebrew/lib/libglfw.dylib
Assignment_1: CMakeFiles/Assignment_1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Assignment_1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Assignment_1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Assignment_1.dir/build: Assignment_1
.PHONY : CMakeFiles/Assignment_1.dir/build

CMakeFiles/Assignment_1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Assignment_1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Assignment_1.dir/clean

CMakeFiles/Assignment_1.dir/depend:
	cd /Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer /Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer /Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer/build /Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer/build /Users/linsanity/Documents/GitHub/OpenGL_Tutorial/3DViewer/build/CMakeFiles/Assignment_1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Assignment_1.dir/depend


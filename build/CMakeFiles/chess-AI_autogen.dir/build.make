# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nachiket/Documents/qt5-workspace/chess-AI

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nachiket/Documents/qt5-workspace/chess-AI/build

# Utility rule file for chess-AI_autogen.

# Include the progress variables for this target.
include CMakeFiles/chess-AI_autogen.dir/progress.make

CMakeFiles/chess-AI_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target chess-AI"
	/usr/bin/cmake -E cmake_autogen /home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles/chess-AI_autogen.dir/AutogenInfo.json Release

chess-AI_autogen: CMakeFiles/chess-AI_autogen
chess-AI_autogen: CMakeFiles/chess-AI_autogen.dir/build.make

.PHONY : chess-AI_autogen

# Rule to build all files generated by this target.
CMakeFiles/chess-AI_autogen.dir/build: chess-AI_autogen

.PHONY : CMakeFiles/chess-AI_autogen.dir/build

CMakeFiles/chess-AI_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/chess-AI_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/chess-AI_autogen.dir/clean

CMakeFiles/chess-AI_autogen.dir/depend:
	cd /home/nachiket/Documents/qt5-workspace/chess-AI/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nachiket/Documents/qt5-workspace/chess-AI /home/nachiket/Documents/qt5-workspace/chess-AI /home/nachiket/Documents/qt5-workspace/chess-AI/build /home/nachiket/Documents/qt5-workspace/chess-AI/build /home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles/chess-AI_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/chess-AI_autogen.dir/depend


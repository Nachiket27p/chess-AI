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
CMAKE_SOURCE_DIR = /home/nachiket/Documents/qt5-workspace/chess-AI

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nachiket/Documents/qt5-workspace/chess-AI/build

# Include any dependencies generated for this target.
include CMakeFiles/chess-AI.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/chess-AI.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/chess-AI.dir/flags.make

CMakeFiles/chess-AI.dir/chess-AI_autogen/mocs_compilation.cpp.o: CMakeFiles/chess-AI.dir/flags.make
CMakeFiles/chess-AI.dir/chess-AI_autogen/mocs_compilation.cpp.o: chess-AI_autogen/mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/chess-AI.dir/chess-AI_autogen/mocs_compilation.cpp.o"
	/bin/g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess-AI.dir/chess-AI_autogen/mocs_compilation.cpp.o -c /home/nachiket/Documents/qt5-workspace/chess-AI/build/chess-AI_autogen/mocs_compilation.cpp

CMakeFiles/chess-AI.dir/chess-AI_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess-AI.dir/chess-AI_autogen/mocs_compilation.cpp.i"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nachiket/Documents/qt5-workspace/chess-AI/build/chess-AI_autogen/mocs_compilation.cpp > CMakeFiles/chess-AI.dir/chess-AI_autogen/mocs_compilation.cpp.i

CMakeFiles/chess-AI.dir/chess-AI_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess-AI.dir/chess-AI_autogen/mocs_compilation.cpp.s"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nachiket/Documents/qt5-workspace/chess-AI/build/chess-AI_autogen/mocs_compilation.cpp -o CMakeFiles/chess-AI.dir/chess-AI_autogen/mocs_compilation.cpp.s

CMakeFiles/chess-AI.dir/main.cpp.o: CMakeFiles/chess-AI.dir/flags.make
CMakeFiles/chess-AI.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/chess-AI.dir/main.cpp.o"
	/bin/g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess-AI.dir/main.cpp.o -c /home/nachiket/Documents/qt5-workspace/chess-AI/main.cpp

CMakeFiles/chess-AI.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess-AI.dir/main.cpp.i"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nachiket/Documents/qt5-workspace/chess-AI/main.cpp > CMakeFiles/chess-AI.dir/main.cpp.i

CMakeFiles/chess-AI.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess-AI.dir/main.cpp.s"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nachiket/Documents/qt5-workspace/chess-AI/main.cpp -o CMakeFiles/chess-AI.dir/main.cpp.s

CMakeFiles/chess-AI.dir/board.cpp.o: CMakeFiles/chess-AI.dir/flags.make
CMakeFiles/chess-AI.dir/board.cpp.o: ../board.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/chess-AI.dir/board.cpp.o"
	/bin/g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess-AI.dir/board.cpp.o -c /home/nachiket/Documents/qt5-workspace/chess-AI/board.cpp

CMakeFiles/chess-AI.dir/board.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess-AI.dir/board.cpp.i"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nachiket/Documents/qt5-workspace/chess-AI/board.cpp > CMakeFiles/chess-AI.dir/board.cpp.i

CMakeFiles/chess-AI.dir/board.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess-AI.dir/board.cpp.s"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nachiket/Documents/qt5-workspace/chess-AI/board.cpp -o CMakeFiles/chess-AI.dir/board.cpp.s

CMakeFiles/chess-AI.dir/theme.cpp.o: CMakeFiles/chess-AI.dir/flags.make
CMakeFiles/chess-AI.dir/theme.cpp.o: ../theme.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/chess-AI.dir/theme.cpp.o"
	/bin/g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess-AI.dir/theme.cpp.o -c /home/nachiket/Documents/qt5-workspace/chess-AI/theme.cpp

CMakeFiles/chess-AI.dir/theme.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess-AI.dir/theme.cpp.i"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nachiket/Documents/qt5-workspace/chess-AI/theme.cpp > CMakeFiles/chess-AI.dir/theme.cpp.i

CMakeFiles/chess-AI.dir/theme.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess-AI.dir/theme.cpp.s"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nachiket/Documents/qt5-workspace/chess-AI/theme.cpp -o CMakeFiles/chess-AI.dir/theme.cpp.s

CMakeFiles/chess-AI.dir/boardtile.cpp.o: CMakeFiles/chess-AI.dir/flags.make
CMakeFiles/chess-AI.dir/boardtile.cpp.o: ../boardtile.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/chess-AI.dir/boardtile.cpp.o"
	/bin/g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess-AI.dir/boardtile.cpp.o -c /home/nachiket/Documents/qt5-workspace/chess-AI/boardtile.cpp

CMakeFiles/chess-AI.dir/boardtile.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess-AI.dir/boardtile.cpp.i"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nachiket/Documents/qt5-workspace/chess-AI/boardtile.cpp > CMakeFiles/chess-AI.dir/boardtile.cpp.i

CMakeFiles/chess-AI.dir/boardtile.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess-AI.dir/boardtile.cpp.s"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nachiket/Documents/qt5-workspace/chess-AI/boardtile.cpp -o CMakeFiles/chess-AI.dir/boardtile.cpp.s

CMakeFiles/chess-AI.dir/rules.cpp.o: CMakeFiles/chess-AI.dir/flags.make
CMakeFiles/chess-AI.dir/rules.cpp.o: ../rules.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/chess-AI.dir/rules.cpp.o"
	/bin/g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess-AI.dir/rules.cpp.o -c /home/nachiket/Documents/qt5-workspace/chess-AI/rules.cpp

CMakeFiles/chess-AI.dir/rules.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess-AI.dir/rules.cpp.i"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nachiket/Documents/qt5-workspace/chess-AI/rules.cpp > CMakeFiles/chess-AI.dir/rules.cpp.i

CMakeFiles/chess-AI.dir/rules.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess-AI.dir/rules.cpp.s"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nachiket/Documents/qt5-workspace/chess-AI/rules.cpp -o CMakeFiles/chess-AI.dir/rules.cpp.s

CMakeFiles/chess-AI.dir/piece.cpp.o: CMakeFiles/chess-AI.dir/flags.make
CMakeFiles/chess-AI.dir/piece.cpp.o: ../piece.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/chess-AI.dir/piece.cpp.o"
	/bin/g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess-AI.dir/piece.cpp.o -c /home/nachiket/Documents/qt5-workspace/chess-AI/piece.cpp

CMakeFiles/chess-AI.dir/piece.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess-AI.dir/piece.cpp.i"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nachiket/Documents/qt5-workspace/chess-AI/piece.cpp > CMakeFiles/chess-AI.dir/piece.cpp.i

CMakeFiles/chess-AI.dir/piece.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess-AI.dir/piece.cpp.s"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nachiket/Documents/qt5-workspace/chess-AI/piece.cpp -o CMakeFiles/chess-AI.dir/piece.cpp.s

CMakeFiles/chess-AI.dir/debugwindow.cpp.o: CMakeFiles/chess-AI.dir/flags.make
CMakeFiles/chess-AI.dir/debugwindow.cpp.o: ../debugwindow.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/chess-AI.dir/debugwindow.cpp.o"
	/bin/g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess-AI.dir/debugwindow.cpp.o -c /home/nachiket/Documents/qt5-workspace/chess-AI/debugwindow.cpp

CMakeFiles/chess-AI.dir/debugwindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess-AI.dir/debugwindow.cpp.i"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nachiket/Documents/qt5-workspace/chess-AI/debugwindow.cpp > CMakeFiles/chess-AI.dir/debugwindow.cpp.i

CMakeFiles/chess-AI.dir/debugwindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess-AI.dir/debugwindow.cpp.s"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nachiket/Documents/qt5-workspace/chess-AI/debugwindow.cpp -o CMakeFiles/chess-AI.dir/debugwindow.cpp.s

CMakeFiles/chess-AI.dir/minmaxabp.cpp.o: CMakeFiles/chess-AI.dir/flags.make
CMakeFiles/chess-AI.dir/minmaxabp.cpp.o: ../minmaxabp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/chess-AI.dir/minmaxabp.cpp.o"
	/bin/g++-9  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/chess-AI.dir/minmaxabp.cpp.o -c /home/nachiket/Documents/qt5-workspace/chess-AI/minmaxabp.cpp

CMakeFiles/chess-AI.dir/minmaxabp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chess-AI.dir/minmaxabp.cpp.i"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nachiket/Documents/qt5-workspace/chess-AI/minmaxabp.cpp > CMakeFiles/chess-AI.dir/minmaxabp.cpp.i

CMakeFiles/chess-AI.dir/minmaxabp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chess-AI.dir/minmaxabp.cpp.s"
	/bin/g++-9 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nachiket/Documents/qt5-workspace/chess-AI/minmaxabp.cpp -o CMakeFiles/chess-AI.dir/minmaxabp.cpp.s

# Object files for target chess-AI
chess__AI_OBJECTS = \
"CMakeFiles/chess-AI.dir/chess-AI_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/chess-AI.dir/main.cpp.o" \
"CMakeFiles/chess-AI.dir/board.cpp.o" \
"CMakeFiles/chess-AI.dir/theme.cpp.o" \
"CMakeFiles/chess-AI.dir/boardtile.cpp.o" \
"CMakeFiles/chess-AI.dir/rules.cpp.o" \
"CMakeFiles/chess-AI.dir/piece.cpp.o" \
"CMakeFiles/chess-AI.dir/debugwindow.cpp.o" \
"CMakeFiles/chess-AI.dir/minmaxabp.cpp.o"

# External object files for target chess-AI
chess__AI_EXTERNAL_OBJECTS =

chess-AI: CMakeFiles/chess-AI.dir/chess-AI_autogen/mocs_compilation.cpp.o
chess-AI: CMakeFiles/chess-AI.dir/main.cpp.o
chess-AI: CMakeFiles/chess-AI.dir/board.cpp.o
chess-AI: CMakeFiles/chess-AI.dir/theme.cpp.o
chess-AI: CMakeFiles/chess-AI.dir/boardtile.cpp.o
chess-AI: CMakeFiles/chess-AI.dir/rules.cpp.o
chess-AI: CMakeFiles/chess-AI.dir/piece.cpp.o
chess-AI: CMakeFiles/chess-AI.dir/debugwindow.cpp.o
chess-AI: CMakeFiles/chess-AI.dir/minmaxabp.cpp.o
chess-AI: CMakeFiles/chess-AI.dir/build.make
chess-AI: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.12.8
chess-AI: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.12.8
chess-AI: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.12.8
chess-AI: CMakeFiles/chess-AI.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable chess-AI"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chess-AI.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/chess-AI.dir/build: chess-AI

.PHONY : CMakeFiles/chess-AI.dir/build

CMakeFiles/chess-AI.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/chess-AI.dir/cmake_clean.cmake
.PHONY : CMakeFiles/chess-AI.dir/clean

CMakeFiles/chess-AI.dir/depend:
	cd /home/nachiket/Documents/qt5-workspace/chess-AI/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nachiket/Documents/qt5-workspace/chess-AI /home/nachiket/Documents/qt5-workspace/chess-AI /home/nachiket/Documents/qt5-workspace/chess-AI/build /home/nachiket/Documents/qt5-workspace/chess-AI/build /home/nachiket/Documents/qt5-workspace/chess-AI/build/CMakeFiles/chess-AI.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/chess-AI.dir/depend


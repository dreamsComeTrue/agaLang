# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /home/squall/projects/agaLang

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/squall/projects/agaLang/build

# Include any dependencies generated for this target.
include CMakeFiles/agalang.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/agalang.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/agalang.dir/flags.make

CMakeFiles/agalang.dir/src/agaCompiler.cpp.o: CMakeFiles/agalang.dir/flags.make
CMakeFiles/agalang.dir/src/agaCompiler.cpp.o: ../src/agaCompiler.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/squall/projects/agaLang/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/agalang.dir/src/agaCompiler.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/agalang.dir/src/agaCompiler.cpp.o -c /home/squall/projects/agaLang/src/agaCompiler.cpp

CMakeFiles/agalang.dir/src/agaCompiler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/agalang.dir/src/agaCompiler.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/squall/projects/agaLang/src/agaCompiler.cpp > CMakeFiles/agalang.dir/src/agaCompiler.cpp.i

CMakeFiles/agalang.dir/src/agaCompiler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/agalang.dir/src/agaCompiler.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/squall/projects/agaLang/src/agaCompiler.cpp -o CMakeFiles/agalang.dir/src/agaCompiler.cpp.s

CMakeFiles/agalang.dir/src/agaCompiler.cpp.o.requires:
.PHONY : CMakeFiles/agalang.dir/src/agaCompiler.cpp.o.requires

CMakeFiles/agalang.dir/src/agaCompiler.cpp.o.provides: CMakeFiles/agalang.dir/src/agaCompiler.cpp.o.requires
	$(MAKE) -f CMakeFiles/agalang.dir/build.make CMakeFiles/agalang.dir/src/agaCompiler.cpp.o.provides.build
.PHONY : CMakeFiles/agalang.dir/src/agaCompiler.cpp.o.provides

CMakeFiles/agalang.dir/src/agaCompiler.cpp.o.provides.build: CMakeFiles/agalang.dir/src/agaCompiler.cpp.o

CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o: CMakeFiles/agalang.dir/flags.make
CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o: ../src/agaCodeGenerator.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/squall/projects/agaLang/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o -c /home/squall/projects/agaLang/src/agaCodeGenerator.cpp

CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/squall/projects/agaLang/src/agaCodeGenerator.cpp > CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.i

CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/squall/projects/agaLang/src/agaCodeGenerator.cpp -o CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.s

CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o.requires:
.PHONY : CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o.requires

CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o.provides: CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o.requires
	$(MAKE) -f CMakeFiles/agalang.dir/build.make CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o.provides.build
.PHONY : CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o.provides

CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o.provides.build: CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o

CMakeFiles/agalang.dir/src/agaParser.cpp.o: CMakeFiles/agalang.dir/flags.make
CMakeFiles/agalang.dir/src/agaParser.cpp.o: ../src/agaParser.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/squall/projects/agaLang/build/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/agalang.dir/src/agaParser.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/agalang.dir/src/agaParser.cpp.o -c /home/squall/projects/agaLang/src/agaParser.cpp

CMakeFiles/agalang.dir/src/agaParser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/agalang.dir/src/agaParser.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/squall/projects/agaLang/src/agaParser.cpp > CMakeFiles/agalang.dir/src/agaParser.cpp.i

CMakeFiles/agalang.dir/src/agaParser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/agalang.dir/src/agaParser.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/squall/projects/agaLang/src/agaParser.cpp -o CMakeFiles/agalang.dir/src/agaParser.cpp.s

CMakeFiles/agalang.dir/src/agaParser.cpp.o.requires:
.PHONY : CMakeFiles/agalang.dir/src/agaParser.cpp.o.requires

CMakeFiles/agalang.dir/src/agaParser.cpp.o.provides: CMakeFiles/agalang.dir/src/agaParser.cpp.o.requires
	$(MAKE) -f CMakeFiles/agalang.dir/build.make CMakeFiles/agalang.dir/src/agaParser.cpp.o.provides.build
.PHONY : CMakeFiles/agalang.dir/src/agaParser.cpp.o.provides

CMakeFiles/agalang.dir/src/agaParser.cpp.o.provides.build: CMakeFiles/agalang.dir/src/agaParser.cpp.o

CMakeFiles/agalang.dir/src/agaToken.cpp.o: CMakeFiles/agalang.dir/flags.make
CMakeFiles/agalang.dir/src/agaToken.cpp.o: ../src/agaToken.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/squall/projects/agaLang/build/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/agalang.dir/src/agaToken.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/agalang.dir/src/agaToken.cpp.o -c /home/squall/projects/agaLang/src/agaToken.cpp

CMakeFiles/agalang.dir/src/agaToken.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/agalang.dir/src/agaToken.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/squall/projects/agaLang/src/agaToken.cpp > CMakeFiles/agalang.dir/src/agaToken.cpp.i

CMakeFiles/agalang.dir/src/agaToken.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/agalang.dir/src/agaToken.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/squall/projects/agaLang/src/agaToken.cpp -o CMakeFiles/agalang.dir/src/agaToken.cpp.s

CMakeFiles/agalang.dir/src/agaToken.cpp.o.requires:
.PHONY : CMakeFiles/agalang.dir/src/agaToken.cpp.o.requires

CMakeFiles/agalang.dir/src/agaToken.cpp.o.provides: CMakeFiles/agalang.dir/src/agaToken.cpp.o.requires
	$(MAKE) -f CMakeFiles/agalang.dir/build.make CMakeFiles/agalang.dir/src/agaToken.cpp.o.provides.build
.PHONY : CMakeFiles/agalang.dir/src/agaToken.cpp.o.provides

CMakeFiles/agalang.dir/src/agaToken.cpp.o.provides.build: CMakeFiles/agalang.dir/src/agaToken.cpp.o

CMakeFiles/agalang.dir/src/agaException.cpp.o: CMakeFiles/agalang.dir/flags.make
CMakeFiles/agalang.dir/src/agaException.cpp.o: ../src/agaException.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/squall/projects/agaLang/build/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/agalang.dir/src/agaException.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/agalang.dir/src/agaException.cpp.o -c /home/squall/projects/agaLang/src/agaException.cpp

CMakeFiles/agalang.dir/src/agaException.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/agalang.dir/src/agaException.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/squall/projects/agaLang/src/agaException.cpp > CMakeFiles/agalang.dir/src/agaException.cpp.i

CMakeFiles/agalang.dir/src/agaException.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/agalang.dir/src/agaException.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/squall/projects/agaLang/src/agaException.cpp -o CMakeFiles/agalang.dir/src/agaException.cpp.s

CMakeFiles/agalang.dir/src/agaException.cpp.o.requires:
.PHONY : CMakeFiles/agalang.dir/src/agaException.cpp.o.requires

CMakeFiles/agalang.dir/src/agaException.cpp.o.provides: CMakeFiles/agalang.dir/src/agaException.cpp.o.requires
	$(MAKE) -f CMakeFiles/agalang.dir/build.make CMakeFiles/agalang.dir/src/agaException.cpp.o.provides.build
.PHONY : CMakeFiles/agalang.dir/src/agaException.cpp.o.provides

CMakeFiles/agalang.dir/src/agaException.cpp.o.provides.build: CMakeFiles/agalang.dir/src/agaException.cpp.o

CMakeFiles/agalang.dir/src/agaLogger.cpp.o: CMakeFiles/agalang.dir/flags.make
CMakeFiles/agalang.dir/src/agaLogger.cpp.o: ../src/agaLogger.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/squall/projects/agaLang/build/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/agalang.dir/src/agaLogger.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/agalang.dir/src/agaLogger.cpp.o -c /home/squall/projects/agaLang/src/agaLogger.cpp

CMakeFiles/agalang.dir/src/agaLogger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/agalang.dir/src/agaLogger.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/squall/projects/agaLang/src/agaLogger.cpp > CMakeFiles/agalang.dir/src/agaLogger.cpp.i

CMakeFiles/agalang.dir/src/agaLogger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/agalang.dir/src/agaLogger.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/squall/projects/agaLang/src/agaLogger.cpp -o CMakeFiles/agalang.dir/src/agaLogger.cpp.s

CMakeFiles/agalang.dir/src/agaLogger.cpp.o.requires:
.PHONY : CMakeFiles/agalang.dir/src/agaLogger.cpp.o.requires

CMakeFiles/agalang.dir/src/agaLogger.cpp.o.provides: CMakeFiles/agalang.dir/src/agaLogger.cpp.o.requires
	$(MAKE) -f CMakeFiles/agalang.dir/build.make CMakeFiles/agalang.dir/src/agaLogger.cpp.o.provides.build
.PHONY : CMakeFiles/agalang.dir/src/agaLogger.cpp.o.provides

CMakeFiles/agalang.dir/src/agaLogger.cpp.o.provides.build: CMakeFiles/agalang.dir/src/agaLogger.cpp.o

CMakeFiles/agalang.dir/src/agaLexer.cpp.o: CMakeFiles/agalang.dir/flags.make
CMakeFiles/agalang.dir/src/agaLexer.cpp.o: ../src/agaLexer.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/squall/projects/agaLang/build/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/agalang.dir/src/agaLexer.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/agalang.dir/src/agaLexer.cpp.o -c /home/squall/projects/agaLang/src/agaLexer.cpp

CMakeFiles/agalang.dir/src/agaLexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/agalang.dir/src/agaLexer.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/squall/projects/agaLang/src/agaLexer.cpp > CMakeFiles/agalang.dir/src/agaLexer.cpp.i

CMakeFiles/agalang.dir/src/agaLexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/agalang.dir/src/agaLexer.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/squall/projects/agaLang/src/agaLexer.cpp -o CMakeFiles/agalang.dir/src/agaLexer.cpp.s

CMakeFiles/agalang.dir/src/agaLexer.cpp.o.requires:
.PHONY : CMakeFiles/agalang.dir/src/agaLexer.cpp.o.requires

CMakeFiles/agalang.dir/src/agaLexer.cpp.o.provides: CMakeFiles/agalang.dir/src/agaLexer.cpp.o.requires
	$(MAKE) -f CMakeFiles/agalang.dir/build.make CMakeFiles/agalang.dir/src/agaLexer.cpp.o.provides.build
.PHONY : CMakeFiles/agalang.dir/src/agaLexer.cpp.o.provides

CMakeFiles/agalang.dir/src/agaLexer.cpp.o.provides.build: CMakeFiles/agalang.dir/src/agaLexer.cpp.o

CMakeFiles/agalang.dir/src/agaMain.cpp.o: CMakeFiles/agalang.dir/flags.make
CMakeFiles/agalang.dir/src/agaMain.cpp.o: ../src/agaMain.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/squall/projects/agaLang/build/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/agalang.dir/src/agaMain.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/agalang.dir/src/agaMain.cpp.o -c /home/squall/projects/agaLang/src/agaMain.cpp

CMakeFiles/agalang.dir/src/agaMain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/agalang.dir/src/agaMain.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/squall/projects/agaLang/src/agaMain.cpp > CMakeFiles/agalang.dir/src/agaMain.cpp.i

CMakeFiles/agalang.dir/src/agaMain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/agalang.dir/src/agaMain.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/squall/projects/agaLang/src/agaMain.cpp -o CMakeFiles/agalang.dir/src/agaMain.cpp.s

CMakeFiles/agalang.dir/src/agaMain.cpp.o.requires:
.PHONY : CMakeFiles/agalang.dir/src/agaMain.cpp.o.requires

CMakeFiles/agalang.dir/src/agaMain.cpp.o.provides: CMakeFiles/agalang.dir/src/agaMain.cpp.o.requires
	$(MAKE) -f CMakeFiles/agalang.dir/build.make CMakeFiles/agalang.dir/src/agaMain.cpp.o.provides.build
.PHONY : CMakeFiles/agalang.dir/src/agaMain.cpp.o.provides

CMakeFiles/agalang.dir/src/agaMain.cpp.o.provides.build: CMakeFiles/agalang.dir/src/agaMain.cpp.o

# Object files for target agalang
agalang_OBJECTS = \
"CMakeFiles/agalang.dir/src/agaCompiler.cpp.o" \
"CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o" \
"CMakeFiles/agalang.dir/src/agaParser.cpp.o" \
"CMakeFiles/agalang.dir/src/agaToken.cpp.o" \
"CMakeFiles/agalang.dir/src/agaException.cpp.o" \
"CMakeFiles/agalang.dir/src/agaLogger.cpp.o" \
"CMakeFiles/agalang.dir/src/agaLexer.cpp.o" \
"CMakeFiles/agalang.dir/src/agaMain.cpp.o"

# External object files for target agalang
agalang_EXTERNAL_OBJECTS =

agalang: CMakeFiles/agalang.dir/src/agaCompiler.cpp.o
agalang: CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o
agalang: CMakeFiles/agalang.dir/src/agaParser.cpp.o
agalang: CMakeFiles/agalang.dir/src/agaToken.cpp.o
agalang: CMakeFiles/agalang.dir/src/agaException.cpp.o
agalang: CMakeFiles/agalang.dir/src/agaLogger.cpp.o
agalang: CMakeFiles/agalang.dir/src/agaLexer.cpp.o
agalang: CMakeFiles/agalang.dir/src/agaMain.cpp.o
agalang: CMakeFiles/agalang.dir/build.make
agalang: CMakeFiles/agalang.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable agalang"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/agalang.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/agalang.dir/build: agalang
.PHONY : CMakeFiles/agalang.dir/build

CMakeFiles/agalang.dir/requires: CMakeFiles/agalang.dir/src/agaCompiler.cpp.o.requires
CMakeFiles/agalang.dir/requires: CMakeFiles/agalang.dir/src/agaCodeGenerator.cpp.o.requires
CMakeFiles/agalang.dir/requires: CMakeFiles/agalang.dir/src/agaParser.cpp.o.requires
CMakeFiles/agalang.dir/requires: CMakeFiles/agalang.dir/src/agaToken.cpp.o.requires
CMakeFiles/agalang.dir/requires: CMakeFiles/agalang.dir/src/agaException.cpp.o.requires
CMakeFiles/agalang.dir/requires: CMakeFiles/agalang.dir/src/agaLogger.cpp.o.requires
CMakeFiles/agalang.dir/requires: CMakeFiles/agalang.dir/src/agaLexer.cpp.o.requires
CMakeFiles/agalang.dir/requires: CMakeFiles/agalang.dir/src/agaMain.cpp.o.requires
.PHONY : CMakeFiles/agalang.dir/requires

CMakeFiles/agalang.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/agalang.dir/cmake_clean.cmake
.PHONY : CMakeFiles/agalang.dir/clean

CMakeFiles/agalang.dir/depend:
	cd /home/squall/projects/agaLang/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/squall/projects/agaLang /home/squall/projects/agaLang /home/squall/projects/agaLang/build /home/squall/projects/agaLang/build /home/squall/projects/agaLang/build/CMakeFiles/agalang.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/agalang.dir/depend

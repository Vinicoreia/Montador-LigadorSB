# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.7

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2017.1.1\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2017.1.1\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\Users\x04618860146\Desktop\Vinicius\Montador-LigadorSB

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\Users\x04618860146\Desktop\Vinicius\Montador-LigadorSB\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/ligador.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ligador.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ligador.dir/flags.make

CMakeFiles/ligador.dir/ligador.cpp.obj: CMakeFiles/ligador.dir/flags.make
CMakeFiles/ligador.dir/ligador.cpp.obj: ../ligador.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\Users\x04618860146\Desktop\Vinicius\Montador-LigadorSB\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ligador.dir/ligador.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\ligador.dir\ligador.cpp.obj -c D:\Users\x04618860146\Desktop\Vinicius\Montador-LigadorSB\ligador.cpp

CMakeFiles/ligador.dir/ligador.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ligador.dir/ligador.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E D:\Users\x04618860146\Desktop\Vinicius\Montador-LigadorSB\ligador.cpp > CMakeFiles\ligador.dir\ligador.cpp.i

CMakeFiles/ligador.dir/ligador.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ligador.dir/ligador.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S D:\Users\x04618860146\Desktop\Vinicius\Montador-LigadorSB\ligador.cpp -o CMakeFiles\ligador.dir\ligador.cpp.s

CMakeFiles/ligador.dir/ligador.cpp.obj.requires:

.PHONY : CMakeFiles/ligador.dir/ligador.cpp.obj.requires

CMakeFiles/ligador.dir/ligador.cpp.obj.provides: CMakeFiles/ligador.dir/ligador.cpp.obj.requires
	$(MAKE) -f CMakeFiles\ligador.dir\build.make CMakeFiles/ligador.dir/ligador.cpp.obj.provides.build
.PHONY : CMakeFiles/ligador.dir/ligador.cpp.obj.provides

CMakeFiles/ligador.dir/ligador.cpp.obj.provides.build: CMakeFiles/ligador.dir/ligador.cpp.obj


# Object files for target ligador
ligador_OBJECTS = \
"CMakeFiles/ligador.dir/ligador.cpp.obj"

# External object files for target ligador
ligador_EXTERNAL_OBJECTS =

ligador.exe: CMakeFiles/ligador.dir/ligador.cpp.obj
ligador.exe: CMakeFiles/ligador.dir/build.make
ligador.exe: CMakeFiles/ligador.dir/linklibs.rsp
ligador.exe: CMakeFiles/ligador.dir/objects1.rsp
ligador.exe: CMakeFiles/ligador.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\Users\x04618860146\Desktop\Vinicius\Montador-LigadorSB\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ligador.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\ligador.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ligador.dir/build: ligador.exe

.PHONY : CMakeFiles/ligador.dir/build

CMakeFiles/ligador.dir/requires: CMakeFiles/ligador.dir/ligador.cpp.obj.requires

.PHONY : CMakeFiles/ligador.dir/requires

CMakeFiles/ligador.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\ligador.dir\cmake_clean.cmake
.PHONY : CMakeFiles/ligador.dir/clean

CMakeFiles/ligador.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\Users\x04618860146\Desktop\Vinicius\Montador-LigadorSB D:\Users\x04618860146\Desktop\Vinicius\Montador-LigadorSB D:\Users\x04618860146\Desktop\Vinicius\Montador-LigadorSB\cmake-build-debug D:\Users\x04618860146\Desktop\Vinicius\Montador-LigadorSB\cmake-build-debug D:\Users\x04618860146\Desktop\Vinicius\Montador-LigadorSB\cmake-build-debug\CMakeFiles\ligador.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ligador.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/ricanontherun/Code/telemetry-framework

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ricanontherun/Code/telemetry-framework/aggregator

# Include any dependencies generated for this target.
include aggregator/CMakeFiles/aggregator.dir/depend.make

# Include the progress variables for this target.
include aggregator/CMakeFiles/aggregator.dir/progress.make

# Include the compile flags for this target's objects.
include aggregator/CMakeFiles/aggregator.dir/flags.make

aggregator/CMakeFiles/aggregator.dir/main.cpp.o: aggregator/CMakeFiles/aggregator.dir/flags.make
aggregator/CMakeFiles/aggregator.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ricanontherun/Code/telemetry-framework/aggregator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object aggregator/CMakeFiles/aggregator.dir/main.cpp.o"
	cd /home/ricanontherun/Code/telemetry-framework/aggregator/aggregator && clang++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/aggregator.dir/main.cpp.o -c /home/ricanontherun/Code/telemetry-framework/aggregator/main.cpp

aggregator/CMakeFiles/aggregator.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/aggregator.dir/main.cpp.i"
	cd /home/ricanontherun/Code/telemetry-framework/aggregator/aggregator && clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ricanontherun/Code/telemetry-framework/aggregator/main.cpp > CMakeFiles/aggregator.dir/main.cpp.i

aggregator/CMakeFiles/aggregator.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/aggregator.dir/main.cpp.s"
	cd /home/ricanontherun/Code/telemetry-framework/aggregator/aggregator && clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ricanontherun/Code/telemetry-framework/aggregator/main.cpp -o CMakeFiles/aggregator.dir/main.cpp.s

aggregator/CMakeFiles/aggregator.dir/main.cpp.o.requires:

.PHONY : aggregator/CMakeFiles/aggregator.dir/main.cpp.o.requires

aggregator/CMakeFiles/aggregator.dir/main.cpp.o.provides: aggregator/CMakeFiles/aggregator.dir/main.cpp.o.requires
	$(MAKE) -f aggregator/CMakeFiles/aggregator.dir/build.make aggregator/CMakeFiles/aggregator.dir/main.cpp.o.provides.build
.PHONY : aggregator/CMakeFiles/aggregator.dir/main.cpp.o.provides

aggregator/CMakeFiles/aggregator.dir/main.cpp.o.provides.build: aggregator/CMakeFiles/aggregator.dir/main.cpp.o


# Object files for target aggregator
aggregator_OBJECTS = \
"CMakeFiles/aggregator.dir/main.cpp.o"

# External object files for target aggregator
aggregator_EXTERNAL_OBJECTS =

aggregator/bin/aggregator: aggregator/CMakeFiles/aggregator.dir/main.cpp.o
aggregator/bin/aggregator: aggregator/CMakeFiles/aggregator.dir/build.make
aggregator/bin/aggregator: /usr/local/lib/libzmq.so
aggregator/bin/aggregator: aggregator/CMakeFiles/aggregator.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ricanontherun/Code/telemetry-framework/aggregator/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/aggregator"
	cd /home/ricanontherun/Code/telemetry-framework/aggregator/aggregator && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/aggregator.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
aggregator/CMakeFiles/aggregator.dir/build: aggregator/bin/aggregator

.PHONY : aggregator/CMakeFiles/aggregator.dir/build

aggregator/CMakeFiles/aggregator.dir/requires: aggregator/CMakeFiles/aggregator.dir/main.cpp.o.requires

.PHONY : aggregator/CMakeFiles/aggregator.dir/requires

aggregator/CMakeFiles/aggregator.dir/clean:
	cd /home/ricanontherun/Code/telemetry-framework/aggregator/aggregator && $(CMAKE_COMMAND) -P CMakeFiles/aggregator.dir/cmake_clean.cmake
.PHONY : aggregator/CMakeFiles/aggregator.dir/clean

aggregator/CMakeFiles/aggregator.dir/depend:
	cd /home/ricanontherun/Code/telemetry-framework/aggregator && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ricanontherun/Code/telemetry-framework /home/ricanontherun/Code/telemetry-framework/aggregator /home/ricanontherun/Code/telemetry-framework/aggregator /home/ricanontherun/Code/telemetry-framework/aggregator/aggregator /home/ricanontherun/Code/telemetry-framework/aggregator/aggregator/CMakeFiles/aggregator.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : aggregator/CMakeFiles/aggregator.dir/depend


# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/anyonepaw/CLionProjects/ns3-iot-realisation

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/anyonepaw/CLionProjects/ns3-iot-realisation

# Include any dependencies generated for this target.
include src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/depend.make

# Include the progress variables for this target.
include src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/progress.make

# Include the compile flags for this target's objects.
include src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/flags.make

src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/model/virtual-net-device.cc.o: src/virtual-net-device/model/virtual-net-device.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/anyonepaw/CLionProjects/ns3-iot-realisation/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/model/virtual-net-device.cc.o"
	cd /Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/virtual-net-device && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/model/virtual-net-device.cc.o -c /Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/virtual-net-device/model/virtual-net-device.cc

src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/model/virtual-net-device.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/model/virtual-net-device.cc.i"
	cd /Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/virtual-net-device && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/virtual-net-device/model/virtual-net-device.cc > CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/model/virtual-net-device.cc.i

src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/model/virtual-net-device.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/model/virtual-net-device.cc.s"
	cd /Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/virtual-net-device && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/virtual-net-device/model/virtual-net-device.cc -o CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/model/virtual-net-device.cc.s

ns3.29-virtual-net-device-debug-obj: src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/model/virtual-net-device.cc.o

.PHONY : ns3.29-virtual-net-device-debug-obj

# Rule to build all files generated by this target.
src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/build: ns3.29-virtual-net-device-debug-obj

.PHONY : src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/build

src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/clean:
	cd /Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/virtual-net-device && $(CMAKE_COMMAND) -P CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/cmake_clean.cmake
.PHONY : src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/clean

src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/depend:
	cd /Users/anyonepaw/CLionProjects/ns3-iot-realisation && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/anyonepaw/CLionProjects/ns3-iot-realisation /Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/virtual-net-device /Users/anyonepaw/CLionProjects/ns3-iot-realisation /Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/virtual-net-device /Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/virtual-net-device/CMakeFiles/ns3.29-virtual-net-device-debug-obj.dir/depend


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
include scratch/CMakeFiles/test.dir/depend.make

# Include the progress variables for this target.
include scratch/CMakeFiles/test.dir/progress.make

# Include the compile flags for this target's objects.
include scratch/CMakeFiles/test.dir/flags.make

scratch/CMakeFiles/test.dir/test.cc.o: scratch/test.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/anyonepaw/CLionProjects/ns3-iot-realisation/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object scratch/CMakeFiles/test.dir/test.cc.o"
	cd /Users/anyonepaw/CLionProjects/ns3-iot-realisation/scratch && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test.dir/test.cc.o -c /Users/anyonepaw/CLionProjects/ns3-iot-realisation/scratch/test.cc

scratch/CMakeFiles/test.dir/test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test.dir/test.cc.i"
	cd /Users/anyonepaw/CLionProjects/ns3-iot-realisation/scratch && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/anyonepaw/CLionProjects/ns3-iot-realisation/scratch/test.cc > CMakeFiles/test.dir/test.cc.i

scratch/CMakeFiles/test.dir/test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test.dir/test.cc.s"
	cd /Users/anyonepaw/CLionProjects/ns3-iot-realisation/scratch && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/anyonepaw/CLionProjects/ns3-iot-realisation/scratch/test.cc -o CMakeFiles/test.dir/test.cc.s

# Object files for target test
test_OBJECTS = \
"CMakeFiles/test.dir/test.cc.o"

# External object files for target test
test_EXTERNAL_OBJECTS = \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/csma-system-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tc/fq-codel-queue-disc-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tc/pfifo-fast-queue-disc-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-cwnd-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-interop-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-loss-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-no-delay-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-socket-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-state-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/nsctcp-loss-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-socket-writer.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3wifi/wifi-interference-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3wifi/wifi-msdu-aggregator-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/ns3wifi/wifi-ac-mapping-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/traced/traced-callback-typedef-test-suite.cc.o" \
"/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/CMakeFiles/ns3.29-test-debug.dir/traced/traced-value-callback-typedef-test-suite.cc.o"

build/bin/test: scratch/CMakeFiles/test.dir/test.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/csma-system-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tc/fq-codel-queue-disc-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tc/pfifo-fast-queue-disc-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-cwnd-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-interop-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-loss-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-no-delay-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-socket-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-state-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/nsctcp-loss-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3tcp/ns3tcp-socket-writer.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3wifi/wifi-interference-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3wifi/wifi-msdu-aggregator-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/ns3wifi/wifi-ac-mapping-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/traced/traced-callback-typedef-test-suite.cc.o
build/bin/test: src/test/CMakeFiles/ns3.29-test-debug.dir/traced/traced-value-callback-typedef-test-suite.cc.o
build/bin/test: build/lib/libns3.29-aodv-debug.dylib
build/bin/test: build/lib/libns3.29-brite-debug.dylib
build/bin/test: build/lib/libns3.29-click-debug.dylib
build/bin/test: build/lib/libns3.29-csma-layout-debug.dylib
build/bin/test: build/lib/libns3.29-dsdv-debug.dylib
build/bin/test: build/lib/libns3.29-dsr-debug.dylib
build/bin/test: build/lib/libns3.29-flow-monitor-debug.dylib
build/bin/test: build/lib/libns3.29-internet-apps-debug.dylib
build/bin/test: build/lib/libns3.29-mesh-debug.dylib
build/bin/test: build/lib/libns3.29-netanim-debug.dylib
build/bin/test: build/lib/libns3.29-nix-vector-routing-debug.dylib
build/bin/test: build/lib/libns3.29-olsr-debug.dylib
build/bin/test: build/lib/libns3.29-point-to-point-layout-debug.dylib
build/bin/test: build/lib/libns3.29-sixlowpan-debug.dylib
build/bin/test: build/lib/libns3.29-topology-read-debug.dylib
build/bin/test: build/lib/libns3.29-uan-debug.dylib
build/bin/test: build/lib/libns3.29-wave-debug.dylib
build/bin/test: build/lib/libns3.29-wifi-debug.dylib
build/bin/test: build/lib/libns3.29-wimax-debug.dylib
build/bin/test: build/lib/libns3.29-contrib-haraldott-debug.dylib
build/bin/test: build/lib/libbrite.dylib
build/bin/test: build/lib/libns3.29-lr-wpan-debug.dylib
build/bin/test: build/lib/libns3.29-lte-debug.dylib
build/bin/test: build/lib/libns3.29-applications-debug.dylib
build/bin/test: build/lib/libns3.29-buildings-debug.dylib
build/bin/test: build/lib/libns3.29-config-store-debug.dylib
build/bin/test: /usr/lib/libxml2.dylib
build/bin/test: build/lib/libns3.29-csma-debug.dylib
build/bin/test: build/lib/libns3.29-virtual-net-device-debug.dylib
build/bin/test: build/lib/libns3.29-energy-debug.dylib
build/bin/test: build/lib/libns3.29-spectrum-debug.dylib
build/bin/test: build/lib/libns3.29-antenna-debug.dylib
build/bin/test: build/lib/libns3.29-propagation-debug.dylib
build/bin/test: build/lib/libns3.29-mobility-debug.dylib
build/bin/test: build/lib/libns3.29-point-to-point-debug.dylib
build/bin/test: /usr/local/lib/libpcre2-8.dylib
build/bin/test: build/lib/libns3.29-internet-debug.dylib
build/bin/test: build/lib/libns3.29-bridge-debug.dylib
build/bin/test: build/lib/libns3.29-mpi-debug.dylib
build/bin/test: build/lib/libns3.29-traffic-control-debug.dylib
build/bin/test: build/lib/libns3.29-network-debug.dylib
build/bin/test: build/lib/libns3.29-stats-debug.dylib
build/bin/test: build/lib/libns3.29-core-debug.dylib
build/bin/test: /usr/local/Cellar/gsl/2.5/lib/libgsl.dylib
build/bin/test: /usr/local/Cellar/gsl/2.5/lib/libgslcblas.dylib
build/bin/test: scratch/CMakeFiles/test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/anyonepaw/CLionProjects/ns3-iot-realisation/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../build/bin/test"
	cd /Users/anyonepaw/CLionProjects/ns3-iot-realisation/scratch && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
scratch/CMakeFiles/test.dir/build: build/bin/test

.PHONY : scratch/CMakeFiles/test.dir/build

scratch/CMakeFiles/test.dir/clean:
	cd /Users/anyonepaw/CLionProjects/ns3-iot-realisation/scratch && $(CMAKE_COMMAND) -P CMakeFiles/test.dir/cmake_clean.cmake
.PHONY : scratch/CMakeFiles/test.dir/clean

scratch/CMakeFiles/test.dir/depend:
	cd /Users/anyonepaw/CLionProjects/ns3-iot-realisation && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/anyonepaw/CLionProjects/ns3-iot-realisation /Users/anyonepaw/CLionProjects/ns3-iot-realisation/scratch /Users/anyonepaw/CLionProjects/ns3-iot-realisation /Users/anyonepaw/CLionProjects/ns3-iot-realisation/scratch /Users/anyonepaw/CLionProjects/ns3-iot-realisation/scratch/CMakeFiles/test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : scratch/CMakeFiles/test.dir/depend


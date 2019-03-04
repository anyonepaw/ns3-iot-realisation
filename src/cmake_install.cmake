# Install script for directory: /Users/anyonepaw/CLionProjects/ns3-iot-realisation/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/antenna/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/aodv/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/applications/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/bridge/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/brite/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/buildings/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/click/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/config-store/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/core/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/csma/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/csma-layout/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/dsdv/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/dsr/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/energy/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/flow-monitor/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/internet/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/internet-apps/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/lr-wpan/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/lte/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/mesh/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/mobility/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/mpi/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/netanim/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/network/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/nix-vector-routing/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/olsr/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/point-to-point/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/point-to-point-layout/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/propagation/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/sixlowpan/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/spectrum/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/stats/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/topology-read/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/traffic-control/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/uan/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/virtual-net-device/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/wave/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/wifi/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/wimax/cmake_install.cmake")
  include("/Users/anyonepaw/CLionProjects/ns3-iot-realisation/src/test/cmake_install.cmake")

endif()


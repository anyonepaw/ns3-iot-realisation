cmake_minimum_required(VERSION 3.13)
project(Bitcoin_Simulator_master)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/applications/helper)
include_directories(src/applications/model)
include_directories(src/internet/helper)

add_executable(Bitcoin_Simulator_master
        scratch/bitcoin-test.cc
        scratch/selfish-miner-test.cc
        src/applications/helper/bitcoin-miner-helper.cc
        src/applications/helper/bitcoin-miner-helper.h
        src/applications/helper/bitcoin-node-helper.cc
        src/applications/helper/bitcoin-node-helper.h
        src/applications/helper/bitcoin-topology-helper.cc
        src/applications/helper/bitcoin-topology-helper.h
        src/applications/model/bandwidth-distributions.h
        src/applications/model/bitcoin-miner.cc
        src/applications/model/bitcoin-miner.h
        src/applications/model/bitcoin-node.cc
        src/applications/model/bitcoin-node.h
        src/applications/model/bitcoin-selfish-miner-trials.cc
        src/applications/model/bitcoin-selfish-miner-trials.h
        src/applications/model/bitcoin-selfish-miner.cc
        src/applications/model/bitcoin-selfish-miner.h
        src/applications/model/bitcoin-simple-attacker.cc
        src/applications/model/bitcoin-simple-attacker.h
        src/applications/model/bitcoin.cc
        src/applications/model/bitcoin.h
        src/internet/helper/ipv4-address-helper-custom.cc
        src/internet/helper/ipv4-address-helper-custom.h)

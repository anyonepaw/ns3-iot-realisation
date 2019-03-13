set(name bitcoin)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/applications/helper)
include_directories(src/applications/model)
include_directories(src/internet/helper)

set(source_files
        src/applications/helper/bitcoin-miner-helper.cc
        src/applications/helper/bitcoin-node-helper.cc
        src/applications/helper/bitcoin-topology-helper.cc
        src/applications/model/bitcoin-miner.cc
        src/applications/model/bitcoin-node.cc
        src/applications/model/bitcoin-selfish-miner-trials.cc
        src/applications/model/bitcoin-selfish-miner.cc
        src/applications/model/bitcoin-simple-attacker.cc
        src/applications/model/bitcoin.cc
        src/internet/helper/ipv4-address-helper-custom.cc
        )

set(header_files
        src/applications/helper/bitcoin-miner-helper.h
        src/applications/helper/bitcoin-topology-helper.h
        src/applications/model/bandwidth-distributions.h
        src/internet/helper/ipv4-address-helper-custom.h
        src/applications/model/bitcoin.h
        src/applications/model/bitcoin-simple-attacker.h
        src/applications/model/bitcoin-selfish-miner.h
        src/applications/model/bitcoin-selfish-miner-trials.h
        src/applications/model/bitcoin-node.h
        src/applications/model/bitcoin-miner.h
        src/applications/helper/bitcoin-node-helper.h
        )


#link to dependencies
set(libraries_to_link
        ${libantenna}
        ${libaodv}
        ${libapplications}
        ${libbridge}
        ${libbrite}
        ${libbuildings}
        ${libclick}
        ${libconfig-store}
        ${libcontrib-haraldott}
        ${libcore}
        ${libcsma}
        ${libcsma-layout}
        ${libdsdv}
        ${libdsr}
        ${libenergy}
        ${libflow-monitor}
        ${libinternet-apps}
        ${libinternet}
        ${liblr-wpan}
        ${liblte}
        ${libmesh}
        ${libmobility}
        ${libmpi}
        ${libnetanim}
        ${libnetwork}
        ${libnix-vector-routing}
        ${libolsr}
        ${libpoint-to-point}
        ${libpoint-to-point-layout}
        ${libpropagation}
        ${libsixlowpan}
        ${libspectrum}
        ${libstats}
        ${libtopology-read}
        ${libtraffic-control}
        ${libuan}
        ${libvirtual-net-device}
        ${libwave}
        ${libwifi}
        ${libwimax}
        )


add_executable(bitcoin
        scratch/bitcoin-test.cc
        scratch/selfish-miner-test.cc)

build_lib("${name}" "${source_files}" "${header_files}" "${libraries_to_link}" "${test_sources}")

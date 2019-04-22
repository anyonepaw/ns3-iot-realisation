
set(name internet-of-things)

set(source_files
        model/iot-node-application.cc
        )

set(header_files
        model/iot-node-application.h
        )

set(libraries_to_link ${libcore} ${libnetwork})

set(test_sources)

build_lib("${name}" "${source_files}" "${header_files}" "${libraries_to_link}" "${test_sources}")




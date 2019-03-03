#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PahoMqttCpp::paho-mqttpp3" for configuration "Debug"
set_property(TARGET PahoMqttCpp::paho-mqttpp3 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(PahoMqttCpp::paho-mqttpp3 PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libpaho-mqttpp3.1.0.1.dylib"
  IMPORTED_SONAME_DEBUG "@rpath/libpaho-mqttpp3.1.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS PahoMqttCpp::paho-mqttpp3 )
list(APPEND _IMPORT_CHECK_FILES_FOR_PahoMqttCpp::paho-mqttpp3 "${_IMPORT_PREFIX}/lib/libpaho-mqttpp3.1.0.1.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)

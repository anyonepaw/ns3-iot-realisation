file(REMOVE_RECURSE
  "../../build/lib/libns3.29-lte-debug.pdb"
  "../../build/lib/libns3.29-lte-debug.dylib"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/ns3.29-lte-debug.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

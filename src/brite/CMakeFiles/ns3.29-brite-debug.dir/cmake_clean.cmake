file(REMOVE_RECURSE
  "../../build/lib/libns3.29-brite-debug.pdb"
  "../../build/lib/libns3.29-brite-debug.dylib"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/ns3.29-brite-debug.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

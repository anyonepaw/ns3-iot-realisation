file(REMOVE_RECURSE
  "../../build/lib/libns3.29-mpi-debug.pdb"
  "../../build/lib/libns3.29-mpi-debug.dylib"
)

# Per-language clean rules from dependency scanning.
foreach(lang CXX)
  include(CMakeFiles/ns3.29-mpi-debug.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()

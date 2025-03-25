# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "src/app/BuildIT_autogen"
  "src/app/CMakeFiles/BuildIT_autogen.dir/AutogenUsed.txt"
  "src/app/CMakeFiles/BuildIT_autogen.dir/ParseCache.txt"
  )
endif()

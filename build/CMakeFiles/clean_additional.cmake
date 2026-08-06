# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  [[CMakeFiles\MyQtApp_autogen.dir\AutogenUsed.txt]]
  [[CMakeFiles\MyQtApp_autogen.dir\ParseCache.txt]]
  "MyQtApp_autogen"
  )
endif()

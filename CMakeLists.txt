# CMakeList.txt : CMake project for ImpressiveUpdaterX, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


project ("ImpressiveUpdaterX")

# Add source to this project's executable.
add_executable (ImpressiveUpdaterX "ImpressiveUpdaterX.cc" "ImpressiveUpdaterX.hh" "dirent.h" "zip.h" "zip.c" "miniz.h")

# TODO: Add tests and install targets if needed.
link_directories(.\packages\curl-vc141-dynamic-x86_64.7.59.0\build\native\lib\Win32\static\)
#add_dependencies(${CMAKE_BINARY_DIR}/deps/curl-7.81.0/include/curl

#target_include_directories(${CMAKE_BINARY_DIR}/deps/curl-7.81.0/include/curl)
#add_library( deps/libs/libcurl.dll SHARED IMPORTED )
#find_library(CURLIB ${CMAKE_BINARY_DIR}/deps/libs/libcurl.dll)
#target_link_libraries(ImpressiveUpdaterX PRIVATE "${CURLIB}")

#set(CMAKE_C_STANDARD 90)
if (MSVC)
  # Use secure functions by default and suppress warnings about "deprecated" functions
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES=1")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES_COUNT=1")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /D _CRT_NONSTDC_NO_WARNINGS=1 /D _CRT_SECURE_NO_WARNINGS=1")

elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" OR
        "${CMAKE_C_COMPILER_ID}" STREQUAL "AppleClang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -pedantic")
endif (MSVC)
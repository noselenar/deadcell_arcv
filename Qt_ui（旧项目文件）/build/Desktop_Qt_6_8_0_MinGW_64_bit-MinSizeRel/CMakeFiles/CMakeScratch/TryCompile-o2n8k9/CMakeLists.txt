cmake_minimum_required(VERSION 3.29.3.0)
cmake_policy(SET CMP0126 OLD)
cmake_policy(SET CMP0128 OLD)
project(CMAKE_TRY_COMPILE CXX)
set_property(DIRECTORY PROPERTY INCLUDE_DIRECTORIES "")
set(CMAKE_VERBOSE_MAKEFILE 1)
set(CMAKE_CXX_FLAGS "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMPILE_DEFINITIONS}")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_EXE_LINKER_FLAGS "")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${EXE_LINKER_FLAGS}")
include_directories(${INCLUDE_DIRECTORIES})
set(CMAKE_SUPPRESS_REGENERATION 1)
link_directories(${LINK_DIRECTORIES})
add_definitions([==[-v]==])
cmake_policy(SET CMP0065 NEW)
cmake_policy(SET CMP0083 NEW)
cmake_policy(SET CMP0155 OLD)
cmake_policy(SET CMP0157 OLD)
include("${CMAKE_ROOT}/Modules/Internal/HeaderpadWorkaround.cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "D:/projs/personal projects/deadcell_save/Qt_ui/build/Desktop_Qt_6_8_0_MinGW_64_bit-MinSizeRel/CMakeFiles/CMakeScratch/TryCompile-o2n8k9")
add_executable(cmTC_409bb)
target_sources(cmTC_409bb PRIVATE
  "D:/applications/Qt/Tools/CMake_64/share/cmake-3.29/Modules/CMakeCXXCompilerABI.cpp"
)
file(GENERATE OUTPUT "${CMAKE_BINARY_DIR}/cmTC_409bb_loc"
     CONTENT $<TARGET_FILE:cmTC_409bb>)
target_link_libraries(cmTC_409bb ${LINK_LIBRARIES})

cmake_minimum_required(VERSION 3.10.2)

if(WIN32 AND DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

project(CosmoSpaceShip VERSION 0.1.0)

set(BUILD_SHARED_LIBS OFF)
option(BUILD_TESTING OFF)
option(WITH_GFLAGS OFF)
add_compile_options(-DHAVE_SNPRINTF)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

add_subdirectory(glog)
include_directories(glog)

enable_testing()
add_subdirectory(googletest)

add_subdirectory(src)

add_subdirectory(CosmoSpaceShipTest)

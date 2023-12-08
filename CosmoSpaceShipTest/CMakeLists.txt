cmake_minimum_required(VERSION 3.10.2)

project(SolitaireTest)

if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

find_library(GTEST_LIB gtestd ${CMAKE_BINARY_DIR}/lib/Debug)

if(GTEST_LIB)
    message("Gtest library found")
else()
    message("Gtest library not found")
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

message("CMAKE_BINARY_DIR = " ${CMAKE_BINARY_DIR})
include_directories("${CMAKE_BINARY_DIR}/googletest")

if (CMAKE_VERSION VERSION_LESS 2.8.11)

    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include")

    message(gtest_SOURCE_DIR ${gtest_SOURCE_DIR})
    message(gmock_SOURCE_DIR ${gmock_SOURCE_DIR})  

endif()

set(engine_dir_test ../src/Engine/)
set(utils_dir_test ../src/Utils/)

file(GLOB_RECURSE common_sources_test ${engine_dir_test}*.cpp ${utils_dir_test}*.cpp)  
file(GLOB_RECURSE common_headers_test ${engine_dir_test}*.h ${utils_dir_test}*.h)

file(GLOB_RECURSE TEST_SRC_SOURCE ${PROJECT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE TEST_SRC_HEADERS ${PROJECT_SOURCE_DIR}/*.h)

set(sources_test
    ${TEST_SRC_SOURCE}
    ${TEST_SRC_HEADERS}
    ${common_sources_test}
    ${common_headers_test}
)

# find_package(
#             SFML 2.5
#             COMPONENTS system window graphics
#             REQUIRED)


find_library(GLOG_LIB glogd ${CMAKE_BINARY_DIR}/glog/Debug)
if(GLOG_LIB)
    message ("GLOG_TEST FOUND!!!")
else()
    message ("GLOG_TEST NOT FOUND!!!")
endif()

add_executable(testmain ${sources_test})
target_link_libraries(testmain gtest gmock_main gtest_main glog sfml-system sfml-window
        sfml-graphics)
add_test(NAME testmain COMMAND testmain)
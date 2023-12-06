cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

set(coregame_dir CoreGame/)
set(engine_dir Engine/)
set(utils_dir Utils/)

file(GLOB_RECURSE common_sources ${engine_dir}*.cpp ${utils_dir}*.cpp ${coregame_dir}*.cpp)
file(GLOB_RECURSE common_headers ${engine_dir}*.h ${utils_dir}*.h ${coregame_dir}*.h)

file(GLOB_RECURSE src_sources *.cpp)
file(GLOB_RECURSE src_headers *.h)

find_package(
  SFML 2.5
  COMPONENTS system window graphics
  REQUIRED)

set(sources ${src_sources} ${src_headers} ${common_sources} ${common_headers})

add_executable(${PROJECT_NAME} ${sources})

find_library(GLOG_LIB glogd ${CMAKE_BINARY_DIR}/glog/Debug)
if(GLOG_LIB)
  message("GLOG FOUND!!!")
else()
  message("GLOG NOT FOUND!!!")
endif()

target_link_libraries(${PROJECT_NAME} glog sfml-system sfml-window
                      sfml-graphics)

# set(BLUEPRINTS_DIR ../Blueprints)
# file(GLOB blueprints ${BLUEPRINTS_DIR}/*.*)

# foreach(item IN LISTS blueprints)
    # get_filename_component(dest_filename ${item} NAME)
# 
    # add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy ${item} 
        # $<TARGET_FILE_DIR:${PROJECT_NAME}>/${dest_filename}
    # )
# endforeach()

# set(FONT_DIR ../Font)
# file(GLOB font ${FONT_DIR}/*.*)

# foreach(item IN LISTS font)
    # get_filename_component(dest_filename ${item} NAME)
# 
    # add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy ${item} 
        # $<TARGET_FILE_DIR:${PROJECT_NAME}>/${dest_filename}
    # )
# endforeach()
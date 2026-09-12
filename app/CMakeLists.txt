cmake_minimum_required(VERSION 3.20.0)

cmake_path(GET CMAKE_CURRENT_LIST_DIR PARENT_PATH PARENT_DIR)
list(APPEND BOARD_ROOT ${PARENT_DIR})
find_package(Zephyr REQUIRED)
project(my_zephyr_app)

target_sources(app PRIVATE src/main.cpp)

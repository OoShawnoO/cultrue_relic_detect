cmake_minimum_required(VERSION 3.25)
project(culture_relic_detect CXX)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../libs/jinja2cpp/lib/jinja2cpp)
set(Torch_DIR ~/CLionProjects/libs/torch/)
set(Torch_DIR ~/CLionProjects/libs/libtorch/share/cmake/Torch)
set(OpenCV_DIR ~/CLionProjects/libs/opencv/lib/cmake/opencv4)

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} ${OPENCV_CXX_FLAGS}")

include_directories(${TORCH_INCLUDE_DIRS})
include_directories(${OPENCV_INCLUDE_DIRS})

find_package(jinja2cpp REQUIRED)
find_package(FMT REQUIRED)

link_directories(~/CLionProjects/libs/sqlite3/lib)
include_directories(~/CLionProjects/libs/sqlite3/include)
include_directories(conv_event)
include_directories(~/CLionProjects/libs/jinja2cpp/include)
add_executable(culture_relic_detect main.cpp)

target_link_libraries(culture_relic_detect jinja2cpp fmt::fmt sqlite3)
target_link_libraries(culture_relic_detect "${TORCH_LIBRARIES}" "${OpenCV_LIBS}")

set_target_properties(culture_relic_detect PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )
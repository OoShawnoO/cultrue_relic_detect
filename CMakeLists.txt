cmake_minimum_required(VERSION 3.25)

set(TARGET culture_relic_detect)

project(${TARGET} CXX)

set(LIBS ~/CLionProjects/libs)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH} ${LIBS}")

set(jinja2cpp_DIR ${LIBS}/jinja2cpp/lib/jinja2cpp)
set(Torch_DIR ${LIBS}/torch/)
set(Torch_DIR ${LIBS}/libtorch/share/cmake/Torch)
set(OpenCV_DIR ${LIBS}/opencv/lib/cmake/opencv4)


find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)
find_package(jinja2cpp REQUIRED)
find_package(FMT REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS} ${OPENCV_CXX_FLAGS} -DNO_ERROR_LOG")

link_directories(${LIBS}/sqlite3/lib)
include_directories(conv_event)
include_directories(${TORCH_INCLUDE_DIRS})
include_directories(${OPENCV_INCLUDE_DIRS})
include_directories(${LIBS}/sqlite3/include)
include_directories(${LIBS}/jinja2cpp/include)
include_directories(${LIBS}/hiredis/include)
link_directories(${LIBS}/hiredis/lib)

add_executable(${TARGET} main.cpp routers/detect_router.h routers/index_router.h routers/item_router.h routers/login_router.h filters/templates_filter.h filters/tmp_router_filter.h)

add_custom_target(
        generate_header ALL
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/routers/routers.py
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/filters/filters.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
add_dependencies(${TARGET} generate_header)
message([INFO]生成routers.h完毕)
message([INFO]生成filters.h完毕)


find_path(REDIS_PLUS_PLUS_HEADER sw)
target_include_directories(${TARGET} PUBLIC ${REDIS_PLUS_PLUS_HEADER})

find_library(REDIS_PLUS_PLUS_LIB redis++)
target_link_libraries(${TARGET} ${REDIS_PLUS_PLUS_LIB})
target_link_libraries(${TARGET} jinja2cpp fmt::fmt sqlite3)
target_link_libraries(${TARGET} "${TORCH_LIBRARIES}" "${OpenCV_LIBS}")

set_target_properties(${TARGET} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )
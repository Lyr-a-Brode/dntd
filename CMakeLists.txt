cmake_minimum_required(VERSION 3.0)

project(dntd)

FIND_PACKAGE(OpenCV 4.1.2 REQUIRED)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src 
    ${OpenCV_INCLUDE_DIRS} 
    ${CMAKE_SOURCE_DIR}/lib/nng/include
    ${CMAKE_SOURCE_DIR}/lib/nngpp/include
    ${CMAKE_SOURCE_DIR}/lib/cxxopts/include)

link_directories(${OpenCV_LIB_DIR})

SET(SRC 
    src/options.cpp
    src/main.cpp
    src/dntd.cpp)

add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${CMAKE_SOURCE_DIR}/lib/nng/libnng.a)
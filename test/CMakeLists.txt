cmake_minimum_required(VERSION 2.8)

project(test)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
# set(OpenCV_DIR /usr/local/Cellar/opencv/4.2.0/lib/cmake/opencv4)
# FIND_PACKAGE( OpenCV )
# INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )
# include_directories(${PCL_INCLUDE_DIRS} ${OpenCV_DIR})


add_executable(${PROJECT_NAME} "main2.cpp")
# target_link_libraries(test LINK_PUBLIC ${OpenCV_LIBS})
target_link_libraries(test LINK_PUBLIC pthread)

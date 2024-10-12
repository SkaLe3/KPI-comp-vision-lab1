cmake_minimum_required(VERSION 3.5)
project(CompVision)

set(OPENCV_MAP_IMPORTED_CONFIG "")

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries)

set(CMAKE_DEFAULT_TARGET App)

find_package(OpenCV REQUIRED world)
find_package(Eigen3 REQUIRED)
find_package(Ceres REQUIRED)

add_subdirectory(CVLib)
add_subdirectory(App) 
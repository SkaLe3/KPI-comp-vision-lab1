cmake_minimum_required(VERSION 3.5)

file(GLOB_RECURSE SRCS ${CMAKE_CURRENT_SOURCE_DIR}/Source/*.[ch]pp)

add_executable(App ${SRCS})

target_link_libraries(App PRIVATE ${CERES_LIBRARIES} Eigen3::Eigen ${OpenCV_LIBS} CVLib)

# Set binary output directory for App
set_target_properties(App PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries)
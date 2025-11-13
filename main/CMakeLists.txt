# Defining CMAKE project
cmake_minimum_required(VERSION 3.10)
project(TrainingCodes)

# Setting LSP to auto complete and to detect OpenCV
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Finding OpenCV Packages
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Gather all .cpp files
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Loop through each .cpp file
foreach(src_file ${SRC_FILES})
  get_filename_component(exec_name ${src_file} NAME_WE)
  add_executable(${exec_name} ${src_file})
  target_link_libraries(${exec_name} PRIVATE ${OpenCV_LIBS})
  message(STATUS "Added executable: ${exec_name}")
endforeach()

# Which OpenCV version and path are used
message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV include path: ${OpenCV_INCLUDE_DIRS}")

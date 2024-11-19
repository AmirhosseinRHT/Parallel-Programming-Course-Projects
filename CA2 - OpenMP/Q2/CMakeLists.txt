cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER:FILEPATH   C:/Strawberry/c/bin/g++.exe)
set(CMAKE_C_COMPILER:FILEPATH   C:/Strawberry/c/bin/g++.exe)
project(Q1)  

find_package(OpenCV REQUIRED)  

# Check for OpenMP support  
find_package(OpenMP)  
if (OPENMP_FOUND)  
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")  
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")  
else()  
    message(WARNING "OpenMP not found - project may not compile")  
endif()  

set(ENV{OPENCV_HIGHGUI_BACKEND} "WIN32")
add_executable(Q1 main.cpp)  
target_link_libraries(Q1 ${OpenCV_LIBS} OpenMP::OpenMP_CXX) 
set_target_properties(Q1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")  
# add_custom_command(  
#     TARGET Q1 POST_BUILD  
#     COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_CURRENT_BINARY_DIR}"  
#     COMMENT "Deleting build directory"  
# )  
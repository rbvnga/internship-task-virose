cmake_minimum_required(VERSION 3.15) #

# Project Name
project("RobotConfig")

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(rapidjson_include_dir "${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include/")

include_directories(${rapidjson_include_dir})
include_directories(include) ##
# include_directories()

## add_executable(test Test.cpp)
## add_executable(test2 test2.cpp)
add_executable(2 
src/motion_movie.cpp 
src/motion_unit.cpp 
tugas_2.cpp
)
 ##

# target_link_libraries()

#
cmake_minimum_required(VERSION 3.9)

project(mc_hologrammer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#--------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} calculating.cpp)

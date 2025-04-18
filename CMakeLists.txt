cmake_minimum_required(VERSION 3.30)
project(thread_cpp)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 23)

# 使用不同的目标名称
add_executable(thread_example thread/main.cpp)
add_executable(async_example async/main.cpp)
add_executable(mutex_example mutex/mutex.cpp)

cmake_minimum_required(VERSION 3.10)
project(Zing)

set(CMAKE_CXX_STANDARD 11)

add_executable(Zing main.cpp util.cpp util.h epoll.cpp epoll.h threadpool.cpp threadpool.h)
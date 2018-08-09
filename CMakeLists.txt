cmake_minimum_required(VERSION 3.10)
project(Zing)

set(CMAKE_CXX_STANDARD 11)

add_executable(Zing main.cpp util.cpp util.h epoll.cpp epoll.h threadpool.cpp threadpool.h http_request.cpp http_request.h http.cpp http.h http_parse.cpp http_parse.h list.h error.h rio.cpp rio.h priority_queue.cpp priority_queue.h test_unit/priority_queue_test.cpp)
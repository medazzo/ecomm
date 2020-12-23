cmake_minimum_required(VERSION 3.0)

project(eposcomm)

add_executable(eposcomm main.cpp)

install(TARGETS eposcomm RUNTIME DESTINATION bin)

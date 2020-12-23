cmake_minimum_required(VERSION 3.0)

project(eposcomm)

add_executable(eposcomm EposComm.cpp EposCommand.cpp main.cpp)
include_directories(eposcomm . )
target_link_libraries(eposcomm -lpthread)
install(TARGETS eposcomm RUNTIME DESTINATION bin)

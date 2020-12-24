cmake_minimum_required(VERSION 3.0)

project(eposcomm)

add_executable(eposcommbi EposComm.cpp EposCommand.cpp duplex.cpp EposCommBi.cpp)
add_executable(eposcomm EposComm.cpp EposCommand.cpp main.cpp EposCommBi.cpp)

include_directories(eposcomm . )
include_directories(eposcommbi . )

target_link_libraries(eposcommbi -lpthread)
target_link_libraries(eposcomm -lpthread)

install(TARGETS eposcommbi RUNTIME DESTINATION bin)
install(TARGETS eposcomm RUNTIME DESTINATION bin)

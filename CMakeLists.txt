cmake_minimum_required(VERSION 3.0)

project(eposcomm)

add_executable(eposduplex EposComm.cpp EposCommand.cpp duplex.cpp)
add_executable(eposcomm EposComm.cpp EposCommand.cpp main.cpp)

include_directories(eposcomm . )
include_directories(eposduplex . )

target_link_libraries(eposduplex -lpthread)
target_link_libraries(eposcomm -lpthread)

install(TARGETS eposduplex RUNTIME DESTINATION bin)
install(TARGETS eposcomm RUNTIME DESTINATION bin)

cmake_minimum_required(VERSION 3.0)

project(easincomm LANGUAGES CXX)

set( CMAKE_CXX_STANDARD_REQUIRED ON )

#
# version information
#
set( E_COMM_MAJOR_VERSION 0 )
set( E_COMM_MINOR_VERSION 1 )
set( E_COMM_MICRO_VERSION 0 )
set( PKG_VERSION ${E_COMM_MAJOR_VERSION}.${E_COMM_MINOR_VERSION}.${E_COMM_MICRO_VERSION} )
set( E_POS_MGR_MAJMIN_VERSION ${E_COMM_MAJOR_VERSION}.${E_COMM_MINOR_VERSION} )

#
# output folders
#
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) 


#
# Check our options
#
option( BUILD_TESTS       "Build tests" ON )
option( USE_MESSAGE_QUEUE "Use System V Message Queue" OFF )

#
# Add main source code folder
#
add_subdirectory( src )

#
# Check if build tests is enabled
#
if( BUILD_TESTS )
    enable_testing()
    add_subdirectory( tests )
endif( BUILD_TESTS )


#
# Print configuration information for the user
#
message(STATUS "")
message(STATUS "")
message(STATUS "Easin Comm configuration summary:")
message(STATUS "")

message(STATUS "  Build type ................................. : ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install prefix ............................. : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Library location ........................... : ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}")
message(STATUS "  C++ compiler ............................... : ${CMAKE_CXX_COMPILER}")
message(STATUS "  ---------------------------------------------------------------")
message(STATUS "  Build Unitary tests ........................ : ${BUILD_TESTS}")
message(STATUS "  Use Message Queue( else Named Pipe FIFO ) .. : ${USE_MESSAGE_QUEUE}")

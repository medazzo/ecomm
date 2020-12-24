# Easin Comm library

#Compute required sources
file(GLOB lib_sources "*.cpp")
file(GLOB lib_headers "*.h")

#Create library
ADD_LIBRARY(ecomm SHARED ${lib_sources})

#add this library's header folder to the global include set
INCLUDE_DIRECTORIES(".")

# set public headers  
foreach(file  ${lib_headers})
    set_target_properties( ecomm PROPERTIES PUBLIC_HEADER  "${file}") 
endforeach()

# set version and needed link
set_target_properties( ecomm PROPERTIES VERSION ${PKG_VERSION} SOVERSION ${E_COMM_MINOR_VERSION} )


 # set how to install
 install(TARGETS ecomm DESTINATION lib)

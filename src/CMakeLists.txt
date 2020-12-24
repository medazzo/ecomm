# Easin Comm library


#Compute required sources
file(GLOB lib_sources "*.cpp")
file(GLOB lib_headers "*.h")

#Create library
ADD_LIBRARY(ecomm SHARED ${lib_headers} ${lib_sources})
set_target_properties(ecomm PROPERTIES LINKER_LANGUAGE CXX)
#
# Check if using Message Queue is enabled, 
# If Enbled , then will use system V Message Queue insted of Nampe Pipes.
# Please Be aware that performace of Named Pipe Are Way better than M Queue !!
# On our base tests for 500 000 Message :
# - Mqueue takes 0.69 msec per msg 
# - Fifo   takes 0.66 msec per msg
#
if( USE_MESSAGE_QUEUE )
    target_compile_options(ecomm PRIVATE "-DUSE_MESSAGE_QUEUE=1" )
endif( USE_MESSAGE_QUEUE )

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

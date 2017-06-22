#set(Boost_NO_BOOST_CMAKE ON) 
find_package(Boost 1.58.0 COMPONENTS system filesystem REQUIRED)
include_directories( ${HalIR_SOURCE_DIR/SpecSac} ${Boost_INCLUDE_DIR} ${HalIR_SOURCE_DIR/HalIR} )
set( SRCS test.cpp )
ADD_DEFINITIONS( -std=c++11 )
link_directories( ${HalIR_BINARY_DIR/SpecSac} ${HalIR_BINARY_DIR/HalIR} ${Boost_LIBRARY_DIR} )

add_executable(mytest ${SRCS}) 

target_link_libraries(mytest SpecSac HalIR ${Boost_LIBRARIES})

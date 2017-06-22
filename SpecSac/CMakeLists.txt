cmake_minimum_required(VERSION 2.8)

set( SRCS specsac.cpp spectra.cpp spc.cpp )

add_library(SpecSac ${SRCS}) 
#target_link_libraries(SpecSac ${BOOST_LIBRARIES} )

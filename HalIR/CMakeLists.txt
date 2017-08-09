cmake_minimum_required(VERSION 2.8)

find_package(Armadillo)
#get_directory_properties(output ${ARMADILLO_INCLUDE_DIRS})
message(STATUS ${ARMADILLO_INCLUDE_DIRS})
message(STATUS ${ARMADILLO_LIBRARIES})
include_directories( ${ARMADILLO_INCLUDE_DIRS} )
set( SRCS hitran.cpp halirSpec.cpp halir.cpp Faddeeva.cpp )
#libraries( ${ARMSILLO_LIBRARIES} )

add_library(HalIR ${SRCS})

cmake_minimum_required(VERSION 2.8)

find_package(Armadillo)
#get_directory_properties(output ${ARMADILLO_INCLUDE_DIRS})
#message(STATUS ${ARMADILLO_INCLUDE_DIRS})
#message(STATUS ${ARMADILLO_LIBRARIES})
find_package(OpenMP)
if (OPENMP_FOUND)
    #set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()


find_package(Boost COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories( ${ARMADILLO_INCLUDE_DIRS} )
set( SRCS hitran.cpp halirSpec.cpp halir.cpp Faddeeva.cpp )
#libraries( ${ARMSILLO_LIBRARIES} )

add_library(HalIR SHARED ${SRCS})

target_link_libraries(HalIR ${ARMADILLO_LIBRARIES} ${OpenMP_CXX_FLAGS} ${Boost_LIBRARIES})

install( TARGETS HalIR DESTINATION .mlib)

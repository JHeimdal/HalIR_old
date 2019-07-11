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

find_package(PythonInterp 3.5 REQUIRED)
find_package(PythonLibs 3.5 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(Boost COMPONENTS system filesystem python REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories( ${ARMADILLO_INCLUDE_DIRS} )
set( SRCS hitran.cpp halirSpec.cpp halir.cpp Faddeeva.cpp )
#libraries( ${ARMSILLO_LIBRARIES} )

add_library(calcSpec_cu SHARED calcSpectra.cu)
add_library(HalIR SHARED ${SRCS})

target_link_libraries(HalIR ${ARMADILLO_LIBRARIES} ${OpenMP_CXX_FLAGS} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} calcSpec_cu)

install( TARGETS HalIR calcSpec_cu DESTINATION .mlib)

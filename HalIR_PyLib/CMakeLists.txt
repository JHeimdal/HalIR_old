cmake_minimum_required(VERSION 2.8)

find_package(Armadillo)


find_package(PythonInterp 3.5 REQUIRED)
find_package(PythonLibs 3.5 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

find_package(Boost COMPONENTS python REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories( ${ARMADILLO_INCLUDE_DIRS} )

add_library(halir_pylib SHARED halir_pylib.cpp)

target_link_libraries(halir_pylib ${ARMADILLO_LIBRARIES} ${OpenMP_CXX_FLAGS} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES} HalIR SpecSac)

set_target_properties(halir_pylib PROPERTIES SUFFIX .so)
set_target_properties(halir_pylib PROPERTIES PREFIX "")

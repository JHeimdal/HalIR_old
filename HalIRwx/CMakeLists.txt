cmake_minimum_required(VERSION 2.8)

project(halIR)

#include_directories("${PROJECT_SOURCE_DIR}/utils")
#add_subdirectory(utils)
#add_definitions(-DUSE_XPM_BITMAPS)
#set( SRCS halirwx.cpp FreeType.cpp hitran.cpp mdraw.cpp specrender.cpp spectra.cpp )

set( SRCS halirwx.cpp mdraw.cpp )

find_package(wxWidgets COMPONENTS base core gl REQUIRED)
#find_package(Armadillo)
#find_package(GLUT)
find_package(Boost REQUIRED)
find_package(OpenGL)
find_package(Freetype REQUIRED)
include(${wxWidgets_USE_FILE} ${BOOST_INCLUDE_DIR} )
include_directories( ${FREETYPE_INCLUDE_DIRS})

if(WIN32)
  set(SRCS ${SRCS} halirwx.rc)
  add_executable(halIR WIN32 ${SRCS})
else()
  add_executable(halIR ${SRCS}) 
endif(WIN32)

#target_link_libraries(test1 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${ARMADILLO_LIBRARIES})
#target_link_libraries(test1 ${ARMADILLO_LIBRARIES})
target_link_libraries(halIR ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES} ${FREETYPE_LIBRARIES} )


cmake_minimum_required(VERSION 2.8)
#ADD_DEFINITIONS( -O3 -xavx -qopt-report1 -std=c++11 )
ADD_DEFINITIONS( -mavx2 -fopenmp -ftree-vectorize -ftree-vectorizer-verbose=2 -O3 -std=c++11)
#ADD_DEFINITIONS( -O2 -std=c++11)
project(HalIR)

set(BOOST_ROOT /usr/local/boost)
set(Boost_NO_BOOST_CMAKE ON)
# Add directories containing libraries
add_subdirectory( SpecSac )
add_subdirectory( HalIR )
# Add directories that compile execiutables
add_subdirectory( Test )


cmake_minimum_required(VERSION 2.8)

project(MeshScape CXX)

set(CMAKE_CXX_FLAGS "-Wfatal-errors -Wextra -Wall -Weffc++ -std=gnu++11 -pedantic -pipe -march=native -frounding-math")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -DBOOST_UBLAS_NDEBUG")
#set(CMAKE_CXX_FLAGS "-std=c++0x")
#set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -DNDEBUG -DBOOST_UBLAS_NDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

if (PROFILE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -ggdb")
endif (PROFILE)

# Disable CGAL's -frounding-math check when running in valgrind, which has
# incorrect rounding.
#
# add_definitions(-DCGAL_DISABLE_ROUNDING_MATH_CHECK)

find_package(CGAL COMPONENTS ImageIO)
if (CGAL_FOUND)
    message(STATUS "Include ${CGAL_USE_FILE}")
    include(${CGAL_USE_FILE})
    message(STATUS "CGAL_LIBRARIES ${CGAL_LIBRARIES}")
    message(STATUS "CGAL_3RD_PARTYLIBRARIES ${CGAL_3RD_PARTYLIBRARIES}")
else (CGAL_FOUND)
    message(FATAL_ERROR "CGAL not found")
endif (CGAL_FOUND)

#find_package(Boost 1.49)
#if (!Boost_FOUND)
#    message(FATAL_ERROR "Boost not found")
#endif (!Boost_FOUND)

add_executable(meshscape meshscape.cpp cli.cpp)

target_link_libraries(meshscape ${CGAL_LIBRARIES} tiff)

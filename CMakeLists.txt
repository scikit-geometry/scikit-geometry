cmake_minimum_required(VERSION 2.8)

project(pygal)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/pygal/")

# Add a CMake parameter for choosing a desired Python version
set(PYBIND11_PYTHON_VERSION "3" CACHE STRING "Python version to use for compiling the example application")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

find_package(PythonLibs)

# This is CGAL related stuff
find_package(CGAL COMPONENTS Core)
include(${CGAL_USE_FILE})

include_directories(${PYTHON_INCLUDE_DIR} ${CGAL_INCLUDE_DIRS})  
# Include path for pybind11 header files
include_directories(include thirdparty)

set(PYGAL_SOURCE
  src/pygal.cpp
  src/kernel.cpp
  src/polygon.cpp
  src/global_functions.cpp
  src/boolean.cpp
  src/convex_hull.cpp
  src/visibility.cpp
  src/arrangement.cpp
  src/principal_component_analysis.cpp
  src/minkowski.cpp
  src/polyhedron.cpp
  src/aabb_tree.cpp
  src/voronoi_delaunay.cpp
  src/optimal_transport.cpp
)

# Create the binding library
add_library(_pygal SHARED
  ${PYGAL_SOURCE}
)

# Don't add a 'lib' prefix to the shared library
set_target_properties(_pygal PROPERTIES PREFIX "")
target_link_libraries(_pygal ${PYTHON_LIBRARY} ${CGAL_LIBRARIES})
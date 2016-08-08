# CMakeLists.txt -- Build system for the pybind11 examples
#
# Copyright (c) 2015 Wenzel Jakob <wenzel@inf.ethz.ch>
#
# All rights reserved. Use of this source code is governed by a
# BSD-style license that can be found in the LICENSE file.

cmake_minimum_required(VERSION 2.8)

project(pygal)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "pygal/")
# Add a CMake parameter for choosing a desired Python version
set(PYBIND11_PYTHON_VERSION "3" CACHE STRING "Python version to use for compiling the example application")

# Set a default build configuration if none is specified. 'MinSizeRel' produces the smallest binaries
#if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#  message(STATUS "Setting build type to 'MinSizeRel' as none was specified.")
#  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
#  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
#    "Release" "RelWithDebInfo")
#endif()
#string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(Python_ADDITIONAL_VERSIONS 3.5)

find_package(PythonLibs)

# This is CGAL related stuff

find_package(CGAL COMPONENTS Core)
include(${CGAL_USE_FILE})

include_directories(${PYTHON_INCLUDE_DIR} ${CGAL_INCLUDE_DIRS})  
# Include path for pybind11 header files
include_directories(include thirdparty)

set(PYBIND11_HEADERS
  thirdparty/pybind11/attr.h
  thirdparty/pybind11/cast.h
  thirdparty/pybind11/common.h
  thirdparty/pybind11/complex.h
  thirdparty/pybind11/descr.h
  thirdparty/pybind11/functional.h
  thirdparty/pybind11/numpy.h
  thirdparty/pybind11/operators.h
  thirdparty/pybind11/pybind11.h
  thirdparty/pybind11/pytypes.h
  thirdparty/pybind11/stl.h
  thirdparty/pybind11/typeid.h
)

set(PYGAL_SOURCE
  src/pygal.cpp
  src/pygal_kernel.cpp
  src/pygal_global_functions.cpp
  src/pygal_polygon.cpp
  src/pygal_boolean.cpp
  src/pygal_convex_hull.cpp
  src/pygal_visibility.cpp
  src/pygal_arrangement.cpp
  src/pygal_principal_component_analysis.cpp
  src/pygal_minkowski.cpp
  src/pygal_polyhedron.cpp
  src/pygal_aabb_tree.cpp
  src/pygal_3D_minkowski.cpp
  src/pygal_voronoi_delaunay.cpp
  src/pygal_optimal_transport.cpp
  # src/funcs.cpp
)

# Create the binding library
add_library(pygal SHARED
  ${PYBIND11_HEADERS}
  ${PYGAL_SOURCE}
)

# Don't add a 'lib' prefix to the shared library
set_target_properties(pygal PROPERTIES PREFIX "")

target_link_libraries(pygal ${PYTHON_LIBRARY} ${CGAL_LIBRARIES})

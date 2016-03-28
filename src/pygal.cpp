#include <pybind11/pybind11.h>
namespace py = pybind11;

// Python CGAL bindings 
// Author: Wolf Vollprecht <wolf.vollprecht@rapyuta-robotics.com>
// 
// This is a single-file python wrapper for CGAL
// It currently exports function useful for 2D manipulation of
// Polygons and Polygons with holes
// But can also serve as basis to add more of 
// CGAL's functionality (e.g. for 3D functions)

void init_pygal_kernel(py::module &);
void init_global_functions(py::module &);
void init_polygon(py::module &);
void init_boolean_set(py::module &);
void init_convex_hull(py::module &);
void init_test(py::module &);
void init_arrangement(py::module &);
void init_visibility(py::module &);
void init_principal_component_analysis(py::module &);
void init_minkowski(py::module &);

PYBIND11_PLUGIN(pygal) {
    py::module m("pygal", "Python Geometric Algorithms Library - based on CGAL");
    init_pygal_kernel(m);
    init_global_functions(m);
    init_polygon(m);
    init_boolean_set(m);
    init_convex_hull(m);
    init_arrangement(m);
    init_visibility(m);
    init_principal_component_analysis(m);
    init_minkowski(m);
    return m.ptr();
}

// Python CGAL bindings 
// Author: Wolf Vollprecht <w.vollprecht@gmail.com>
// 
// This is a single-file python wrapper for CGAL
// It currently exports function useful for 2D manipulation of
// Polygons and Polygons with holes
// But can also serve as basis to add more of 
// CGAL's functionality (e.g. for 3D functions)

#include <pybind11/pybind11.h>
namespace py = pybind11;


void init_pygal_kernel(py::module&);
void init_global_functions(py::module&);
void init_polygon(py::module&);
void init_boolean_set(py::module&);
void init_convex_hull(py::module&);
void init_arrangement(py::module&);
void init_visibility(py::module&);
void init_principal_component_analysis(py::module&);
void init_minkowski(py::module&);
void init_polyhedron(py::module&);
void init_aabb_tree(py::module&);
void init_voronoi_delaunay(py::module&);
void init_optimal_transport(py::module&);
void init_polygon_offset(py::module&);


PYBIND11_MODULE(_pygal, m) {
    m.doc() = "";

    init_global_functions(m);
    init_pygal_kernel(m);
    init_arrangement(m);
    init_polygon(m);
    init_boolean_set(m);
    init_convex_hull(m);
    init_visibility(m);
    init_principal_component_analysis(m);
    init_minkowski(m);
    init_polyhedron(m);
    init_aabb_tree(m);
    init_voronoi_delaunay(m);
    init_optimal_transport(m);
    // init_polygon_offset(m);
}

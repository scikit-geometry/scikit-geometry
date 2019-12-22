#include "skgeom.hpp"
#include <CGAL/minkowski_sum_2.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <CGAL/minkowski_sum_3.h>

typedef CGAL::Polyhedron_3<Kernel>                  Polyhedron_3;
typedef CGAL::Nef_polyhedron_3<Kernel>              Nef_polyhedron_3;


template <typename T1, typename T2>
Polygon_with_holes_2 get_minkowski(T1 p, T2 q) {
    return CGAL::minkowski_sum_2(p, q);
}

Polyhedron_3 polyhedron_minkowski_sum_3(Polyhedron_3& p, Polyhedron_3& q) {
    // convert Polyhedron to NEF
    Nef_polyhedron_3 a(p);
    Nef_polyhedron_3 b(q);
    Nef_polyhedron_3 summed = CGAL::minkowski_sum_3(a, b);
    Polyhedron_3 result;
    summed.convert_to_polyhedron(result);
    return result;
}

void init_minkowski(py::module & m) {
    auto sub = m.def_submodule("minkowski");
    sub.def("minkowski_sum", &get_minkowski<Polygon_2, Polygon_2>);
    sub.def("minkowski_sum", &get_minkowski<Polygon_with_holes_2, Polygon_2>);
    sub.def("minkowski_sum", &get_minkowski<Polygon_with_holes_2, Polygon_with_holes_2>);
    sub.def("minkowski_sum", &polyhedron_minkowski_sum_3);
}
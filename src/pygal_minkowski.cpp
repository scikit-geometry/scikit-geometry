#include "pygal.hpp"
#include <CGAL/minkowski_sum_2.h>

template <typename T1, typename T2>
Polygon_with_holes_2 get_minkowski(T1 p, T2 q) {
	return CGAL::minkowski_sum_2(p, q);
}

void init_minkowski(py::module & m) {
	auto sub = m.def_submodule("minkowski");
	sub.def("minkowski_sum", &get_minkowski<Polygon_2, Polygon_2>);
	sub.def("minkowski_sum", &get_minkowski<Polygon_with_holes_2, Polygon_2>);
	sub.def("minkowski_sum", &get_minkowski<Polygon_with_holes_2, Polygon_with_holes_2>);
}
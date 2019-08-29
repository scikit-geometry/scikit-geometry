#include "pygal.hpp"

#include <CGAL/create_offset_polygons_2.h>

template <typename T1>
std::vector<Polygon_2 *> get_offset_polygon(T1 p, double q) {
	return CGAL::offset_polygon(p, q);
}

void init_minkowski(py::module & m) {
	auto sub = m.def_submodule("straight_skeleton");
	sub.def("create_exterior_skeleton_and_offset_polygons_with_holes_2", &get_offset_polygon<Polygon_2>);
	sub.def("create_interior_skeleton_and_offset_polygons_with_holes_2", &get_offset_polygon<Polygon_with_holes_2>);
}
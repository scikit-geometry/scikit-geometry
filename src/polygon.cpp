#include "skgeom.hpp"
#include "funcs.hpp"

std::vector<Segment_2> get_edges(Polygon_2 p) {
	auto seg_begin = p.edges_begin();
	std::vector<Segment_2> result;
	for(; seg_begin != p.edges_end(); seg_begin++) {
		result.push_back(*seg_begin);
	}
	return result;
}

CGAL::Bounded_side on_side(Polygon_2& poly, Point_2& point) {
    return CGAL::bounded_side_2(poly.vertices_begin(), poly.vertices_end(), point, Kernel());
}

Point_2 centroid(Polygon_2& poly) {
    return CGAL::centroid(poly.vertices_begin(), poly.vertices_end(), CGAL::Dimension_tag<0>());
}

void init_polygon(py::module &m) {

    py::class_<Polygon_2>(m, "Polygon")
        .def(py::init<>())
	    .def(py::init([](const std::vector<Point_2>& vertices) {
            return new Polygon_2(vertices.begin(), vertices.end());
        }))
        .def("push_back", &Polygon_2::push_back)
        .def("append", &Polygon_2::push_back)
        .def_property_readonly("vertices", [](Polygon_2& s) {
            return py::make_iterator(s.vertices_begin(), s.vertices_end());
        }, py::keep_alive<0, 1>())
        .def_property_readonly("edges", [](Polygon_2& s) {
            return py::make_iterator(s.edges_begin(), s.edges_end()); 
        }, py::keep_alive<0, 1>())
    	.def("__len__", &Polygon_2::size)
    	.def("is_simple", &Polygon_2::is_simple)
    	.def("is_convex", &Polygon_2::is_convex)
    	.def("orientation", &Polygon_2::orientation)
        .def("reverse_orientation", &Polygon_2::reverse_orientation)
    	.def("bbox", &Polygon_2::bbox)
    	.def("area", &Polygon_2::area)
        .def("oriented_side", &Polygon_2::oriented_side)
        .def("__repr__", &toString<Polygon_2>)
    ;

    m.def("centroid", &centroid);

    py::class_<Polygon_with_holes_2>(m, "PolygonWithHoles")
        .def(py::init([](const Polygon_2& outer, const std::vector<Polygon_2>& holes) {
            return new Polygon_with_holes_2(outer, holes.begin(), holes.end());
        }))
        .def("outer_boundary", py::overload_cast<>(&Polygon_with_holes_2::outer_boundary))
        .def_property_readonly("holes", [](const Polygon_with_holes_2& p) {
            return py::make_iterator(p.holes_begin(), p.holes_end());
        }, py::keep_alive<0, 1>())
        .def("__repr__", &toString<Polygon_with_holes_2>)
    ;
}
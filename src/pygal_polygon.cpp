#include "pygal.hpp"
#include "funcs.hpp"

std::vector<Segment_2> get_edges(Polygon_2 p) {
	auto seg_begin = p.edges_begin();
	std::vector<Segment_2> result;
	for(; seg_begin != p.edges_end(); seg_begin++) {
		result.push_back(*seg_begin);
	}
	return result;
}

void make_polygon(Polygon_2 & poly, std::vector<Point_2> vertices) {
    new (&poly) Polygon_2(vertices.begin(), vertices.end());
};

void make_polygon_with_holes(Polygon_with_holes_2 & poly, Polygon_2 outer, std::vector<Polygon_2> holes) {
    new (&poly) Polygon_with_holes_2(outer, holes.begin(), holes.end());
};


CGAL::Bounded_side on_side(Polygon_2 & poly, Point_2 & point) {
    return CGAL::bounded_side_2(poly.vertices_begin(), poly.vertices_end(), point, Kernel());
}

Point_2 poly_center(Polygon_2 & poly) {
    return CGAL::centroid(poly.vertices_begin(), poly.vertices_end(), CGAL::Dimension_tag<0>());
}

void init_polygon(py::module &m) {

    py::class_<Polygon_2>(m, "Polygon")
        .def(py::init<>())
	    .def("__init__", &make_polygon)
        .def("push_back", &Polygon_2::push_back)
        .def("append", &Polygon_2::push_back)
        .def_property_readonly("vertices", [](py::object s) { 
            return PySequenceIterator<Polygon_2::Vertex_iterator>(
                s.cast<const Polygon_2 &>().vertices_begin(), 
                s.cast<const Polygon_2 &>().vertices_end(), 
                s); 
        }, py::return_value_policy::copy)
        .def_property_readonly("edges", [](py::object s) { 
            return PySequenceIterator<Polygon_2::Edge_const_iterator>(
                s.cast<const Polygon_2 &>().edges_begin(), 
                s.cast<const Polygon_2 &>().edges_end(), 
                s); 
        }, py::return_value_policy::copy)
    	.def("__len__", &Polygon_2::size)
    	.def("is_simple", &Polygon_2::is_simple)
    	.def("is_convex", &Polygon_2::is_convex)
    	.def("orientation", &Polygon_2::orientation)
    	.def("bbox", &Polygon_2::bbox)
    	.def("area", &Polygon_2::area)
        .def("__repr__", &toString<Polygon_2>)
    ;

    py::class_<Polygon_with_holes_2>(m, "Polygon_with_holes")
        .def("__init__", &make_polygon_with_holes)
        .def("outer_boundary", [](const Polygon_with_holes_2 & p) -> Polygon_2 {
            return p.outer_boundary();
        })
        .def_property_readonly("holes", [](py::object s) { 
            return PySequenceIterator<Polygon_with_holes_2::Hole_iterator>(
                s.cast<Polygon_with_holes_2 &>().holes_begin(), 
                s.cast<Polygon_with_holes_2 &>().holes_end(), 
                s
            ); 
        })
        .def("__repr__", &toString<Polygon_with_holes_2>)
    ;

    typedef PySequenceIterator<Polygon_2::Vertex_iterator> PyVertexIterator;
    typedef PySequenceIterator<Polygon_2::Edge_const_iterator> PyEdgeIterator;
    typedef PySequenceIterator<Polygon_with_holes_2::Hole_iterator> PyHoleIterator;
    
    py::class_<PyVertexIterator>(m, "VertexIterator")
        .def("__iter__", [](PyVertexIterator &it) -> PyVertexIterator& { return it; })
        .def("__len__", &PyVertexIterator::len)
        .def("__next__", &PyVertexIterator::next);

    py::class_<PyEdgeIterator>(m, "EdgeIterator")
        .def("__iter__", [](PyEdgeIterator &it) -> PyEdgeIterator& { return it; })
        .def("__len__", &PyEdgeIterator::len)
        .def("__next__", &PyEdgeIterator::next);

    py::class_<PyHoleIterator>(m, "HoleIterator")
        .def("__iter__", [](PyHoleIterator &it) -> PyHoleIterator& { return it; })
        .def("__len__", &PyHoleIterator::len)
        .def("__next__", &PyHoleIterator::next);

}
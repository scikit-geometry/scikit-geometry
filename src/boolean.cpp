#include "pygal.hpp"

#include <CGAL/Boolean_set_operations_2.h>

template<typename T1, typename T2>
std::vector<Polygon_with_holes_2> difference(T1 a, T2 b) {
	std::vector<Polygon_with_holes_2> result;
	CGAL::difference(a, b, std::back_inserter(result));
	return result;
}

template<typename T1, typename T2>
py::object join(T1 a, T2 b) {
	Polygon_with_holes_2 result;
	bool joined = CGAL::join(a, b, result);
	if(!joined) {
		return py::none();
	}
	return py::cast(result);
}

template<typename T1, typename T2>
std::vector<Polygon_with_holes_2> symmetric_difference(T1 a, T2 b) {
	std::vector<Polygon_with_holes_2> result;
	CGAL::symmetric_difference(a, b, std::back_inserter(result));
	return result;
}

template<typename T1, typename T2>
std::vector<Polygon_with_holes_2> poly_intersect(T1 a, T2 b) {
    std::vector<Polygon_with_holes_2> result;
    CGAL::intersection(a, b, std::back_inserter(result));
    return result;
}

void init_boolean_set(py::module & m) {
    py::module submodule = m.def_submodule("boolean_set");

    submodule.def("join", &join<Polygon_2, Polygon_2>);
    submodule.def("join", &join<Polygon_with_holes_2, Polygon_2>);
    submodule.def("join", &join<Polygon_with_holes_2, Polygon_with_holes_2>);

    submodule.def("difference", &difference<Polygon_2, Polygon_2>);
    submodule.def("difference", &difference<Polygon_with_holes_2, Polygon_2>);
    submodule.def("difference", &difference<Polygon_with_holes_2, Polygon_with_holes_2>);

    submodule.def("symmetric_difference", &symmetric_difference<Polygon_2, Polygon_2>);
    submodule.def("symmetric_difference", &symmetric_difference<Polygon_with_holes_2, Polygon_2>);
    submodule.def("symmetric_difference", &symmetric_difference<Polygon_with_holes_2, Polygon_with_holes_2>);

    submodule.def("intersect", &poly_intersect<Polygon_2, Polygon_2>);
    submodule.def("intersect", &poly_intersect<Polygon_with_holes_2, Polygon_2>);
    submodule.def("intersect", &poly_intersect<Polygon_with_holes_2, Polygon_with_holes_2>);
}

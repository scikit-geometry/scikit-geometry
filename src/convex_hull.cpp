#include "skgeom.hpp"
#include "funcs.hpp"
#include <CGAL/ch_graham_andrew.h>

void init_convex_hull(py::module &m) {
	py::module sub = m.def_submodule("convex_hull");

	sub.def("graham_andrew", [](const std::vector<Point_2> & in) -> std::vector<Point_2> {
		std::vector<Point_2> out;
		CGAL::ch_graham_andrew(in.begin(), in.end(), std::back_inserter(out));
		return out;
	});
}
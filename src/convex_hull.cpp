#include "skgeom.hpp"
#include "funcs.hpp"

#include <pybind11/numpy.h>

#include <CGAL/ch_graham_andrew.h>

void init_convex_hull(py::module &m) {
	py::module sub = m.def_submodule("convex_hull");

	sub.def("graham_andrew", [](const std::vector<Point_2> & in) -> std::vector<Point_2> {
		std::vector<Point_2> out;
		CGAL::ch_graham_andrew(in.begin(), in.end(), std::back_inserter(out));
		return out;
	});
	sub.def("graham_andrew", [](const py::array_t<double>& in) -> std::vector<Point_2> {
		std::vector<Point_2> in_pts;
		std::vector<Point_2> out;
		auto r = in.unchecked<2>();
		if (r.shape(1) != 2) {
			throw std::runtime_error("points array needs to be 2 dimensional");
		}
		const ssize_t n = r.shape(0);

		for (ssize_t i = 0; i < n; i++) {
			in_pts.push_back(Point_2(r(i, 0), r(i, 1)));
		}
		CGAL::ch_graham_andrew(in_pts.begin(), in_pts.end(), std::back_inserter(out));
		return out;
	});
}
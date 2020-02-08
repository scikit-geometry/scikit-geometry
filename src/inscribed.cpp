#include "skgeom.hpp"
#include "funcs.hpp"

#include <CGAL/Largest_empty_iso_rectangle_2.h>
#include <CGAL/extremal_polygon_2.h>
#include <CGAL/Cartesian.h>

typedef CGAL::Largest_empty_iso_rectangle_2<Kernel>         Largest_empty_iso_rectangle_2;
typedef CGAL::Cartesian<double>  	                        CartesianDoubleKernel;

void init_inscribed(py::module &m) {

    py::class_<Largest_empty_iso_rectangle_2>(m, "LargestEmptyIsoRectangle")
        .def(py::init<Iso_rectangle_2>())
        .def(py::init<Point_2, Point_2>())
        .def("push_back", &Largest_empty_iso_rectangle_2::push_back)
        .def("insert", [](Largest_empty_iso_rectangle_2& r, std::vector<Point_2>& points) {
            r.insert(points.begin(), points.end());
        })
        .def("remove", &Largest_empty_iso_rectangle_2::remove)
        .def("clear", &Largest_empty_iso_rectangle_2::clear)
        .def_property_readonly("left", [](Largest_empty_iso_rectangle_2& r) {
            return r.get_left_bottom_right_top().get<0>();
        })
        .def_property_readonly("bottom", [](Largest_empty_iso_rectangle_2& r) {
            return r.get_left_bottom_right_top().get<1>();
        })
        .def_property_readonly("right", [](Largest_empty_iso_rectangle_2& r) {
            return r.get_left_bottom_right_top().get<2>();
        })
        .def_property_readonly("top", [](Largest_empty_iso_rectangle_2& r) {
            return r.get_left_bottom_right_top().get<3>();
        })
        .def_property_readonly("largest_empty_iso_rectangle", [](Largest_empty_iso_rectangle_2& r) {
            return r.get_largest_empty_iso_rectangle();
        })
    ;

    m.def("maximum_area_inscribed_k_gon", [](std::vector<Point_2>& p, int k) {
		Polygon_2 k_gon;
		CGAL::maximum_area_inscribed_k_gon_2(
			p.begin(), p.end(), k, std::back_inserter(k_gon));
		return new Polygon_2(k_gon);
    });

    m.def("maximum_perimeter_inscribed_k_gon", [](const std::vector<Point_2>& points, int k) {
		// maximum_perimeter_inscribed_k_gon needs a kernel with sqrt; which is
		// why we switch to CGAL::Cartesian<double> here.

		std::vector<CartesianDoubleKernel::Point_2> cartesian_p;
		for (const auto &p : points) {
			cartesian_p.push_back(CartesianDoubleKernel::Point_2(
				CGAL::to_double(p.x()), CGAL::to_double(p.y())));
		}

		CGAL::Polygon_2<CartesianDoubleKernel> k_gon;
		CGAL::maximum_perimeter_inscribed_k_gon_2(
			cartesian_p.begin(), cartesian_p.end(), k, std::back_inserter(k_gon));

		std::vector<Kernel::Point_2> result_p;
		for (auto i = k_gon.vertices_begin(); i != k_gon.vertices_end(); i++) {
			result_p.push_back(Kernel::Point_2(i->x(), i->y()));
		}

		return new Polygon_2(result_p.begin(), result_p.end());
    });
}

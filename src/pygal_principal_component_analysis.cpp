#include "pygal.hpp"
#include <CGAL/centroid.h>
#include <CGAL/barycenter.h>
#include <CGAL/linear_least_squares_fitting_2.h>
#include <CGAL/bounding_box.h>

typedef double                      DoubleFT;
typedef CGAL::Simple_cartesian<DoubleFT>  DoubleK;

typedef CGAL::Cartesian_converter<Kernel,DoubleK> Kernel_To_DoubleK;
typedef CGAL::Cartesian_converter<DoubleK,Kernel> DoubleK_To_Kernel;

void init_principal_component_analysis(py::module & m) {
	py::module sub = m.def_submodule("principal_component_analysis");

	sub.def("centroid", [](std::vector<Point_2> & points) -> Point_2 {
		return CGAL::centroid(points.begin(), points.end(), CGAL::Dimension_tag<0>());
	});
	
	sub.def("barycenter", [](std::vector<std::pair<Point_2, Kernel::FT> > & weighted_points) -> Point_2 {
		return CGAL::barycenter(weighted_points.begin(), weighted_points.end());
	});

	Kernel_To_DoubleK kernel_to_doublek;
	DoubleK_To_Kernel doublek_to_kernel;

	sub.def("linear_least_squares_fitting", [doublek_to_kernel, kernel_to_doublek](std::vector<Point_2> & points) -> Line_2 {
		DoubleK::Line_2 line_out;
		std::vector<DoubleK::Point_2> simple_points;
		for(auto p : points) {
			simple_points.push_back(kernel_to_doublek(p));
		}

		CGAL::linear_least_squares_fitting_2(
			simple_points.begin(), simple_points.end(), 
			line_out, CGAL::Dimension_tag<0>()
		);
		return doublek_to_kernel(line_out);
	});

	sub.def("bounding_box", [](std::vector<Point_2> & points) -> Iso_rectangle_2 {;
		return CGAL::bounding_box(points.begin(), points.end());
	});

}
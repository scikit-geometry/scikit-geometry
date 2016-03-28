#include "pygal.hpp"
#include "funcs.hpp"

struct Intersection_visitor {
  typedef py::object result_type;
  template<typename T>
  result_type operator()(const T &t) const {
    return py::cast(t);
  }
};

template<typename T1, typename T2>
py::object intersect (T1 a, T2 b) {
	auto result = CGAL::intersection(a, b);
	if(!result) {
		// return None
		return py::object();
	}
	return boost::apply_visitor(Intersection_visitor(), *result);
}

template<typename T1, typename T2>
double sqdpy(T1 p, T2 q) {
	return (double) CGAL::to_double(CGAL::squared_distance(p, q));
}

// Polygon_with_holes_2 minkowski_sum(Polygon_with_holes_2 a, Polygon_2 b) {
// 	return CGAL::minkowski_sum_2(a, b);
// }

// Polygon_2 connect_holes(Polygon_with_holes_2 p) {
//     std::vector<Point_2> pts;
//     CGAL::connect_holes(p, std::back_inserter(pts));
//     return Polygon_2(pts.begin(), pts.end());
// }

template<typename T1, typename T2>
bool do_intersect(T1 p, T2 q) {
    return CGAL::do_intersect(p, q);
}

// Polygon_2 offset_polygon(Polygon_2 p, double offset) {
//   PolygonPtrVector inner_offset_polygons = 
//     CGAL::create_interior_skeleton_and_offset_polygons_2(offset, p);
//   return *inner_offset_polygons[0];
// }

// typedef std::vector<Traits_Polygon_2> Traits_Polygon_list;
// typedef std::vector<Polygon_2> Polygon_list;
// Polygon_list get_optimal_convex_partition(Polygon_2 p) {
//     Traits_Polygon_list partition_polys;
//     CGAL::optimal_convex_partition_2(p.vertices_begin(),
//                                 p.vertices_end(),
//                                 std::back_inserter(partition_polys));
//     Polygon_list result;
//     for (Traits_Polygon_2 poly : partition_polys) {
//         result.push_back(Polygon_2(poly.vertices_begin(), poly.vertices_end()));
//     }
//     return result;
//     // return partition_polys;
// }

void init_pygal_kernel(py::module &m) {
    // py::class_<CGAL::Epeck>(m, "Epeck");
    py::class_<CGAL::Lazy_exact_nt<CGAL::Gmpq> >(m, "Lazy_exact_nt_Gmpq")
        .def(py::init<double>());
    // py::class_<CGAL::Gmpq>(m, "GMPQ")
    //     .def(py::init<double>());


    py::class_<Vector_2>(m, "Vector") 
    	.def(py::init<Point_2, Point_2>())
        .def(py::init<Ray_2>())
        .def(py::init<Segment_2>())
        .def(py::init<Line_2>())
        .def("squared_length", &Vector_2::squared_length)
        .def("direction", &Vector_2::direction)
        .def("perpendicular", &Vector_2::perpendicular)
        .def("cartesian", &Vector_2::cartesian)
        .def("x", &Vector_2::x)
        .def("y", &Vector_2::y)
        .def("hx", &Vector_2::hx)
        .def("hy", &Vector_2::hy)
        .def("hw", &Vector_2::hw)
        .def(py::self * double())
        .def(py::self + Vector_2())
        .def(py::self - Vector_2())
        .def(double() * py::self)
        .def(py::self / double())
        .def(-py::self)
        .def("__repr__", &toString<Vector_2>)
    ;
    py::class_<Direction_2>(m, "Direction")
        .def(py::init<Vector_2>())
        .def(py::init<Line_2>())
        .def(py::init<Ray_2>())
        .def(py::init<Segment_2>())
        .def(py::init<double, double>())
        .def("to_vector", &Direction_2::to_vector)
        .def("delta", &Direction_2::delta)
        .def("dx", &Direction_2::dx)
        .def("dy", &Direction_2::dy)
        .def(-py::self)
        .def(py::self == Direction_2())
        .def(py::self != Direction_2())
        .def(py::self <= Direction_2())
        .def(py::self >= Direction_2())
        .def(py::self < Direction_2())
        .def(py::self > Direction_2())
        .def("counterclockwise_in_between", &Direction_2::counterclockwise_in_between)
        .def("__repr__", &toString<Direction_2>)
    ;

    py::class_<Bbox_2>(m, "Bbox")
    	.def(py::init<double, double, double, double>())
    ;

    py::class_<Line_2>(m, "Line")
    	.def(py::init<Point_2, Point_2>())
    	.def(py::init<Segment_2>())
    	.def(py::init<Point_2, Direction_2>())
    	.def(py::init<double, double, double>())
    	.def(py::init<Ray_2>())
        .def("__repr__", &toString<Line_2>)
        .def("to_vector", &Line_2::to_vector)
        .def("x_at_y", &Line_2::x_at_y)
        .def("y_at_x", &Line_2::y_at_x)
        .def("to_vector", &Line_2::to_vector)
        .def("direction", &Line_2::direction)
        .def("opposite", &Line_2::opposite)
        .def("perpendicular", &Line_2::perpendicular)
        .def("transform", &Line_2::transform)
        .def("is_horizontal", &Line_2::is_horizontal)
        .def("is_vertical", &Line_2::is_vertical)
        .def("is_degenerate", &Line_2::is_degenerate)
        .def("has_on", &Line_2::has_on)
        .def("has_on_positive_side", &Line_2::has_on_positive_side)
        .def("has_on_negative_side", &Line_2::has_on_negative_side)
        .def(py::self == Line_2())
        .def(py::self != Line_2())
    ;

    py::class_<Point_2>(m, "Point")
        .def(py::init<double, double>())
        .def("x", &Point_2::x)
        .def("y", &Point_2::y)
        .def("hx", &Point_2::hx)
        .def("hy", &Point_2::hy)
        .def("hw", &Point_2::hw)
        .def("dimension", &Point_2::dimension)
        .def("bbox", &Point_2::bbox)
        .def(py::self - Point_2())
        .def(py::self + Vector_2())
        .def(py::self - Vector_2())
        .def(py::self == Point_2())
        .def(py::self != Point_2())
        .def("__repr__", &toString<Point_2>)
    ;

    py::class_<Iso_rectangle_2>(m, "Iso_Rectangle")
        .def(py::init<Point_2, Point_2>())
        .def(py::init<Point_2, Point_2, Point_2, Point_2>())
        .def(py::init<Point_2, Point_2, int>())
        .def(py::init<Bbox_2>())
        .def("vertex", &Iso_rectangle_2::vertex)
        .def("__getitem__", &Iso_rectangle_2::vertex)
        .def("min", &Iso_rectangle_2::min)
        .def("max", &Iso_rectangle_2::max)
        .def("xmin", &Iso_rectangle_2::xmin)
        .def("xmax", &Iso_rectangle_2::xmax)
        .def("ymin", &Iso_rectangle_2::ymin)
        .def("ymax", &Iso_rectangle_2::ymax)
        .def("area", &Iso_rectangle_2::area)
        .def("bbox", &Iso_rectangle_2::bbox)
        .def("transform", &Iso_rectangle_2::transform)
        .def("min_coord", &Iso_rectangle_2::min_coord)
        .def("max_coord", &Iso_rectangle_2::max_coord)
        .def("is_degenerate", &Iso_rectangle_2::is_degenerate)
        .def("bounded_side", &Iso_rectangle_2::bounded_side)
        .def("has_on_boundary", &Iso_rectangle_2::has_on_boundary)
        .def("has_on_bounded_side", &Iso_rectangle_2::has_on_bounded_side)
        .def("has_on_unbounded_side", &Iso_rectangle_2::has_on_unbounded_side)
        .def(py::self == Iso_rectangle_2())
        .def(py::self != Iso_rectangle_2())
        .def("__repr__", &toString<Iso_rectangle_2>)
    ;


	py::class_<CGAL::Rotation>(m, "Rotation");
	py::class_<CGAL::Translation>(m, "Translation");
	py::class_<CGAL::Scaling>(m, "Scaling");
	py::class_<CGAL::Reflection>(m, "Reflection");

	m.attr("ROTATION") = py::cast(CGAL::ROTATION);
	m.attr("TRANSLATION") = py::cast(CGAL::TRANSLATION);
	m.attr("SCALING") = py::cast(CGAL::SCALING);
	m.attr("REFLECTION") = py::cast(CGAL::REFLECTION);

    py::class_<Segment_2>(m, "Segment")
        .def(py::init<Point_2, Point_2>())
        .def("source", &Segment_2::source)
        .def("target", &Segment_2::target)
        .def("squared_length", &Segment_2::squared_length)
        .def("point", &Segment_2::point)
        .def("min", &Segment_2::min)
        .def("max", &Segment_2::max)
        .def("supporting_line", &Segment_2::supporting_line)
        .def("direction", &Segment_2::direction)
        .def("to_vector", &Segment_2::to_vector)
        .def("opposite", &Segment_2::opposite)
        .def("is_degenerate", &Segment_2::is_degenerate)
        .def("is_horizontal", &Segment_2::is_horizontal)
        .def("is_vertical", &Segment_2::is_vertical)
        .def("has_on", &Segment_2::has_on)
        .def("bbox", &Segment_2::bbox)
        .def("collinear_has_on", &Segment_2::collinear_has_on)
        .def("transform", &Segment_2::transform)
        .def("__repr__", &toString<Segment_2>)
        .def("__getitem__", &Segment_2::operator[])
        .def(py::self == Segment_2())
        .def(py::self != Segment_2())
    ;

    py::class_<Transformation_2>(m, "Transformation")
 		.def(py::init<>())
 		.def(py::init<CGAL::Rotation, double, double>())
 		.def(py::init<CGAL::Rotation, Direction_2, double>())
 		.def(py::init<CGAL::Translation, Vector_2>())
 		.def(py::init<CGAL::Scaling, double>())
 		.def(py::init<double, double, double, double, double, double, double>())
 		.def(py::init<double, double, double, double, double>())
 		.def(py::self * Transformation_2())
        
        .def("transform", static_cast<Point_2 (Transformation_2::*)(const Point_2 &) const>(&Transformation_2::transform))
        .def("transform", static_cast<Line_2 (Transformation_2::*)(const Line_2 &) const>(&Transformation_2::transform))
        .def("transform", static_cast<Direction_2 (Transformation_2::*)(const Direction_2 &) const>(&Transformation_2::transform))
        .def("transform", static_cast<Vector_2 (Transformation_2::*)(const Vector_2 &) const>(&Transformation_2::transform))

        .def("__call__", static_cast<Point_2 (Transformation_2::*)(const Point_2 &) const>(&Transformation_2::transform))
        .def("__call__", static_cast<Line_2 (Transformation_2::*)(const Line_2 &) const>(&Transformation_2::transform))
        .def("__call__", static_cast<Direction_2 (Transformation_2::*)(const Direction_2 &) const>(&Transformation_2::transform))
        .def("__call__", static_cast<Vector_2 (Transformation_2::*)(const Vector_2 &) const>(&Transformation_2::transform))
        .def("__repr__", &toString<Transformation_2>)
	;

    // TODO add all combinations
    m.def("squared_distance", &sqdpy<Point_2, Point_2>);
    m.def("squared_distance", &sqdpy<Line_2, Point_2>);
    m.def("squared_distance", &sqdpy<Line_2, Line_2>);
    m.def("squared_distance", &sqdpy<Segment_2, Point_2>);
    m.def("squared_distance", &sqdpy<Line_2, Segment_2>);

    m.def("do_intersect", &do_intersect<Line_2, Line_2>);
    m.def("do_intersect", &do_intersect<Line_2, Segment_2>);
    m.def("do_intersect", &do_intersect<Segment_2, Segment_2>);
    m.def("do_intersect", &do_intersect<Ray_2, Line_2>);
    m.def("do_intersect", &do_intersect<Ray_2, Segment_2>);


    m.def("intersection", &intersect<Line_2, Line_2>);
    m.def("intersection", &intersect<Line_2, Segment_2>);
    m.def("intersection", &intersect<Segment_2, Segment_2>);
    m.def("intersection", &intersect<Ray_2, Segment_2>);
    // py::def("connect_holes", &connect_holes);
    // py::def("offset_polygon", &offset_polygon);

    // py::def("minkowski", &minkowski_sum);

    // py::def("optimal_convex_partition", &get_optimal_convex_partition);

    m.def("to_double", &to_double<CGAL::Lazy_exact_nt<CGAL::Gmpq> >);

    // TODO get implicit conversions to work
    py::implicitly_convertible<py::float_, CGAL::Lazy_exact_nt<CGAL::Gmpq> >();
    py::implicitly_convertible<py::int_, Kernel::FT>();

}
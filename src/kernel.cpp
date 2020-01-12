#include "skgeom.hpp"
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
        return py::none();
    }
    return boost::apply_visitor(Intersection_visitor(), *result);
}

template<typename T1, typename T2>
double sqdpy(T1 p, T2 q) {
    return CGAL::to_double(CGAL::squared_distance(p, q));
}

template<typename T1, typename T2>
bool do_intersect(T1 p, T2 q) {
    return CGAL::do_intersect(p, q);
}

#define SINGLE_OP(T, op)       \
    .def(py::self op double()) \
    .def(py::self op T())      \
    .def(double() op py::self) \
    .def(T() op py::self)      \

#define OPERATORS(T) \
    SINGLE_OP(T, +)  \
    SINGLE_OP(T, -)  \
    SINGLE_OP(T, *)  \
    SINGLE_OP(T, /)  \
    SINGLE_OP(T, ==) \
    SINGLE_OP(T, !=) \
    SINGLE_OP(T, >)  \
    SINGLE_OP(T, >=) \
    SINGLE_OP(T, <)  \
    SINGLE_OP(T, <=) \
    .def(-py::self)  \
    .def(+py::self)  \


void init_skgeom_kernel(py::module &m) {

    py::class_<Kernel::FT>(m, "FieldNumberType")
        .def(py::init<double>())
        OPERATORS(Kernel::FT)
        .def("__repr__", &toString<Kernel::FT>)
        .def("__float__", to_double<Kernel::FT>)
    ;

    // py::class_<Kernel::RT>(m, "RingNumberType")
    //     .def(py::init<double>())
    //     OPERATORS(Kernel::RT)
    //     .def("__repr__", &toString<Kernel::RT>)
    //     .def("__float__", to_double<Kernel::RT>)
    // ;

    py::implicitly_convertible<py::float_, Kernel::FT>();
    py::implicitly_convertible<py::int_, Kernel::FT>();

    py::implicitly_convertible<py::float_, Kernel::RT>();
    py::implicitly_convertible<py::int_, Kernel::RT>();

    py::class_<Point_2>(m, "Point2")
        .def(py::init<double, double>())
        .def(py::init<int, int>())
        .def(py::init<Kernel::RT, Kernel::RT, Kernel::RT>())
        .def(py::init<Kernel::FT, Kernel::FT>())
        .def(py::init<int, int>())
        .def("x", &Point_2::x)
        .def("y", &Point_2::y)
        .def("hx", &Point_2::hx)
        .def("hy", &Point_2::hy)
        .def("hw", &Point_2::hw)
        .def("dimension", &Point_2::dimension)
        .def("bbox", &Point_2::bbox)
        .def("transform", &Point_2::transform)
        .def(py::self < Point_2())
        .def(py::self <= Point_2())
        .def(py::self > Point_2())
        .def(py::self >= Point_2())
        .def(py::self - Point_2())
        .def(py::self + Vector_2())
        .def(py::self - Vector_2())
        .def(py::self += Vector_2())
        .def(py::self -= Vector_2())
        .def(py::self == Point_2())
        .def(py::self != Point_2())
        .def("__repr__", &toString<Point_2>)
    ;

    py::class_<Vector_2>(m, "Vector2") 
        .def(py::init<double, double>())
        .def(py::init<int, int>())
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
        .def("transform", &Vector_2::transform)
        .def(-py::self)
        .def(py::self * double())
        .def(double() * py::self)
        .def(py::self / double())
        .def(py::self + Vector_2())
        .def(py::self - Vector_2())
        .def(py::self += Vector_2())
        .def(py::self -= Vector_2())
        .def(py::self == Vector_2())
        .def(py::self != Vector_2())
        .def("__repr__", &toString<Vector_2>)
    ;

    py::class_<Ray_2>(m, "Ray2") 
        .def(py::init<Point_2, Point_2>())
        .def(py::init<Point_2, Direction_2>())
        .def(py::init<Point_2, Vector_2>())
        .def(py::init<Point_2, Line_2>())
        .def("source", &Ray_2::source)
        .def("point", &Ray_2::point)
        .def("direction", &Ray_2::direction)
        .def("to_vector", &Ray_2::to_vector)
        .def("supporting_line", &Ray_2::supporting_line)
        .def("opposite", &Ray_2::opposite)
        .def("is_degenerate", &Ray_2::is_degenerate)
        .def("is_horizontal", &Ray_2::is_horizontal)
        .def("is_vertical", &Ray_2::is_vertical)
        .def("has_on", &Ray_2::has_on)
        .def("collinear_has_on", &Ray_2::collinear_has_on)
        .def("transform", &Ray_2::transform)
        .def(py::self == Ray_2())
        .def(py::self != Ray_2())
        .def("__repr__", &toString<Ray_2>)
    ;

    py::class_<Direction_2>(m, "Direction2")
        .def(py::init<Vector_2>())
        .def(py::init<Line_2>())
        .def(py::init<Ray_2>())
        .def(py::init<Segment_2>())
        .def(py::init<double, double>())
        .def("to_vector", &Direction_2::to_vector)
        .def("delta", &Direction_2::delta)
        .def("dx", &Direction_2::dx)
        .def("dy", &Direction_2::dy)
        .def("counterclockwise_in_between", &Direction_2::counterclockwise_in_between)
        .def(-py::self)
        .def(py::self == Direction_2())
        .def(py::self != Direction_2())
        .def(py::self <= Direction_2())
        .def(py::self >= Direction_2())
        .def(py::self < Direction_2())
        .def(py::self > Direction_2())
        .def("__repr__", &toString<Direction_2>)
    ;

    py::class_<Bbox_2>(m, "Bbox2")
    	.def(py::init<>())
        .def(py::init<double, double, double, double>())
        .def("xmin", &Bbox_2::xmin)
        .def("xmax", &Bbox_2::xmax)
        .def("ymin", &Bbox_2::ymin)
        .def("ymax", &Bbox_2::ymax)
        .def("min", &Bbox_2::min)
        .def("max", &Bbox_2::max)
        // this one seems to be a recent addition
        #if CGAL_VERSION_NR >= CGAL_VERSION_NUMBER(4, 14, 0)
        .def("dilate", &Bbox_2::dilate)
        #endif
        .def("dimension", &Bbox_2::dimension)
        .def("__repr__", &toString<Bbox_2>)
        .def(py::self + Bbox_2())
        .def(py::self += Bbox_2())
        .def(py::self == Bbox_2())
        .def(py::self != Bbox_2())
    ;

    m.def("do_overlap", py::overload_cast<const Bbox_2&, const Bbox_2&>(&CGAL::do_overlap));

    py::class_<Line_2>(m, "Line2")
        // TODO check if RT != double
        // .def(py::init<Kernel::RT, Kernel::RT, Kernel::RT>())
    	.def(py::init<double, double, double>())
    	.def(py::init<Point_2, Point_2>())
    	.def(py::init<Point_2, Direction_2>())
        .def(py::init<Point_2, Vector_2>())
    	.def(py::init<Segment_2>())
    	.def(py::init<Ray_2>())

        .def("a", &Line_2::a)
        .def("b", &Line_2::b)
        .def("c", &Line_2::c)
        .def("point", py::overload_cast<int>(&Line_2::point, py::const_))
        .def("projection", &Line_2::projection)
        .def("x_at_y", &Line_2::x_at_y)
        .def("y_at_x", &Line_2::y_at_x)

        .def("is_degenerate", &Line_2::is_degenerate)
        .def("is_horizontal", &Line_2::is_horizontal)
        .def("is_vertical", &Line_2::is_vertical)
        .def("oriented_side", &Line_2::oriented_side)

        .def("has_on", &Line_2::has_on)
        .def("has_on_positive_side", &Line_2::has_on_positive_side)
        .def("has_on_negative_side", &Line_2::has_on_negative_side)

        .def("to_vector", &Line_2::to_vector)
        .def("direction", &Line_2::direction)
        .def("opposite", &Line_2::opposite)
        .def("perpendicular", &Line_2::perpendicular)
        .def("transform", &Line_2::transform)

        .def(py::self == Line_2())
        .def(py::self != Line_2())

        .def("__repr__", &toString<Line_2>)
    ;

    py::class_<Iso_rectangle_2>(m, "IsoRectangle2")
        .def(py::init<Point_2, Point_2>())
        .def(py::init<Point_2, Point_2, Point_2, Point_2>())
        .def(py::init<Point_2, Point_2, int>())
        .def(py::init<Bbox_2>())

        .def("vertex", &Iso_rectangle_2::vertex)
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

        .def("__getitem__", &Iso_rectangle_2::vertex)
        .def("__repr__", &toString<Iso_rectangle_2>)

        .def(py::self == Iso_rectangle_2())
        .def(py::self != Iso_rectangle_2())
    ;

    py::class_<Segment_2>(m, "Segment2")
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
        .def("collinear_has_on", &Segment_2::collinear_has_on)

        .def("bbox", &Segment_2::bbox)
        .def("transform", &Segment_2::transform)
        .def("__repr__", &toString<Segment_2>)
        .def("__getitem__", &Segment_2::operator[])

        .def(py::self == Segment_2())
        .def(py::self != Segment_2())
    ;

    py::class_<Circle_2>(m, "Circle2") 
        .def(py::init<Point_2, Kernel::FT, CGAL::Orientation>())
        .def(py::init<Point_2, Point_2, Point_2>())
        .def(py::init<Point_2, Point_2, CGAL::Orientation>())
        .def(py::init<Point_2, CGAL::Orientation>())

        .def("center", &Circle_2::center)
        .def("squared_radius", &Circle_2::squared_radius)
        .def("orientation", &Circle_2::orientation)
        .def(py::self == Circle_2())
        .def(py::self != Circle_2())

        .def("is_degenerate", &Circle_2::is_degenerate)
        .def("oriented_side", &Circle_2::oriented_side)
        .def("bounded_side", &Circle_2::bounded_side)
        .def("has_on_positive_side", &Circle_2::has_on_positive_side)
        .def("has_on_negative_side", &Circle_2::has_on_negative_side)
        .def("has_on_boundary", &Circle_2::has_on_boundary)
        .def("has_on_bounded_side", &Circle_2::has_on_bounded_side)
        .def("has_on_unbounded_side", &Circle_2::has_on_unbounded_side)

        .def("opposite", &Circle_2::opposite)
        .def("orthogonal_transform", &Circle_2::orthogonal_transform)
        .def("bbox", &Circle_2::bbox)

        .def("__repr__", &toString<Circle_2>)
    ;

    py::class_<Transformation_2>(m, "Transformation2")
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

    py::class_<Point_3>(m, "Point3")
        .def(py::init<>())
        .def(py::init<Point_3>())
        .def(py::init<CGAL::Origin>())
        .def(py::init<double, double, double>())
        .def(py::init<double, double, double, double>())
        .def("__repr__", &toString<Point_3>)
        .def("x", &Point_3::x)
        .def("y", &Point_3::y)
        .def("z", &Point_3::z)
        .def("hx", &Point_3::hx)
        .def("hy", &Point_3::hy)
        .def("hz", &Point_3::hz)
        .def("hw", &Point_3::hw)
        .def("dimension", &Point_3::dimension)
        .def("homogeneous", &Point_3::homogeneous)
        .def("bbox", &Point_3::bbox)
        .def("__getitem__", &Point_3::operator[])
        .def("cartesian", &Point_3::cartesian)
        .def("transform", &Point_3::transform)
        .def(py::self == Point_3())
        .def(py::self != Point_3())
        .def(py::self < Point_3())
        .def(py::self > Point_3())
        .def(py::self <= Point_3())
        .def(py::self >= Point_3())
        .def(py::self + Vector_3())
        .def(py::self - Point_3())
        .def(py::self - Vector_3())
        .def(py::self - CGAL::Origin())
    ;

    py::class_<Plane_3>(m, "Plane3")
        .def(py::init<>())
        .def(py::init<Plane_3>())
        .def(py::init<Point_3, Point_3, Point_3>())
        .def(py::init<Point_3, Direction_3>())
        .def(py::init<Point_3, Vector_3>())
        .def(py::init<SKGEOM_FT, SKGEOM_FT, SKGEOM_FT, SKGEOM_FT>())
        .def(py::init<double, double, double, double>())
        .def(py::init<Line_3, Point_3>())
        .def(py::init<Segment_3, Point_3>())
        .def(py::init<Ray_3, Point_3>())
        .def("a", &Plane_3::a)
        .def("b", &Plane_3::b)
        .def("c", &Plane_3::c)
        .def("d", &Plane_3::d)
        .def("perpendicular_line", &Plane_3::perpendicular_line)
        .def("opposite", &Plane_3::opposite)
        .def("point", &Plane_3::point)
        .def("projection", &Plane_3::projection)
        .def("orthogonal_vector", &Plane_3::orthogonal_vector)
        .def("orthogonal_direction", &Plane_3::orthogonal_direction)
        .def("base1", &Plane_3::base1)
        .def("base2", &Plane_3::base2)
        .def("to_2d", &Plane_3::to_2d)
        .def("to_3d", &Plane_3::to_3d)
        .def("transform", &Plane_3::transform)
        .def("oriented_side", &Plane_3::oriented_side)
        .def("has_on_positive_side", &Plane_3::has_on_positive_side)
        .def("has_on_negative_side", &Plane_3::has_on_negative_side)
        .def("has_on", (bool (Plane_3::*)(const Point_3&) const)&Plane_3::has_on)
        .def("has_on", (bool (Plane_3::*)(const Line_3&) const)&Plane_3::has_on)
        .def("is_degenerate", &Plane_3::is_degenerate)
        .def("__repr__", &toString<Plane_3>)
        .def(py::self == Plane_3())
        .def(py::self != Plane_3())
    ;

    py::class_<Segment_3>(m, "Segment3")
        .def(py::init<>())
        .def(py::init<Segment_3>())
        .def(py::init<Point_3, Point_3>())
        .def("source", &Segment_3::source)
        .def("target", &Segment_3::target)
        .def("min", &Segment_3::min)
        .def("max", &Segment_3::max)
        .def("vertex", &Segment_3::vertex)
        .def("point", &Segment_3::point)
        .def("has_on", &Segment_3::has_on)
        .def("squared_length", &Segment_3::squared_length)
        .def("is_degenerate", &Segment_3::is_degenerate)
        .def("bbox", &Segment_3::bbox)
        .def("to_vector", &Segment_3::to_vector)
        .def("supporting_line", &Segment_3::supporting_line)
        .def("opposite", &Segment_3::opposite)
        .def("transform", &Segment_3::transform)
        .def("direction", &Segment_3::direction)
        .def("__getitem__", &Segment_3::operator[])
        .def("__repr__", &toString<Segment_3>)
        .def(py::self == Segment_3())
        .def(py::self != Segment_3())
    ;

    py::class_<Vector_3>(m, "Vector3")
        .def(py::init<>())
        .def(py::init<Vector_3>())
        .def(py::init<CGAL::Null_vector>())
        .def(py::init<Point_3, Point_3>())
        .def(py::init<Segment_3>())
        .def(py::init<Ray_3>())
        .def(py::init<Line_3>())
        .def(py::init<SKGEOM_FT, SKGEOM_FT, SKGEOM_FT>())
        .def(py::init<SKGEOM_FT, SKGEOM_FT, SKGEOM_FT, SKGEOM_FT>())
        .def(py::init<double, double, double>())
        .def(py::init<double, double, double, double>())
        .def("x", &Vector_3::x)
        .def("y", &Vector_3::y)
        .def("z", &Vector_3::z)
        .def("hx", &Vector_3::hx)
        .def("hy", &Vector_3::hy)
        .def("hz", &Vector_3::hz)
        .def("hw", &Vector_3::hw)
        .def("cartesian", &Vector_3::cartesian)
        .def("homogeneous", &Vector_3::homogeneous)
        .def("dimension", &Vector_3::dimension)
        .def("squared_length", &Vector_3::squared_length)
        .def("direction", &Vector_3::direction)
        .def("transform", &Vector_3::transform)
        .def("__getitem__", &Vector_3::operator[])
        .def("__repr__", &toString<Vector_3>)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(-py::self)
        .def(py::self - py::self)
        .def(py::self + py::self)
        .def(py::self * py::self)
        .def(py::self * double())
        .def(double() * py::self)
        .def(py::self / double())
        .def(CGAL::Origin() + py::self)
    ;

    py::class_<Ray_3>(m, "Ray3")
        .def(py::init<>())
        .def(py::init<Ray_3&>())
        .def(py::init<Point_3, Point_3>())
        .def(py::init<Point_3, Vector_3>())
        .def(py::init<Point_3, Direction_3>())
        .def(py::init<Point_3, Line_3>())
        .def("source", &Ray_3::source)
        .def("point", &Ray_3::point)
        .def("is_degenerate", &Ray_3::is_degenerate)
        .def("direction", &Ray_3::direction)
        .def("to_vector", &Ray_3::to_vector)
        .def("has_on", &Ray_3::has_on)
        .def("opposite", &Ray_3::opposite)
        .def("supporting_line", &Ray_3::supporting_line)
        .def("transform", &Ray_3::transform)
        .def("__repr__", &toString<Ray_3>)
        .def("start", &Ray_3::start)
        .def("second_point", &Ray_3::second_point)
        .def(py::self == py::self)
        .def(py::self != py::self)
    ;

    py::class_<Triangle_3>(m, "Triangle3")
        .def(py::init<>())
        .def(py::init<Triangle_3>())
        .def(py::init<Point_3, Point_3, Point_3>())
        .def("supporting_plane", &Triangle_3::supporting_plane)
        .def("transform", &Triangle_3::transform)
        .def("has_on", &Triangle_3::has_on)
        .def("is_degenerate", &Triangle_3::is_degenerate)
        .def("vertex", &Triangle_3::vertex)
        .def("bbox", &Triangle_3::bbox)
        .def("squared_area", &Triangle_3::squared_area)
        .def("__repr__", &toString<Triangle_3>)
        .def(py::self == Triangle_3())
        .def(py::self != Triangle_3())
    ;

    // TODO add all combinations
    m.def("squared_distance", &sqdpy<Point_2, Point_2>);
    m.def("squared_distance", &sqdpy<Line_2, Line_2>);
    m.def("squared_distance", &sqdpy<Segment_2, Segment_2>);
    m.def("squared_distance", &sqdpy<Ray_2, Ray_2>);
    m.def("squared_distance", &sqdpy<Line_2, Point_2>);
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
}
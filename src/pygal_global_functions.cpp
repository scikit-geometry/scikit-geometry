#include "pygal.hpp"

typedef Kernel K;
using namespace CGAL;

void init_global_functions(py::module &m) {

    py::enum_<CGAL::Sign>(m, "Sign")
        .value("NEGATIVE", CGAL::Sign::NEGATIVE)
        .value("ZERO",     CGAL::Sign::ZERO)
        .value("POSITIVE", CGAL::Sign::POSITIVE)
    ;

    py::enum_<CGAL::Bounded_side>(m, "Bounded_side")
        .value("ON_BOUNDED_SIDE",   CGAL::Bounded_side::ON_BOUNDED_SIDE)
        .value("ON_BOUNDARY",       CGAL::Bounded_side::ON_BOUNDARY)
        .value("ON_UNBOUNDED_SIDE", CGAL::Bounded_side::ON_UNBOUNDED_SIDE)
    ;

    py::enum_<CGAL::Comparison_result>(m, "Comparison_result")
        .value("SMALLER",   CGAL::Comparison_result::SMALLER)
        .value("EQUAL",     CGAL::Comparison_result::EQUAL)
        .value("LARGER", 	CGAL::Comparison_result::LARGER)
    ;

    py::enum_<CGAL::Angle>(m, "Angle")
        .value("OBTUSE",    CGAL::Angle::OBTUSE)
        .value("RIGHT",     CGAL::Angle::RIGHT)
        .value("ACUTE", 	CGAL::Angle::ACUTE)
    ;

    py::enum_<CGAL::Orientation>(m, "Orientation")
    	.value("COLLINEAR", CGAL::COLLINEAR)
    	.value("LEFT_TURN", CGAL::LEFT_TURN)
    	.value("RIGHT_TURN", CGAL::RIGHT_TURN)
    	.value("CLOCKWISE", CGAL::CLOCKWISE)
    	.value("COUNTERCLOCKWISE", CGAL::COUNTERCLOCKWISE)
    ;

    // m.attr("ORIGIN") = py::cast(CGAL::ORIGIN);
    // m.attr("NULL_VECTOR") = py::cast(CGAL::NULL_VECTOR);

    m.def("angle", static_cast<CGAL::Angle (*)(const K::Vector_2&, const K::Vector_2&)>(CGAL::angle));
    m.def("angle", static_cast<CGAL::Angle (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::angle));
    m.def("angle", static_cast<CGAL::Angle (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::angle));

    m.def("area", static_cast<K::FT (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::area));

    m.def("are_ordered_along_line", static_cast<bool (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::are_ordered_along_line));

    m.def("are_strictly_ordered_along_line", static_cast<bool (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::are_strictly_ordered_along_line));

    m.def("barycenter", [](K::Point_2 & p, double w1, K::Point_2 & q, double w2) -> K::Point_2 {
    	return barycenter(p, w1, q, w2);
    });
    m.def("barycenter", [](K::Point_2 & p, double w1, K::Point_2 & q, double w2) -> K::Point_2 {
    	return barycenter(p, w1, q, w2);
    });
    m.def("barycenter", [](K::Point_2 & p, double w1, K::Point_2 & q, double w2, K::Point_2 & r, double w3) -> K::Point_2 {
    	return barycenter(p, w1, q, w2, r, w3);
    });
    m.def("barycenter", [](K::Point_2 & p, double w1, K::Point_2 & q, double w2, K::Point_2 & r, double w3, K::Point_2 & s, double w4) -> K::Point_2 {
    	return barycenter(p, w1, q, w2, r, w3, s, w4);
    });

    m.def("bisector", static_cast<K::Line_2 (*)(const K::Point_2&, const K::Point_2&)>(bisector));
    // m.def("bisector", static_cast<K::Line_2 (*)(const K::Line_2&, const K::Line_2&)>(bisector));

    m.def("centroid", static_cast<K::Point_2 (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(centroid));
    m.def("centroid", static_cast<K::Point_2 (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&, const K::Point_2&)>(centroid));
    m.def("centroid", static_cast<K::Point_2 (*)(const K::Triangle_2&)>(centroid));
    
    m.def("circumcenter", static_cast<K::Point_2 (*)(const K::Point_2&, const K::Point_2&)>(circumcenter));
    m.def("circumcenter", static_cast<K::Point_2 (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(circumcenter));
    m.def("circumcenter", static_cast<K::Point_2 (*)(const K::Triangle_2&)>(circumcenter));

    m.def("collinear_are_ordered_along_line", static_cast<bool (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(collinear_are_ordered_along_line));
    m.def("collinear_are_strictly_ordered_along_line", static_cast<bool (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(collinear_are_ordered_along_line));

    m.def("collinear", static_cast<bool (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(collinear));

}
#include "skgeom.hpp"

#include <CGAL/Origin.h>

#include <CGAL/centroid.h>
#include <CGAL/barycenter.h>

typedef Kernel K;

void init_global_functions(py::module &m) {

    py::class_<CGAL::Rotation>(m, "Rotation");
    py::class_<CGAL::Translation>(m, "Translation");
    py::class_<CGAL::Scaling>(m, "Scaling");
    py::class_<CGAL::Reflection>(m, "Reflection");

    m.attr("ROTATION") = py::cast(CGAL::ROTATION);
    m.attr("TRANSLATION") = py::cast(CGAL::TRANSLATION);
    m.attr("SCALING") = py::cast(CGAL::SCALING);
    m.attr("REFLECTION") = py::cast(CGAL::REFLECTION);

    py::enum_<CGAL::Bounded_side>(m, "BoundedSide")
        .value("ON_BOUNDED_SIDE",   CGAL::Bounded_side::ON_BOUNDED_SIDE)
        .value("ON_BOUNDARY",       CGAL::Bounded_side::ON_BOUNDARY)
        .value("ON_UNBOUNDED_SIDE", CGAL::Bounded_side::ON_UNBOUNDED_SIDE)
        .export_values()
    ;

    // Orientation, Oriented_side, and Comparison_result are actually only
    // typedefs for sign.

    py::enum_<CGAL::Sign>(m, "Sign", py::module_local())
        .value("NEGATIVE", CGAL::Sign::NEGATIVE)
        .value("ZERO",     CGAL::Sign::ZERO)
        .value("POSITIVE", CGAL::Sign::POSITIVE)
        .value("SMALLER", CGAL::Comparison_result::SMALLER)
        .value("EQUAL",   CGAL::Comparison_result::EQUAL)
        .value("LARGER",  CGAL::Comparison_result::LARGER)
        .value("ON_NEGATIVE_SIDE",     CGAL::ON_NEGATIVE_SIDE)
        .value("ON_ORIENTED_BOUNDARY", CGAL::ON_ORIENTED_BOUNDARY)
        .value("ON_POSITIVE_SIDE", CGAL::ON_POSITIVE_SIDE)
        .value("COLLINEAR",        CGAL::COLLINEAR)
        .value("COPLANAR",        CGAL::COPLANAR)
        .value("DEGENERATE",        CGAL::DEGENERATE)
        .value("LEFT_TURN",        CGAL::LEFT_TURN)
        .value("RIGHT_TURN",       CGAL::RIGHT_TURN)
        .value("CLOCKWISE",        CGAL::CLOCKWISE)
        .value("COUNTERCLOCKWISE", CGAL::COUNTERCLOCKWISE)
        .export_values()
    ;

    py::enum_<CGAL::Angle>(m, "Angle", py::module_local())
        .value("OBTUSE", CGAL::Angle::OBTUSE)
        .value("RIGHT",  CGAL::Angle::RIGHT)
        .value("ACUTE",  CGAL::Angle::ACUTE)
        .export_values()
    ;

    py::class_<CGAL::Origin>(m, "Origin");
    py::class_<CGAL::Null_vector>(m, "NullVector");

    m.attr("ORIGIN") = py::cast(CGAL::ORIGIN);
    m.attr("NULL_VECTOR") = py::cast(CGAL::NULL_VECTOR);

    m.def("angle", static_cast<CGAL::Angle (*)(const K::Vector_2&, const K::Vector_2&)>(CGAL::angle));
    m.def("angle", static_cast<CGAL::Angle (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::angle));
    m.def("angle", static_cast<CGAL::Angle (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::angle));

    m.def("area", static_cast<K::FT (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::area));

    m.def("are_ordered_along_line", static_cast<bool (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::are_ordered_along_line));

    m.def("are_strictly_ordered_along_line", static_cast<bool (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::are_strictly_ordered_along_line));

    m.def("barycenter", static_cast<K::Point_2 (*)(const K::Point_2&, const K::FT&, const K::Point_2&, const K::FT&)>(CGAL::barycenter));
    m.def("barycenter", static_cast<K::Point_2 (*)(const K::Point_2&, const K::FT&, const K::Point_2&, const K::FT&, const K::Point_2&, const K::FT&)>(CGAL::barycenter));
    m.def("barycenter", static_cast<K::Point_2 (*)(const K::Point_2&, const K::FT&, const K::Point_2&, const K::FT&, const K::Point_2&, const K::FT&, const K::Point_2&, const K::FT&)>(CGAL::barycenter));
    m.def("barycenter", [](const std::vector<std::pair<Point_2, Kernel::FT>>& weighted_points) -> Point_2 {
        return CGAL::barycenter(weighted_points.begin(), weighted_points.end());
    });

    m.def("bisector", static_cast<K::Line_2 (*)(const K::Point_2&, const K::Point_2&)>(CGAL::bisector));

    // TODO test if Kernel::RT supports `SQRT`
    // m.def("bisector", static_cast<K::Line_2 (*)(const K::Line_2&, const K::Line_2&)>(bisector));

    m.def("centroid", static_cast<K::Point_2 (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::centroid));
    m.def("centroid", static_cast<K::Point_2 (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::centroid));
    m.def("centroid", static_cast<K::Point_2 (*)(const K::Triangle_2&)>(CGAL::centroid));
    
    m.def("centroid", [](const std::vector<Point_2>& points) -> Point_2 {
        // TODO fix hard coded dimension
        return CGAL::centroid(points.begin(), points.end(), CGAL::Dimension_tag<0>());
    });

    m.def("circumcenter", static_cast<K::Point_2 (*)(const K::Point_2&, const K::Point_2&)>(CGAL::circumcenter));
    m.def("circumcenter", static_cast<K::Point_2 (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::circumcenter));
    m.def("circumcenter", static_cast<K::Point_2 (*)(const K::Triangle_2&)>(CGAL::circumcenter));

    m.def("collinear_are_ordered_along_line", static_cast<bool (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::collinear_are_ordered_along_line));
    m.def("collinear_are_strictly_ordered_along_line", static_cast<bool (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::collinear_are_ordered_along_line));

    m.def("collinear", static_cast<bool (*)(const K::Point_2&, const K::Point_2&, const K::Point_2&)>(CGAL::collinear));

}
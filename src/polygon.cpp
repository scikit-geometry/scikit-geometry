#include "skgeom.hpp"
#include "funcs.hpp"

#include <pybind11/numpy.h>

#include <CGAL/Random.h>
#include <CGAL/random_polygon_2.h>
#include <CGAL/point_generators_2.h>

std::vector<Segment_2> get_edges(Polygon_2 p) {
	auto seg_begin = p.edges_begin();
	std::vector<Segment_2> result;
	for(; seg_begin != p.edges_end(); seg_begin++) {
		result.push_back(*seg_begin);
	}
	return result;
}

CGAL::Bounded_side on_side(Polygon_2& poly, Point_2& point) {
    return CGAL::bounded_side_2(poly.vertices_begin(), poly.vertices_end(), point, Kernel());
}

Point_2 centroid(Polygon_2& poly) {
    return CGAL::centroid(poly.vertices_begin(), poly.vertices_end(), CGAL::Dimension_tag<0>());
}

void build_polygon_from_vertices(Polygon_2& poly, const py::array_t<double>& vertices) {
    auto r = vertices.unchecked<2>();
    if (r.shape(1) != 2) {
        throw std::runtime_error("vertices need to be 2 dimensional");
    }
    const ssize_t n = r.shape(0);

    for (ssize_t i = 0; i < n; i++) {
        poly.push_back(Point_2(r(i, 0), r(i, 1)));
    } 
}

void init_polygon(py::module &m) {

    py::class_<Polygon_2>(m, "Polygon")
        .def(py::init<>())
	    .def(py::init([](const std::vector<Point_2>& vertices) {
            return new Polygon_2(vertices.begin(), vertices.end());
        }))
	    .def(py::init([](const py::array_t<double> &vertices) {
            Polygon_2 *polygon = new Polygon_2;
            build_polygon_from_vertices(*polygon, vertices);
            return polygon;
        }))
        .def("push_back", &Polygon_2::push_back)
        .def("append", &Polygon_2::push_back)
        .def_property_readonly("vertices", [](Polygon_2& s) {
            return py::make_iterator(s.vertices_begin(), s.vertices_end());
        }, py::keep_alive<0, 1>())
        .def_property_readonly("edges", [](Polygon_2& s) {
            return py::make_iterator(s.edges_begin(), s.edges_end()); 
        }, py::keep_alive<0, 1>())
        .def_property_readonly("coords", [](Polygon_2& s) {
            py::array_t<double, py::array::c_style> coords;
            coords.resize({s.size(), size_t(2)});
            auto r = coords.mutable_unchecked<2>();
            size_t i = 0;
            for (auto v = s.vertices_begin(); v != s.vertices_end(); v++, i++) {
                r(i, 0) = CGAL::to_double(v->x());
                r(i, 1) = CGAL::to_double(v->y());
            }
            assert(i == s.size());
            return coords;
        })
    	.def("__len__", &Polygon_2::size)
    	.def("is_simple", &Polygon_2::is_simple)
    	.def("is_convex", &Polygon_2::is_convex)
    	.def("orientation", &Polygon_2::orientation)
        .def("reverse_orientation", &Polygon_2::reverse_orientation)
    	.def("bbox", &Polygon_2::bbox)
    	.def("area", &Polygon_2::area)
        .def("oriented_side", &Polygon_2::oriented_side)
        .def("__repr__", &toString<Polygon_2>)
        .def("_ipython_display_", [](Polygon_2& s) {
            py::module::import("skgeom.draw").attr("draw_polygon")(
                s, py::arg("facecolor") = "lightgray");
        })
    ;

    m.def("centroid", &centroid);

    m.def("random_polygon", [](int n, const std::string &shape, double size, unsigned int seed) {
        std::unique_ptr<Polygon_2> polygon(new Polygon_2);
        auto inserter = std::back_inserter(*polygon);

        boost::optional<CGAL::Random> custom_random;
        if (seed != 0) {
            custom_random = CGAL::Random(seed);
        }
        auto &random = (seed == 0) ? CGAL::get_default_random() : *custom_random;

        if (shape == "square") {
            CGAL::random_polygon_2(n, inserter,
                CGAL::Random_points_in_square_2<Point_2>(size, random));
        } else if (shape == "disc") {
            CGAL::random_polygon_2(n, inserter,
                CGAL::Random_points_in_disc_2<Point_2>(size, random));
        } else if (shape == "circle") {
            CGAL::random_polygon_2(n, inserter,
                CGAL::Random_points_on_circle_2<Point_2>(size, random));
        } else {
            throw std::runtime_error("unsupported shape " + shape);
        }
        return polygon.release();
    }, py::arg("n") = 7, py::arg("shape") = "square", py::arg("size") = 0.5, py::arg("seed") = 0);

    py::class_<Polygon_with_holes_2>(m, "PolygonWithHoles")
        .def(py::init([](const Polygon_2& outer, const std::vector<Polygon_2>& holes) {
            return new Polygon_with_holes_2(outer, holes.begin(), holes.end());
        }))
        .def(py::init([](const py::array_t<double>& outer, const py::list holes) {
            Polygon_2 outer_poly;
            build_polygon_from_vertices(outer_poly, outer);
            std::vector<Polygon_2> holes_poly(holes.size());
            for (size_t i = 0; i < holes.size(); i++) {
                build_polygon_from_vertices(
                    holes_poly[i], holes[i].cast<py::array_t<double>>());
            }
            return new Polygon_with_holes_2(outer_poly, holes_poly.begin(), holes_poly.end());
        }))
        .def("outer_boundary", py::overload_cast<>(&Polygon_with_holes_2::outer_boundary))
        .def_property_readonly("holes", [](const Polygon_with_holes_2& p) {
            return py::make_iterator(p.holes_begin(), p.holes_end());
        }, py::keep_alive<0, 1>())
        .def("__repr__", &toString<Polygon_with_holes_2>)
        .def("_ipython_display_", [](Polygon_with_holes_2& s) {
            py::module::import("skgeom.draw").attr("draw_polygon")(
                s.outer_boundary(),
                py::arg("polygon_with_holes") = s,
                py::arg("facecolor") = "lightgray");
        })
    ;
}
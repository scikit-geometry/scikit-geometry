#include "skgeom.hpp"
#include "funcs.hpp"

#include <CGAL/Boolean_set_operations_2.h>

template<typename T>
Polygon_set_2 *join(const Polygon_set_2 &a, const T &b) {
    std::unique_ptr<Polygon_set_2> r(new Polygon_set_2);
    r->join(a, Polygon_set_2(b));
    return r.release();
}

template<typename T>
Polygon_set_2 *intersection(const Polygon_set_2 &a, const T &b) {
    std::unique_ptr<Polygon_set_2> r(new Polygon_set_2);
    r->intersection(a, Polygon_set_2(b));
    return r.release();
}

template<typename T>
Polygon_set_2 *difference(const Polygon_set_2 &a, const T &b) {
    std::unique_ptr<Polygon_set_2> r(new Polygon_set_2);
    r->difference(a, Polygon_set_2(b));
    return r.release();
}

template<typename T>
Polygon_set_2 *symmetric_difference(const Polygon_set_2 &a, const T &b) {
    std::unique_ptr<Polygon_set_2> r(new Polygon_set_2);
    r->symmetric_difference(a, Polygon_set_2(b));
    return r.release();
}

template<typename T>
bool intersects(const Polygon_set_2 &set, const T &other) {
    return Polygon_set_2(set).do_intersect(other); 
}

std::list<Polygon_with_holes_2> py_polygons_to_list(py::list py_polygons) {
    std::list<Polygon_with_holes_2> polygons;
    const size_t n = py_polygons.size();
    for (size_t i = 0; i < n; i++) {
        py::object polygon = py_polygons[i];
        if (py::isinstance<Polygon_2>(polygon)) {
            polygons.push_back(
                Polygon_with_holes_2(polygon.cast<Polygon_2>()));
        } else if (py::isinstance<Polygon_with_holes_2>(polygon)) {
            polygons.push_back(
                polygon.cast<Polygon_with_holes_2>());
        } else {
            std::ostringstream err;
            err << "encountered illegal polygon type at index " << i;
            throw std::runtime_error(err.str());
        }
    }
    return polygons;
}

void init_polygon_set(py::module &m) {

    py::class_<Polygon_set_2>(m, "PolygonSet")
        .def(py::init<>())
        .def(py::init([] (const Polygon_2 &polygon) {
            return new Polygon_set_2(polygon);
        }))
        .def(py::init([] (const Polygon_with_holes_2 &polygon) {
            return new Polygon_set_2(polygon);
        }))
	    .def(py::init([](py::list polygons, bool disjoint = false) {
            std::list<Polygon_with_holes_2> joined;
            if (disjoint) {
                joined = py_polygons_to_list(polygons);
            } else {
                std::list<Polygon_with_holes_2> input(py_polygons_to_list(polygons));
                CGAL::join(input.begin(), input.end(), std::back_inserter(joined));
            }

            std::unique_ptr<Polygon_set_2> set(new Polygon_set_2);
            for (const auto &p : joined) {
                set->insert(p);
            }
            return set.release();
        }), py::arg("polygons"), py::arg("disjoint") = false)

        .def("join", &join<Polygon_2>)
        .def("join", &join<Polygon_with_holes_2>)
        .def("join", &join<Polygon_set_2>)

        .def("union", &join<Polygon_2>)
        .def("union", &join<Polygon_with_holes_2>)
        .def("union", &join<Polygon_set_2>)

        .def("intersection", &intersection<Polygon_2>)
        .def("intersection", &intersection<Polygon_with_holes_2>)
        .def("intersection", &intersection<Polygon_set_2>)

        .def("difference", &difference<Polygon_2>)
        .def("difference", &difference<Polygon_with_holes_2>)
        .def("difference", &difference<Polygon_set_2>)

        .def("symmetric_difference", &symmetric_difference<Polygon_2>)
        .def("symmetric_difference", &symmetric_difference<Polygon_with_holes_2>)
        .def("symmetric_difference", &symmetric_difference<Polygon_set_2>)

        .def_property_readonly("complement", [] (const Polygon_set_2 &set) {
            std::unique_ptr<Polygon_set_2> r(new Polygon_set_2());
            r->complement(set);
            return r.release();
        })

        .def("intersects", &intersects<Polygon_2>)
        .def("intersects", &intersects<Polygon_with_holes_2>)
        .def("intersects", &intersects<Polygon_set_2>)

        .def_property_readonly("is_empty", [] (const Polygon_set_2 &set) {
            return set.is_empty();
        })
        .def_property_readonly("is_plane", [] (const Polygon_set_2 &set) {
            return set.is_plane();
        })

        .def("__len__", [] (const Polygon_set_2 &set) {
            return set.number_of_polygons_with_holes();
        })

        .def_property_readonly("polygons", [] (const Polygon_set_2 &set) {
            std::list<Polygon_with_holes_2> polygons;
            set.polygons_with_holes(std::back_inserter(polygons));
            py::list result;
            for (const auto &poly : polygons) {
                result.append(new Polygon_with_holes_2(poly));
            }
            return result;
        })

        .def("oriented_side", [] (const Polygon_set_2 &set, const Point_2 &p) {
            return set.oriented_side(p);
        })
        .def("oriented_side", [] (const Polygon_set_2 &set, const Polygon_2 &p) {
            return set.oriented_side(p);
        })
        .def("oriented_side", [] (const Polygon_set_2 &set, const Polygon_with_holes_2 &p) {
            return set.oriented_side(p);
        })
        .def("oriented_side", [] (const Polygon_set_2 &set, const Polygon_set_2 &other) {
            return set.oriented_side(other);
        })

        .def("locate", [] (const Polygon_set_2 &set, const Point_2 &p) -> py::object {
            std::unique_ptr<Polygon_with_holes_2> r(new Polygon_with_holes_2);
            if (set.locate(p, *r)) {
                return py::cast(r.release());
            } else {
                return py::none();
            }
        })

        .def("_ipython_display_", [](Polygon_set_2& s) {
            py::module::import("skgeom.draw").attr("draw_polygon_set")(
                s, py::arg("facecolor") = "lightgray");
        })
    ;
}

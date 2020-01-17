#include "skgeom.hpp"
#include "funcs.hpp"

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Constrained_triangulation_plus_2.h>
#include <CGAL/Polyline_simplification_2/Squared_distance_cost.h>
#include <CGAL/Polyline_simplification_2/simplify.h>

namespace Simplification_2 = CGAL::Polyline_simplification_2;

typedef Simplification_2::Squared_distance_cost                           Cost;

typedef Simplification_2::Vertex_base_2<Kernel>                           Vertex_base_2;
typedef CGAL::Constrained_triangulation_face_base_2<Kernel>               Face_base_2;
typedef CGAL::Triangulation_data_structure_2<Vertex_base_2, Face_base_2>  Triangulation_DS_2;
typedef CGAL::Constrained_Delaunay_triangulation_2<
    Kernel, Triangulation_DS_2, CGAL::Exact_predicates_tag>               CDT_2;
typedef CGAL::Constrained_triangulation_plus_2<CDT_2>                     Triangulation_2;
typedef Triangulation_2::Constraint_id                                    Constraint_id;

std::list<Polygon_with_holes_2> py_polygons_to_list(py::list py_polygons);

Polygon_2 polygon_from_constraint(const Triangulation_2 &ct, const Constraint_id &id) {
    auto begin = ct.points_in_constraint_begin(id);
    auto end = ct.points_in_constraint_end(id);

    // prevent adding degenerate segments, which causes CGAL to bail out when constructing
    // Polygon_set_2 from such polygons.
    if (end != begin) {
        end--;
        if (*begin != *end) {
            end++;
        }
    }

    return Polygon_2(begin, end);
}

class Simplifier {
    py::object _above_cost;
    py::object _below_count_ratio;
    py::object _below_count;
    const bool _preserve_topology;

public:
    Simplifier(bool preserve_topology) :
        _above_cost(py::none()),
        _below_count_ratio(py::none()),
        _below_count(py::none()),
        _preserve_topology(preserve_topology) {
    }

    void configure_stop(py::object value, const std::string &mode) {
        if (mode == "sqeuclidean") {
            _above_cost = value;
        } else if (mode == "ratio") {
            _below_count_ratio = value;
        } else if (mode == "count") {
            _below_count = value;
        } else {
            throw std::runtime_error("unsupported stop criterion '" + mode + "'");
        }
    }

    template<typename R, typename T>
    R _simplify(T &input) const {
        if (!_above_cost.is_none()) {
            return Simplification_2::simplify(input, Cost(), 
                Simplification_2::Stop_above_cost_threshold(_above_cost.cast<double>()));
        } else if (!_below_count_ratio.is_none()) {
            return Simplification_2::simplify(input, Cost(),
                Simplification_2::Stop_below_count_ratio_threshold(_below_count_ratio.cast<double>()));
        } else if (!_below_count.is_none()) {
            return Simplification_2::simplify(input, Cost(),
                Simplification_2::Stop_below_count_threshold(_below_count.cast<size_t>()));
        } else {
            throw std::runtime_error("no valid simplification parameters given");
        }
    }

    std::list<Polygon_with_holes_2> operator()(const std::list<Polygon_with_holes_2> &polygons) const {
        std::list<Polygon_with_holes_2> result;

        if (_preserve_topology) {
            Triangulation_2 ct;

            struct Construction {
                Constraint_id outer_boundary;
                std::vector<Constraint_id> holes;
            };

            std::vector<Construction> constructions;
            constructions.reserve(polygons.size());

            for (const auto &polygon : polygons) {
                Construction c;
                c.outer_boundary = ct.insert_constraint(polygon.outer_boundary());

                c.holes.reserve(polygon.number_of_holes());
                for(auto i = polygon.holes_begin(); i != polygon.holes_end(); i++) {
                    c.holes.emplace_back(ct.insert_constraint(*i));
                }

                constructions.push_back(c);
            }

            _simplify<bool>(ct);

            for (const Construction &c : constructions) {
                std::vector<Polygon_2> holes;
                holes.reserve(c.holes.size());
                for (const Constraint_id &hole_id : c.holes) {
                    holes.emplace_back(polygon_from_constraint(ct, hole_id));
                }

                result.emplace_back(Polygon_with_holes_2(
                    polygon_from_constraint(ct, c.outer_boundary),
                    holes.begin(), holes.end()));
            }
        } else {
            for (const auto &polygon : polygons) {
                result.emplace_back(_simplify<Polygon_with_holes_2>(polygon));
            }
        }

        return result;
    }

    Polygon_2 operator()(const Polygon_2 &polygon) const {
        std::list<Polygon_with_holes_2> polygons;
        polygons.emplace_back(Polygon_with_holes_2(polygon));
        return (*this)(polygons).front().outer_boundary();
    }

    Polygon_with_holes_2 operator()(const Polygon_with_holes_2 &polygon) const {
        std::list<Polygon_with_holes_2> polygons;
        polygons.push_back(polygon);
        return (*this)(polygons).front();
    }
};

void init_simplification(py::module &m) {

    m.def("simplify", [] (Polygon_2 &p,
        py::object threshold, std::string mode, bool preserve_topology) {
        
        Simplifier simplifier(preserve_topology);
        simplifier.configure_stop(threshold, mode);
        return new Polygon_2(simplifier(p));

    }, py::arg("polygon"), py::arg("threshold") = py::cast(0.5), py::arg("mode") = "ratio",
        py::arg("preserve_topology") = true);

    m.def("simplify", [] (Polygon_with_holes_2 &p,
        py::object threshold, std::string mode, bool preserve_topology) {
        
        Simplifier simplifier(preserve_topology);
        simplifier.configure_stop(threshold, mode);
        return new Polygon_with_holes_2(simplifier(p));
    }, py::arg("polygon"), py::arg("threshold") = py::cast(0.5), py::arg("mode") = "ratio",
        py::arg("preserve_topology") = true);

    m.def("simplify", [] (Polygon_set_2 &set, py::object threshold, std::string mode) {
        
        std::list<Polygon_with_holes_2> polygons;
        set.polygons_with_holes(std::back_inserter(polygons));

        Simplifier simplifier(true);
        simplifier.configure_stop(threshold, mode);
        const std::list<Polygon_with_holes_2> simplified = simplifier(polygons);

        std::unique_ptr<Polygon_set_2> output(new Polygon_set_2);
        output->insert(simplified.begin(), simplified.end());

        return output.release();
    }, py::arg("polygon_set"), py::arg("threshold") = py::cast(0.5), py::arg("mode") = "ratio");

    m.def("simplify", [] (
        py::list py_polygons, py::object threshold, std::string mode, bool preserve_topology) {
        
        std::list<Polygon_with_holes_2> polygons = py_polygons_to_list(py_polygons);

        Simplifier simplifier(preserve_topology);
        simplifier.configure_stop(threshold, mode);
        const std::list<Polygon_with_holes_2> simplified = simplifier(polygons);

        py::list output;
        for (const auto &p : simplified) {
            output.append(py::cast(
                new Polygon_with_holes_2(p),
                pybind11::return_value_policy::automatic));
        }

        return output;
    }, py::arg("polygons"), py::arg("threshold") = py::cast(0.5), py::arg("mode") = "ratio",
        py::arg("preserve_topology") = true);
}

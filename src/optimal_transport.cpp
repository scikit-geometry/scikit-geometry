#include "skgeom.hpp"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Optimal_transportation_reconstruction_2.h>
#include <fstream>
#include <iostream>
#include <string>
#include <iterator>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::FT                                               FT;
typedef K::Point_2                                          INEXACT_Point;
typedef K::Segment_2                                        INEXACT_Segment;
typedef CGAL::Optimal_transportation_reconstruction_2<K>    Otr_2;

typedef std::pair<INEXACT_Point, FT>                        PointMassPair;
typedef std::vector<PointMassPair>                          PointMassList;
typedef CGAL::First_of_pair_property_map<PointMassPair>     Point_property_map;
typedef CGAL::Second_of_pair_property_map<PointMassPair>    Mass_property_map;

typedef CGAL::Optimal_transportation_reconstruction_2<K, Point_property_map, Mass_property_map> Otr_2_with_mass;

void make_otr(Otr_2& otr, std::vector<Point_2> points) {
    std::vector<INEXACT_Point> OTR_points;
    OTR_points.reserve(points.size());
    for(const auto& p : points)
    {
        OTR_points.emplace_back(CGAL::to_double(p.x()), CGAL::to_double(p.y()));
    }
    new (&otr) Otr_2(OTR_points);
};

void make_otr_with_weights(Otr_2_with_mass& otr, std::vector<std::tuple<Point_2, double>> points) {
    PointMassList OTR_points;
    OTR_points.reserve(points.size());
    for(const auto& p : points)
        OTR_points.push_back(
            std::make_pair(
                INEXACT_Point(CGAL::to_double(std::get<0>(p).x()),
                              CGAL::to_double(std::get<0>(p).y())),
                std::get<1>(p)
            )
        );
    Point_property_map ppm;
    Mass_property_map mpm;
    new (&otr) Otr_2_with_mass(OTR_points, ppm, mpm);
};


void init_optimal_transport(py::module & m) {
	auto sub = m.def_submodule("optimal_transport");

    py::class_<Otr_2>(sub, "OptimalTransport")
        .def("__init__", &make_otr)
        .def("run", &Otr_2::run)
        .def("output", [](const Otr_2& otr) {
            std::vector<INEXACT_Point> isolated_points;
            std::vector<INEXACT_Segment> segments;

            std::vector<Point_2> isolated_points_skgeom;
            std::vector<Segment_2> segments_skgeom;

            otr.list_output(std::back_inserter(isolated_points),
                            std::back_inserter(segments));

            isolated_points_skgeom.reserve(isolated_points.size());
            segments_skgeom.reserve(segments.size());

            for(const auto& p : isolated_points)
            {
                isolated_points_skgeom.emplace_back(p.x(), p.y());
            }

            for(const auto& s : segments)
            {
                segments_skgeom.emplace_back(
                    Point_2(s.source().x(), s.source().y()),
                    Point_2(s.target().x(), s.target().y())
                );
            }
            return std::make_tuple(isolated_points_skgeom, segments_skgeom);
        })
        .def("relocate_all_points", &Otr_2::relocate_all_points)
    ;

    py::class_<Otr_2_with_mass>(sub, "OptimalTransportWithWeights")
        .def("__init__", &make_otr_with_weights)
        .def("run", &Otr_2_with_mass::run)
        .def("output", [](const Otr_2_with_mass& otr) {
            std::vector<INEXACT_Point> isolated_points;
            std::vector<INEXACT_Segment> segments;

            std::vector<Point_2> isolated_points_skgeom;
            std::vector<Segment_2> segments_skgeom;

            otr.list_output(std::back_inserter(isolated_points),
                            std::back_inserter(segments));

            isolated_points_skgeom.reserve(isolated_points.size());
            segments_skgeom.reserve(segments.size());

            for(const auto& p : isolated_points)
            {
                isolated_points_skgeom.emplace_back(p.x(), p.y());
            }

            for(const auto& s : segments)
            {
                segments_skgeom.emplace_back(
                    Point_2(s.source().x(), s.source().y()),
                    Point_2(s.target().x(), s.target().y())
                );
            }
            return std::make_tuple(isolated_points_skgeom, segments_skgeom);
        })
        .def("relocate_all_points", &Otr_2_with_mass::relocate_all_points)
    ;
}

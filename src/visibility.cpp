#include "skgeom.hpp"

#include <CGAL/Rotational_sweep_visibility_2.h>
#include <CGAL/Triangular_expansion_visibility_2.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>

typedef CGAL::Rotational_sweep_visibility_2<Segment_Arrangement_2>      RSV;
typedef CGAL::Triangular_expansion_visibility_2<Segment_Arrangement_2>          TEV;

void init_visibility(py::module& m) {
    py::class_<RSV>(m, "RotationalSweepVisibility")
        .def(py::init<const Segment_Arrangement_2&>())
        .def("compute_visibility", [](RSV& rsv, Point_2 query_point, Halfedge_handle he) {
            Segment_Arrangement_2 output_arr;
            rsv.compute_visibility(query_point, he, output_arr);
            return output_arr;
        })
        .def("compute_visibility", [](RSV& rsv, Point_2 query_point, Face_handle fh) {
            Segment_Arrangement_2 output_arr;
            rsv.compute_visibility(query_point, fh, output_arr);
            return output_arr;
        })
    ;
    py::class_<TEV>(m, "TriangularExpansionVisibility")
        .def(py::init<const Segment_Arrangement_2&>())
        .def("compute_visibility", [](TEV& tev, Point_2 query_point, Face_handle fh) {
            Segment_Arrangement_2 output_arr;
            tev.compute_visibility(query_point, fh, output_arr);
            // we shoudl try to return the result face handle of tev but lifetime management
            // becomes hard
            return output_arr;
        })
        .def("compute_visibility", [](TEV& tev, Point_2 query_point, Halfedge_handle he) {
            Segment_Arrangement_2 output_arr;
            tev.compute_visibility(query_point, he, output_arr);
            return output_arr;
        })
    ;


    // m.def("compute", [](std::vector<Segment_2> segs) {
    //     Segment_Arrangement_2 arrangement;
    //     std::vector<Segment_2> scopy = segs;
    //     std::vector<Segment_Arrangement_2> rarrs;

    //     for(auto s : scopy)
    //     {
    //         CGAL::insert(arrangement, s);
    //     }
    //     RSV rsv(arrangement);
    //     int i = 0;
    //     for (auto it = arrangement.halfedges_begin(); it != arrangement.halfedges_end(); ++it) {
    //         i++;
    //         if(i > 10) { break; }
    //         if (it->face()->has_outer_ccb())
    //         {
    //             Segment_Arrangement_2 output_arr;
    //             try {
    //                 rsv.compute_visibility(it->target()->point(), it->face(), output_arr);
    //                 std::cout << "got new result" << std::endl;
    //                 rarrs.push_back(output_arr);
    //             }
    //             catch(...) {};
    //         }
    //     }
    //     return rarrs;
    // });
}
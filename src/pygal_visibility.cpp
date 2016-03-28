#include "pygal.hpp"
#include <CGAL/Rotational_sweep_visibility_2.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>

typedef CGAL::Rotational_sweep_visibility_2<Segment_Arrangement_2>      RSV;

void init_visibility(py::module &m) {
    py::class_<RSV>(m, "Rotational_sweep_visibility")
        .def(py::init<Segment_Arrangement_2>())
        .def("compute_visibility", [](RSV & rsv, const Point_2 & query_point, Halfedge_handle he) {
            Segment_Arrangement_2 output_arr;
            rsv.compute_visibility(query_point, he, output_arr);
            return output_arr;
        })
        .def("compute_visibility", [](RSV & rsv, const Point_2 & query_point, Face_handle fh) {
            Segment_Arrangement_2 output_arr;
            rsv.compute_visibility(query_point, fh, output_arr);
            return output_arr;
        })
    ;
}
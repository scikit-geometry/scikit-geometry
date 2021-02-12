#pragma once
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/intersections.h>
#include <CGAL/Bbox_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/squared_distance_2.h> 
#include <CGAL/minkowski_sum_2.h>
#include <CGAL/partition_2.h>
#include <CGAL/connect_holes.h>
#include <CGAL/centroid.h>

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <iostream>

namespace py = pybind11;

typedef CGAL::Exact_predicates_inexact_constructions_kernel   Kernel;

typedef Kernel::FT                                          SKGEOM_FT;
typedef Kernel::RT                                          SKGEOM_RT;

typedef Kernel::Point_2                                     Point_2;
typedef Kernel::Segment_2                                   Segment_2;
typedef Kernel::Vector_2                                    Vector_2;
typedef Kernel::Line_2                                      Line_2;
typedef Kernel::Direction_2                                 Direction_2;
typedef Kernel::Ray_2                                       Ray_2;
typedef Kernel::Iso_rectangle_2                             Iso_rectangle_2;
typedef Kernel::Triangle_2                                  Triangle_2;
typedef Kernel::Aff_transformation_2                        Transformation_2;
typedef Kernel::Circle_2                                    Circle_2;
typedef CGAL::Polygon_2<Kernel>                             Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>                  Polygon_with_holes_2;
typedef CGAL::Polygon_set_2<Kernel>                         Polygon_set_2;

typedef CGAL::Bbox_2                                        Bbox_2;
typedef CGAL::Bbox_3                                        Bbox_3;

typedef Kernel::Point_3                                     Point_3;
typedef Kernel::Vector_3                                    Vector_3;
typedef Kernel::Ray_3                                       Ray_3;
typedef Kernel::Plane_3                                     Plane_3;
typedef Kernel::Line_3                                      Line_3;
typedef Kernel::Direction_3                                 Direction_3;
typedef Kernel::Segment_3                                   Segment_3;
typedef Kernel::Circle_3                                    Circle_3;
typedef Kernel::Sphere_3                                    Sphere_3;
typedef Kernel::Tetrahedron_3                               Tetrahedron_3;
typedef Kernel::Triangle_3                                  Triangle_3;
typedef Kernel::Iso_cuboid_3                                Iso_cuboid_3;
typedef Kernel::Aff_transformation_3                        Aff_transformation_3;


typedef CGAL::Partition_traits_2<Kernel>                    Traits;
typedef CGAL::Is_convex_2<Traits>                           Is_convex_2;
typedef Traits::Point_2                                     Traits_Point_2;
typedef Traits::Polygon_2                                   Traits_Polygon_2;
typedef std::vector<Polygon_with_holes_2>                   Pwh_list_2;

typedef CGAL::Arr_segment_traits_2<Kernel>                              Segment_Traits_2;
typedef CGAL::Arrangement_2<Segment_Traits_2>                           Segment_Arrangement_2;

typedef Segment_Arrangement_2::Halfedge_const_handle                    Halfedge_const_handle;
typedef Segment_Arrangement_2::Vertex_const_handle                      Vertex_const_handle;
typedef Segment_Arrangement_2::Vertex_handle                            Vertex_handle;
typedef Segment_Arrangement_2::Vertex_iterator                          Vertex_iterator;
typedef Segment_Arrangement_2::Vertex_const_iterator                    Vertex_const_iterator;
typedef Segment_Arrangement_2::Vertex                                   Vertex;
typedef Segment_Arrangement_2::Halfedge_handle                          Halfedge_handle;
typedef Segment_Arrangement_2::Halfedge_iterator                        Halfedge_iterator;
typedef Segment_Arrangement_2::Halfedge_const_iterator                  Halfedge_const_iterator;
typedef Segment_Arrangement_2::Face_iterator                            Face_iterator;
typedef Segment_Arrangement_2::Face_const_iterator                      Face_const_iterator;
typedef Segment_Arrangement_2::Face_handle                              Face_handle;
typedef Segment_Arrangement_2::Face_const_handle                        Face_const_handle;
typedef Segment_Arrangement_2::Halfedge                                 Halfedge;
typedef Segment_Arrangement_2::Face                                     Face;
typedef Segment_Arrangement_2::Hole_iterator                            Hole_iterator;


#define CGAL_HOLDER_HELPER(T, H)                                            \
    template <>                                                             \
    struct always_construct_holder<H> : always_construct_holder<void>  { }; \
    template <>                                                             \
    class type_caster<H>                                                    \
        : public copyable_holder_caster<T, H> { };                          \
                                                                            \
    template <>                                                             \
    struct holder_helper<H> {                                               \
        static const T* get(const H &p) { return p.operator->(); }          \
    };                                                                      \

namespace pybind11 { namespace detail {
    CGAL_HOLDER_HELPER(Halfedge, Halfedge_handle);
    CGAL_HOLDER_HELPER(Vertex, Vertex_handle);
    CGAL_HOLDER_HELPER(Face, Face_handle);
}}


namespace {
    // Special iterator data structure for python
    template <class T>
    struct PySequenceIterator
    {
        PySequenceIterator(const T& begin, const T& end, py::object ref) : begin(begin), iter(begin), end(end), ref(ref)
        {
        }
         
        // , ref(ref) {}
        py::object next() {
            if (iter == end)
                throw py::stop_iteration();
            return py::cast(*(iter++));
        }

        int len() {
            return std::distance(begin, end);
        }

        py::object getitem(int i) {
            return py::cast<T>(begin[i]);
        }

        T begin;
        T iter;
        T end;
        py::object ref; // keep a reference
        size_t index = 0;
    };

    template <typename T>
    struct PySequenceCirculator {
        PySequenceCirculator(const T & begin, py::object ref) : begin(begin), iter(begin), ref(ref) {}
        py::object next() {
            return py::cast(*(iter++));
        }

        // int len() {
        //     return std::distance(begin, end);
        // }

        py::object getitem(int i) {
            return py::cast(begin[i]);
        }

        T begin;
        T iter;
        py::object ref; // keep a reference
        size_t index = 0;
    };
};

template<typename T1>
double to_double(T1 v) {
    return CGAL::to_double(v);
}

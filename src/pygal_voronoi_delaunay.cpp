#include "pygal.hpp"

#include <CGAL/Triangulation_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>

#include <CGAL/Voronoi_diagram_2/Handle_adaptor.h>
#include <CGAL/Voronoi_diagram_2/Face.h>

typedef CGAL::Triangulation_2<Kernel>         Triangulation;
typedef Triangulation::Vertex                 T_Vertex;
typedef Triangulation::Vertex_handle          T_Vertex_handle;
typedef Triangulation::Vertex_circulator      Vertex_circulator;
typedef Triangulation::Face_handle            T_Face_handle;


typedef CGAL::Delaunay_triangulation_2<Kernel>                               DT;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<DT>                 AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<DT> AP;
typedef CGAL::Voronoi_diagram_2<DT, AT, AP>                                    VD;

typedef VD::Locate_result             Locate_result;
typedef VD::Vertex_handle             VD_Vertex_handle;
typedef VD::Vertex                    VD_Vertex;
typedef VD::Face_handle               VD_Face_handle;
typedef VD::Face                      VD_Face;
typedef VD::Halfedge_handle           VD_Halfedge_handle;
typedef VD::Halfedge                  VD_Halfedge;
typedef VD::Ccb_halfedge_circulator   VD_Ccb_halfedge_circulator;


namespace pybind11 { namespace detail {

    template <> class type_caster<VD_Halfedge_handle> : public type_caster_cgal_handle<VD_Halfedge, VD_Halfedge_handle> { };
    template <> class type_caster<VD_Face_handle> : public type_caster_cgal_handle<VD_Face, VD_Face_handle> { };
    template <> class type_caster<VD_Vertex_handle> : public type_caster_cgal_handle<VD_Vertex, VD_Vertex_handle> { };
    template <> class type_caster<T_Vertex_handle> : public type_caster_cgal_handle<T_Vertex, T_Vertex_handle> { };
    template <> class type_caster<DT::Vertex_handle> : public type_caster_cgal_handle<DT::Vertex, DT::Vertex_handle> { };

}}


template <typename T>
struct PySequenceIteratorCP {
    PySequenceIteratorCP(const T & begin, const T & end, py::object ref) : begin(begin), iter(begin), end(end), ref(ref) {}
    py::object next() {
        if (iter == end)
            throw py::stop_iteration();
        return py::cast(*(iter++), py::return_value_policy::copy);
    }

    int len() {
        return std::distance(begin, end);
    }

    py::object getitem(int i) {
        return py::cast(begin[i]);
    }

    T begin;
    T iter;
    T end;
    py::object ref; // keep a reference
    size_t index = 0;
};

template <typename T, typename Triangul>
struct PySequenceIteratorTriangulationEdges {
    PySequenceIteratorTriangulationEdges(const T & begin, const T & end, 
        const Triangul& t, py::object ref) : begin(begin), iter(begin), end(end), t(t), ref(ref) {}
    py::object next() {
        if (iter == end)
            throw py::stop_iteration();
        return py::cast(t.segment(iter++), py::return_value_policy::copy);
    }

    int len() {
        return std::distance(begin, end);
    }

    py::object getitem(int i) {
        return py::cast(begin[i]);
    }

    T begin;
    T iter;
    T end;
    Triangul t;
    py::object ref; // keep a reference
    size_t index = 0;
};

void init_voronoi_delaunay(py::module & m) {
    auto sub = m.def_submodule("voronoi");

    py::class_<VD_Face, VD_Face_handle>(sub, "VoronoiFace")
        .def("halfedge", [](const VD_Face_handle& handle) {return *(handle->halfedge());})
        .def("is_unbounded", [](const VD_Face_handle& handle) {return handle->is_unbounded();})
    ;

    py::class_<VD_Halfedge, VD_Halfedge_handle>(sub, "VoronoiHalfedge")
        .def("has_source", [](VD_Halfedge_handle v) { return v->has_source(); })
        .def("has_target", [](VD_Halfedge_handle v) { return v->has_target(); })
        .def("source", [](VD_Halfedge_handle v) -> py::object {
            if(v->has_source())
                return py::cast(*(v->source()));
            else
                return py::none();
        })
        .def("target", [](VD_Halfedge_handle v) -> py::object {
            if(v->has_target())
                return py::cast(*(v->target()));
            else
                return py::none();
        })
        .def("next", [](VD_Halfedge_handle v) { return v->next(); })
    ;

    py::class_<VD_Vertex, VD_Vertex_handle>(sub, "VoronoiVertex")
        .def("point", [](const VD_Vertex& v) { 
            return v.point(); 
        })
    ;

    typedef PySequenceIteratorCP<VD::Vertex_iterator> PyVDVertexIterator;
    typedef PySequenceIteratorCP<VD::Edge_iterator> PyVDEdgeIterator;
    typedef PySequenceIteratorCP<VD::Site_iterator> PyVDSiteIterator;
    
    py::class_<PyVDVertexIterator>(m, "VertexIterator")
        .def("__iter__", [](PyVDVertexIterator &it) -> PyVDVertexIterator& { return it; })
        .def("__len__", &PyVDVertexIterator::len)
        .def("__next__", &PyVDVertexIterator::next);

    py::class_<PyVDEdgeIterator>(m, "EdgeIterator")
        .def("__iter__", [](PyVDEdgeIterator &it) -> PyVDEdgeIterator& { return it; })
        .def("__len__", &PyVDEdgeIterator::len)
        .def("__next__", &PyVDEdgeIterator::next);

    py::class_<PyVDSiteIterator>(m, "SiteIterator")
        .def("__iter__", [](PyVDSiteIterator &it) -> PyVDSiteIterator& { return it; })
        .def("__len__", &PyVDSiteIterator::len)
        .def("__next__", &PyVDSiteIterator::next);

    py::class_<VD>(sub, "VoronoiDiagram")
        .def(py::init<>())
        .def("insert", static_cast<VD_Face_handle (VD::*)(const Point_2 &)>(&VD::insert))
        .def("is_valid", &VD::is_valid)
        .def("clear", &VD::clear)
        .def("number_of_vertices", &VD::number_of_vertices)
        .def("number_of_faces", &VD::number_of_faces)
        .def("number_of_halfedges", &VD::number_of_halfedges)
        .def_property_readonly("vertices", [](const VD& vd) {
            return py::make_iterator(vd.vertices_begin(), vd.vertices_end());
        }, py::keep_alive<0, 1>())
        .def_property_readonly("sites", [](const VD& vd) {
            return py::make_iterator(vd.sites_begin(), vd.sites_end());
        }, py::keep_alive<0, 1>())
        // .def_property_readonly("vertices", [](py::object s) { 
        //     return PySequenceIteratorCP<VD::Vertex_iterator>(
        //         s.cast<const VD &>().vertices_begin(), 
        //         s.cast<const VD &>().vertices_end(), 
        //         s);
        // }, py::return_value_policy::copy)
        // .def_property_readonly("sites", [](py::object s) { 
        //     return PySequenceIteratorCP<VD::Site_iterator>(
        //         s.cast<const VD &>().sites_begin(), 
        //         s.cast<const VD &>().sites_end(), 
        //         s);
        // }, py::return_value_policy::copy)
        .def_property_readonly("edges", [](py::object s) { 
            return PySequenceIteratorCP<VD::Edge_iterator>(
                s.cast<const VD &>().edges_begin(), 
                s.cast<const VD &>().edges_end(), 
                s);
        }, py::return_value_policy::copy)
    ;

    typedef PySequenceIteratorTriangulationEdges<Triangulation::All_edges_iterator,
                                                 Triangulation> PyTAllEdgesIterator;
    typedef PySequenceIteratorTriangulationEdges<Triangulation::Finite_edges_iterator,
                                                 Triangulation> PyTFiniteEdgesIterator;
    
    py::class_<PyTAllEdgesIterator>(m, "TriangulationEdgesIterator")
        .def("__iter__", [](PyTAllEdgesIterator &it) -> PyTAllEdgesIterator& { return it; })
        .def("__len__", &PyTAllEdgesIterator::len)
        .def("__next__", &PyTAllEdgesIterator::next);;

    py::class_<PyTFiniteEdgesIterator>(m, "TriangulationFiniteEdgesIterator")
        .def("__iter__", [](PyTFiniteEdgesIterator &it) -> PyTFiniteEdgesIterator& { return it; })
        .def("__len__", &PyTFiniteEdgesIterator::len)
        .def("__next__", &PyTFiniteEdgesIterator::next);;

    py::class_<T_Vertex, T_Vertex_handle>(sub, "TriangulationVertex")
        .def("point", [](const T_Vertex& v) {return v.point(); })
    ;

    py::class_<Triangulation>(sub, "Triangulation")
        .def(py::init<>())
        .def("insert", &Triangulation::push_back)
        .def_property_readonly("all_edges", [](py::object s) { 
            return PyTAllEdgesIterator(
                s.cast<const Triangulation &>().all_edges_begin(), 
                s.cast<const Triangulation &>().all_edges_end(), 
                s.cast<const Triangulation &>(),
                s);
        }, py::return_value_policy::copy)
        .def_property_readonly("edges", [](py::object s) { 
            return PyTFiniteEdgesIterator(
                s.cast<const Triangulation &>().finite_edges_begin(), 
                s.cast<const Triangulation &>().finite_edges_end(), 
                s.cast<const Triangulation &>(),
                s);
        }, py::return_value_policy::copy)
        .def_property_readonly("points", [](const Triangulation& dt) {
            return py::make_iterator(dt.points_begin(), dt.points_end());
        }, py::keep_alive<0, 1>())

    ;

    typedef PySequenceIteratorTriangulationEdges<DT::All_edges_iterator,
                                                 DT> PyDTAllEdgesIterator;

    typedef PySequenceIteratorTriangulationEdges<DT::Finite_edges_iterator,
                                                 DT> PyDTFiniteEdgesIterator;
    
    py::class_<PyDTAllEdgesIterator>(m, "DelaunayTriangulationEdgesIterator")
        .def("__iter__", [](PyDTAllEdgesIterator &it) -> PyDTAllEdgesIterator& { return it; })
        .def("__len__", &PyDTAllEdgesIterator::len)
        .def("__next__", &PyDTAllEdgesIterator::next);

    py::class_<PyDTFiniteEdgesIterator>(m, "DelaunayTriangulationFiniteEdgesIterator")
        .def("__iter__", [](PyDTFiniteEdgesIterator &it) -> PyDTFiniteEdgesIterator& { return it; })
        .def("__len__", &PyDTFiniteEdgesIterator::len)
        .def("__next__", &PyDTFiniteEdgesIterator::next);;

    py::class_<DT::Vertex, DT::Vertex_handle>(sub, "DelaunayTriangulationVertex")
        .def("point", [](const DT::Vertex& v) {return v.point(); })
    ;

    py::class_<DT>(sub, "DelaunayTriangulation")
        .def(py::init<>())
        .def("insert", &DT::push_back)
        .def_property_readonly("all_edges", [](py::object s) { 
            return PyDTAllEdgesIterator(
                s.cast<const DT &>().all_edges_begin(), 
                s.cast<const DT &>().all_edges_end(), 
                s.cast<const DT &>(),
                s);
        }, py::keep_alive<0, 1>())
        .def_property_readonly("edges", [](py::object s) { 
            return PyDTFiniteEdgesIterator(
                s.cast<const DT &>().finite_edges_begin(), 
                s.cast<const DT &>().finite_edges_end(), 
                s.cast<const DT &>(),
                s);
        }, py::keep_alive<0, 1>())
        .def_property_readonly("points", [](const DT& dt) {
            return py::make_iterator(dt.points_begin(), dt.points_end());
        }, py::keep_alive<0, 1>())
    ;

}
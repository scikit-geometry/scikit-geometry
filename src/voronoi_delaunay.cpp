#include "skgeom.hpp"

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
typedef CGAL::Voronoi_diagram_2<DT, AT, AP>                                  VD;

typedef VD::Locate_result             Locate_result;
typedef VD::Vertex_handle             VD_Vertex_handle;
typedef VD::Vertex                    VD_Vertex;
typedef VD::Face_handle               VD_Face_handle;
typedef VD::Face                      VD_Face;
typedef VD::Halfedge_handle           VD_Halfedge_handle;
typedef VD::Halfedge                  VD_Halfedge;
typedef VD::Ccb_halfedge_circulator   VD_Ccb_halfedge_circulator;


void init_voronoi_delaunay(py::module & m) {
    auto sub = m.def_submodule("voronoi");

    py::class_<VD_Face>(sub, "VoronoiFace")
        .def("halfedge", [](const VD_Face& self) { return *(self.halfedge()); })
        .def("is_unbounded", [](const VD_Face& self) {return self.is_unbounded();})
    ;

    py::class_<VD_Halfedge>(sub, "VoronoiHalfedge")
        .def("has_source", &VD_Halfedge::has_source)
        .def("has_target", &VD_Halfedge::has_target)
        .def("source", [](const VD_Halfedge& self) -> py::object {
            if(self.has_source()) {
                return py::cast(*(self.source()));
            }
            else {
                return py::none();
            }
        })
        .def("target", [](const VD_Halfedge& self) -> py::object {
            if(self.has_target()) {
                return py::cast(*(self.target()));
            }
            else {
                return py::none();
            }
        })
        .def("next", [](const VD_Halfedge& self) { return *(self.next()); })
    ;

    py::class_<VD_Vertex>(sub, "VoronoiVertex")
        .def("point", [](const VD_Vertex& v) { return v.point(); })
    ;

    py::class_<VD>(sub, "VoronoiDiagram")
        .def(py::init<>())
        .def("insert", [](VD& self, const Point_2& pt) { return *(self.insert(pt)); })
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
        .def_property_readonly("edges", [](const VD& vd) { 
            return py::make_iterator(vd.edges_begin(), vd.edges_end());
        }, py::keep_alive<0, 1>())
        .def_property_readonly("finite_edges", [](const VD& vd) {
            Segment_2 s;
            Ray_2 r;
            Line_2 l;
            py::list res;
            auto& d = vd.dual();
            for (auto eit = d.finite_edges_begin(); eit != d.finite_edges_end(); ++eit)
            {
                CGAL::Object o = d.dual(eit);
                if (CGAL::assign(s, o))
                {
                    res.append(py::cast(s, py::return_value_policy::copy));
                } 
                else if (CGAL::assign(r, o))
                {
                    res.append(py::cast(r, py::return_value_policy::copy));
                }
                else if (CGAL::assign(l, o))
                {
                    res.append(py::cast(l, py::return_value_policy::copy));
                }
            }
            return res;
        })
    ;

    py::class_<T_Vertex>(sub, "TriangulationVertex")
        .def("point", [](const T_Vertex& v) {return v.point(); })
    ;

    py::class_<Triangulation>(sub, "Triangulation")
        .def(py::init<>())
        .def("insert", &Triangulation::push_back)
        .def_property_readonly("all_edges", [](Triangulation& s) { 
            return py::make_iterator(s.all_edges_begin(), s.all_edges_end());
        }, py::keep_alive<0, 1>())
        .def_property_readonly("edges", [](Triangulation& s) { 
            return py::make_iterator(s.edges_begin(), s.edges_end());
        }, py::keep_alive<0, 1>())
        .def_property_readonly("points", [](const Triangulation& dt) {
            return py::make_iterator(dt.points_begin(), dt.points_end());
        }, py::keep_alive<0, 1>())
    ;

    py::class_<DT>(sub, "DelaunayTriangulation")
        .def(py::init<>())
        .def("insert", &DT::push_back)
        .def_property_readonly("all_edges", [](DT& s) { 
            return py::make_iterator(s.all_edges_begin(), s.all_edges_end());
        }, py::keep_alive<0, 1>())
        .def_property_readonly("edges", [](DT& s) { 
            return py::make_iterator(s.edges_begin(), s.edges_end());
        }, py::keep_alive<0, 1>())
        .def_property_readonly("points", [](const DT& dt) {
            return py::make_iterator(dt.points_begin(), dt.points_end());
        }, py::keep_alive<0, 1>())
    ;
}
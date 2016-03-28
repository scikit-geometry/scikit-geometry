#include "pygal.hpp"
#include <CGAL/Rotational_sweep_visibility_2.h>
#include <CGAL/Arr_segment_traits_2.h>
#include <CGAL/Arrangement_2.h>
#include "funcs.hpp"

#include <CGAL/CORE_algebraic_number_traits.h>
#include <CGAL/Arr_Bezier_curve_traits_2.h>

typedef Segment_Arrangement_2::Ccb_halfedge_circulator          Ccb_halfedge_circulator;
typedef Segment_Arrangement_2::Isolated_vertex_iterator         Isolated_vertex_iterator;

typedef CGAL::CORE_algebraic_number_traits              Nt_traits;
typedef Nt_traits::Rational                             NT;
typedef Nt_traits::Rational                             Rational;
typedef Nt_traits::Algebraic                            Algebraic;
typedef CGAL::Cartesian<Rational>                       Rat_kernel;
typedef CGAL::Cartesian<Algebraic>                      Alg_kernel;
typedef Rat_kernel::Point_2                             Rat_point_2;
typedef Alg_kernel::Point_2                             Alg_point_2;
typedef CGAL::Arr_Bezier_curve_traits_2<Rat_kernel, Kernel, Nt_traits>
        Traits_2;
typedef Traits_2::Curve_2                               Bezier_curve_2;
typedef CGAL::Arrangement_2<Traits_2>                   Bezier_Arrangement_2;
typedef CGAL::Cartesian_converter<Rat_kernel,Kernel>	Rational_to_Kernel;
typedef CGAL::Cartesian_converter<Kernel,Rat_kernel>	Kernel_to_Rational;

typedef PySequenceCirculator<Ccb_halfedge_circulator> PyCcbHalfedgeCirculator;

Halfedge_handle insert_non_intersecting_curve_in_arr(Segment_Arrangement_2 & arr, Segment_2 seg) {
	return CGAL::insert_non_intersecting_curve(arr, seg);
}

void insert_non_intersecting_curves_in_arr(Segment_Arrangement_2 & arr, std::vector<Segment_2> segs) {
	CGAL::insert_non_intersecting_curves(arr, segs.begin(), segs.end());
}

Vertex_handle insert_point_in_arr(Segment_Arrangement_2 & arr, Point_2 & p) {
	Vertex_handle h = CGAL::insert_point(arr, p);
	return h;
}

bool remove_vertex_from_arr(Segment_Arrangement_2 & arr, Vertex_handle v) {
	return CGAL::remove_vertex(arr, v);
}

Face_handle remove_edge_from_arr(Segment_Arrangement_2 & arr, Halfedge_handle h) {
	return CGAL::remove_edge(arr, h);
}

py::object find_in_arrangement(Segment_Arrangement_2 & arr, Point_2 & query_point) {
    CGAL::Arr_naive_point_location<Segment_Arrangement_2> pl(arr);
    CGAL::Arr_point_location_result<Segment_Arrangement_2>::Type obj = pl.locate(query_point);

    const Vertex_const_handle*   v;
    const Halfedge_const_handle* e;
    const Face_const_handle*     f;
    // Casting to non - const types since they are only defined in pygal
    if ( (f = boost::get<Face_const_handle>(&obj)) ) { // located inside a face
        return py::cast(*((Face_handle *)f));
    }
    else if ( (e = boost::get<Halfedge_const_handle>(&obj)) ) // located on an edge
    {
        return py::cast(*((Halfedge_handle *)e));   
    }
    else if ( (v = boost::get<Vertex_const_handle>(&obj)) ){ // located on a vertex
        return py::cast(*((Vertex_handle *)v));
    }
    return py::object();
}

Rat_point_2 to_rat_point(Point_2 & p) {
	return Rat_point_2(CGAL::to_double(p.x()), CGAL::to_double(p.y()));
}

template <typename T>
py::class_<T> cgal_iterator(py::module m, std::string name) {
    auto c = py::class_<T>(m, name)
        .def("__iter__", [](T &it) -> T& { return it; })
        .def("__len__", &T::len)
        .def("__next__", &T::next);

    return c;
}


class PyHoleIteratorClass : public PySequenceIterator<Hole_iterator> {
public:
    using PySequenceIterator::PySequenceIterator;
    py::object next() {
        if (iter == end)
            throw py::stop_iteration();
        auto halfedge_iter = *(iter++);
        return py::cast(new PyCcbHalfedgeCirculator(
            halfedge_iter, 
            py::object()
        ));
    }
};

void init_arrangement(py::module &m) {
	py::module sub = m.def_submodule("arrangement");

    py::class_<Segment_Arrangement_2>(sub, "Arrangement")
    	.def(py::init<>())
        .def_property_readonly("halfedges", [](py::object s) { 
            return PySequenceIterator<Halfedge_iterator>(
                s.cast<Segment_Arrangement_2 &>().halfedges_begin(),
                s.cast<Segment_Arrangement_2 &>().halfedges_end(),
                s
            ); 
        })
        // TODO CHANGE to non const?
        .def_property_readonly("faces", [](py::object s) { 
            return PySequenceIterator<Face_iterator>(
                s.cast<Segment_Arrangement_2 &>().faces_begin(),
                s.cast<Segment_Arrangement_2 &>().faces_end(),
                s
            ); 
        })
        .def_property_readonly("vertices", [](py::object s) { 
            return PySequenceIterator<Vertex_iterator>(
                s.cast<Segment_Arrangement_2 &>().vertices_begin(), 
                s.cast<Segment_Arrangement_2 &>().vertices_end(), 
                s
            ); 
        })
        .def("insert_non_intersecting_curve", &insert_non_intersecting_curve_in_arr)
        .def("insert_non_intersecting_curves", &insert_non_intersecting_curves_in_arr)
        .def("unbounded_face", static_cast<Face_handle (Segment_Arrangement_2::*)()>(&Segment_Arrangement_2::unbounded_face))
        // .def("insert_from_left_vertex", &Segment_Arrangement_2::insert_from_left_vertex)
        // .def("insert_from_right_vertex", &Segment_Arrangement_2::insert_from_right_vertex)
        // .def("insert_in_face_interior", &Segment_Arrangement_2::insert_in_face_interior)
        // .def("insert_at_vertices", &Segment_Arrangement_2::insert_at_vertices)
        // .def("split_edge", &Segment_Arrangement_2::split_edge)
        .def("insert_point", &insert_point_in_arr)
        .def("remove_vertex", &remove_vertex_from_arr)
        .def("remove_edge", &remove_edge_from_arr)
        .def("find", &find_in_arrangement)
    ;

    py::class_<Bezier_curve_2>(sub, "BezierCurve")
        .def("__init__", [](Bezier_curve_2 & instance, Point_2 p, Point_2 q, Point_2 r, Point_2 s) {
        	std::vector<Rat_point_2> points = {to_rat_point(p), 
        		to_rat_point(q), 
        		to_rat_point(r), 
        		to_rat_point(s)};
            new (&instance) Bezier_curve_2(points.begin(), points.end());
        })
        .def("get", [](Bezier_curve_2 & bc, double t) -> Rat_point_2 {
        	Rat_point_2 pr =  bc(Rat_kernel::FT(t));
        	return pr;
        })
    ;

    typedef PySequenceIterator<Halfedge_iterator> PyHalfedgeIterator;
    typedef PySequenceIterator<Face_iterator> PyFaceIterator;
    typedef PySequenceIterator<Vertex_iterator> PyVertexIterator;
    
    py::class_<PyHalfedgeIterator>(sub, "HalfedgeIterator")
        .def("__iter__", [](PyHalfedgeIterator &it) -> PyHalfedgeIterator& { return it; })
        .def("__len__", &PyHalfedgeIterator::len)
        .def("__next__", &PyHalfedgeIterator::next);

    py::class_<PyFaceIterator>(sub, "FaceIterator")
        .def("__iter__", [](PyFaceIterator &it) -> PyFaceIterator& { return it; })
        .def("__len__", &PyFaceIterator::len)
        .def("__next__", &PyFaceIterator::next);

    py::class_<PyVertexIterator>(sub, "VertexIterator")
        .def("__iter__", [](PyVertexIterator &it) -> PyVertexIterator& { return it; })
        .def("__len__", &PyVertexIterator::len)
        .def("__next__", &PyVertexIterator::next);

    py::class_<PyCcbHalfedgeCirculator>(sub, "Ccb_halfedge_circulator")
        .def("__iter__", [](PyCcbHalfedgeCirculator &it) -> PyCcbHalfedgeCirculator& { return it; })
        .def("__next__", &PyCcbHalfedgeCirculator::next);


    py::class_<Halfedge, Halfedge_handle>(sub, "Halfedge")
        .def("prev", static_cast<Halfedge_handle (Halfedge::*)()>(&Halfedge::prev))
        .def("next", static_cast<Halfedge_handle (Halfedge::*)()>(&Halfedge::next))
        .def("twin", static_cast<Halfedge_handle (Halfedge::*)()>(&Halfedge::twin))
        .def("source", static_cast<Vertex_handle (Halfedge::*)()>(&Halfedge::source))
        .def("target", static_cast<Vertex_handle (Halfedge::*)()>(&Halfedge::target))
        .def("curve", [](Halfedge & he) -> Segment_2 {
        	return he.curve();
        })
        .def("face", static_cast<Face_handle (Halfedge::*)()>(&Halfedge::face))
        .def_property_readonly("ccb", [](py::object s) { 
            return PyCcbHalfedgeCirculator(
                s.cast<Halfedge &>().ccb(), 
                s
            );
        })
    ;

    // typedef PySequenceIterator<Hole_iterator> PyHoleIterator;
    py::class_<PyHoleIteratorClass>(sub, "HoleIterator")
        .def("__iter__", [](PyHoleIteratorClass &it) -> PyHoleIteratorClass& { return it; })
        .def("__len__", &PyHoleIteratorClass::len)
        .def("__next__", &PyHoleIteratorClass::next);

    typedef PySequenceIterator<Isolated_vertex_iterator> PyIsolatedVertexIterator;
    py::class_<PyIsolatedVertexIterator>(sub, "IsolatedVertexIterator")
        .def("__iter__", [](PyIsolatedVertexIterator &it) -> PyIsolatedVertexIterator& { return it; })
        .def("__len__", &PyIsolatedVertexIterator::len)
        .def("__next__", &PyIsolatedVertexIterator::next);

    py::class_<Face, Face_handle>(sub, "Face")
    	.def("is_unbounded", [](Face & f) -> bool { return f.is_unbounded(); })
        .def("has_outer_ccb", &Face::has_outer_ccb)
        .def("number_of_holes", &Face::number_of_holes)
        .def("number_of_isolated_vertices", [](Face & f) -> int { return f.number_of_isolated_vertices(); })
        .def_property_readonly("holes", [](py::object s) { 
            return PyHoleIteratorClass(
                s.cast<Face &>().holes_begin(), 
                s.cast<Face &>().holes_end(), 
                s
            );
        })
        .def_property_readonly("isolated_vertices", [](py::object s) { 
            return PyIsolatedVertexIterator(
                s.cast<Face &>().isolated_vertices_begin(), 
                s.cast<Face &>().isolated_vertices_end(), 
                s
            );
        })
    ;

    py::class_<Vertex, Vertex_handle>(sub, "Vertex")
        .def("point", static_cast<Point_2 & (Vertex::*)()>(&Vertex::point))
        // .def("__repr__", &toString<Vertex>)
        // .add_property("ccb", &py_ccb_circulator<Ccb_halfedge_circulator>)
    ;

    py::class_<Rational>(sub, "Rational")
    	.def("__init__", [](Rational & r, int a, int b) {
    		new (&r) Rational(a, b);
    	})
    	.def("__repr__", &toString<Rational>)
    ;

    m.def("to_double", &to_double<Rational>);

    py::class_<Rat_point_2>(sub, "RationalPoint")
    	.def(py::init<double, double>())
    	.def(py::init<Rational, double>())
    	.def(py::init<Rational, Rational>())
    	.def("x", &Rat_point_2::x)
    	.def("y", &Rat_point_2::y)
    	.def("__repr__", &toString<Rat_point_2>)
    ;
}


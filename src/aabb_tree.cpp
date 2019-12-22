#include "skgeom.hpp"

#include <CGAL/Polyhedron_3.h>

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_triangle_primitive.h>
#include <CGAL/AABB_face_graph_triangle_primitive.h>


typedef CGAL::Polyhedron_3<Kernel>                  Polyhedron_3;

typedef CGAL::Polyhedron_3<Kernel> Polyhedron;
typedef CGAL::AABB_face_graph_triangle_primitive<Polyhedron> Primitive;
typedef CGAL::AABB_traits<Kernel, Primitive> AABBTraits;
typedef CGAL::AABB_tree<AABBTraits> Tree;

typedef boost::optional<Tree::Intersection_and_primitive_id<Segment_3>::Type> Segment_intersection;
// typedef boost::optional<Tree::Intersection_and_primitive_id<Point_3>::Type> Point_intersection;
typedef boost::optional<Tree::Intersection_and_primitive_id<Plane_3>::Type> Plane_intersection;

typedef Tree::Primitive_id Primitive_id;

void make_aabb(Tree& tree, Polyhedron_3& p) {
    new (&tree) Tree(faces(p).first, faces(p).second, p);
}

std::list<py::object> all_intersections(const Tree& tree, const Segment_3& query) {
    std::list<Segment_intersection> intersections;
    std::list<py::object> result;
    tree.all_intersections(query, std::back_inserter(intersections));
    for (auto i : intersections) {
        Point_3* p;
        Segment_3* s;
        if ((p = boost::get<Point_3>(&(i->first)))) {
            result.push_back(py::cast(*p));
        } else if ((s = boost::get<Segment_3>(&(i->first)))) {
            result.push_back(py::cast(*s));
        }
    }
    return result;
}

void init_aabb_tree(py::module & m) {
    py::module sub = m.def_submodule("aabb");
    py::class_<Tree>(sub, "AABB")
        .def(py::init<>())
        .def("__init__", make_aabb)
        .def("do_intersect", &Tree::do_intersect<Ray_3>)
        .def("do_intersect", &Tree::do_intersect<Segment_3>)
        .def("do_intersect", &Tree::do_intersect<Line_3>)
        .def("number_of_intersected_primitives", &Tree::number_of_intersected_primitives<Ray_3>)
        .def("number_of_intersected_primitives", &Tree::number_of_intersected_primitives<Segment_3>)
        .def("number_of_intersected_primitives", &Tree::number_of_intersected_primitives<Line_3>)
        .def("all_intersections", &all_intersections)
        // (bool (Polyhedron_3::*)() const)&Polyhedron_3::is_pure_bivalent
        // .def("number_of_intersected_primitives", &AABB::number_of_intersected_primitives)
    ;
}
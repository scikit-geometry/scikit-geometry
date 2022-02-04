#include "skgeom.hpp"

#include <CGAL/create_straight_skeleton_2.h>
#include <CGAL/create_straight_skeleton_from_polygon_with_holes_2.h>
#include <CGAL/create_offset_polygons_2.h>

typedef CGAL::Straight_skeleton_2<Kernel> Skeleton_2;
typedef std::shared_ptr<Skeleton_2>       Skeleton_2_Ref;

typedef Skeleton_2::Vertex_const_iterator VertexConstIterator;
typedef Skeleton_2::Halfedge_const_iterator HalfedgeConstIterator;
typedef Skeleton_2::Face_const_iterator FaceConstIterator;

typedef CGAL::HalfedgeDS_in_place_list_vertex<Skeleton_2::Vertex> Skeleton_2_Vertex;
typedef CGAL::HalfedgeDS_in_place_list_halfedge<Skeleton_2::Halfedge> Skeleton_2_Halfedge;
typedef CGAL::HalfedgeDS_in_place_list_face<Skeleton_2::Face> Skeleton_2_Face;

template<typename T>
std::shared_ptr<T> to_std(boost::shared_ptr<T> ptr) {
    return std::shared_ptr<T>(ptr.get(), [ptr](T* p) mutable {
		assert(ptr.get() == p);
		ptr.reset();
	});
}

template<typename OwnerRef, typename Value>
class SafeValue {
public:
	OwnerRef owner;
	Value value;

	const Value *operator->() const {
		return &value;
	}
};

template<typename OwnerRef, typename Iterator, typename Value>
class SafeIterator {
	OwnerRef _owner;
	Iterator _it;

public:
	SafeIterator(
		const OwnerRef &owner,
		Iterator it) : _owner(owner), _it(it) {
	}

	bool operator==(const SafeIterator<OwnerRef, Iterator, Value> &other) const {
		return _it == other._it;
	}

	bool operator!=(const SafeIterator<OwnerRef, Iterator, Value> &other) const {
		return _it != other._it;
	}

	SafeIterator<OwnerRef, Iterator, Value> &operator++() {
		_it++;
		return *this;
	}

	SafeIterator<OwnerRef, Iterator, Value> operator++(int) {
		auto tmp(*this);
        operator++();
        return tmp; 
	}

	SafeValue<OwnerRef, Value> operator*() const {
		return SafeValue<OwnerRef, Value>{_owner, *_it};
	}
};

typedef SafeIterator<Skeleton_2_Ref, VertexConstIterator, Skeleton_2_Vertex>     Safe_Skeleton_2_Vertex_Iterator;
typedef SafeValue<Skeleton_2_Ref, Skeleton_2_Vertex>                             Safe_Skeleton_2_Vertex;

typedef SafeIterator<Skeleton_2_Ref, HalfedgeConstIterator, Skeleton_2_Halfedge> Safe_Skeleton_2_Halfedge_Iterator;
typedef SafeValue<Skeleton_2_Ref, Skeleton_2_Halfedge>                           Safe_Skeleton_2_Halfedge;

typedef SafeIterator<Skeleton_2_Ref, FaceConstIterator, Skeleton_2_Face> Safe_Skeleton_2_Face_Iterator;
typedef SafeValue<Skeleton_2_Ref, Skeleton_2_Face>                       Safe_Skeleton_2_Face;

void init_skeleton(py::module & m) {
	py::module sub = m.def_submodule("skeleton");

    py::class_<Skeleton_2, Skeleton_2_Ref>(m, "Skeleton")
        .def_property_readonly("vertices", [](const Skeleton_2_Ref &s) {
            return py::make_iterator(
				Safe_Skeleton_2_Vertex_Iterator(s, s->vertices_begin()),
				Safe_Skeleton_2_Vertex_Iterator(s, s->vertices_end()));
        })
        .def_property_readonly("halfedges", [](const Skeleton_2_Ref &s) {
            return py::make_iterator(
				Safe_Skeleton_2_Halfedge_Iterator(s, s->halfedges_begin()),
				Safe_Skeleton_2_Halfedge_Iterator(s, s->halfedges_end()));
        })
        .def_property_readonly("faces", [](const Skeleton_2_Ref &s) {
            return py::make_iterator(
				Safe_Skeleton_2_Face_Iterator(s, s->faces_begin()),
				Safe_Skeleton_2_Face_Iterator(s, s->faces_end()));
        })
		.def("offset_polygons", [](const Skeleton_2_Ref &s, double offset) -> py::list {
			py::list r;
			auto result = CGAL::create_offset_polygons_2<Polygon_2>(offset, *s);
			for (const auto &polygon : result) {
				r.append(py::cast(new Polygon_2(*polygon), pybind11::return_value_policy::automatic));
			}
			return r;
		});

	py::class_<Safe_Skeleton_2_Vertex>(m, "SkeletonVertex")
		.def_property_readonly("id", [](const Safe_Skeleton_2_Vertex &v) {
			return v->id();
		})
		.def_property_readonly("point", [](const Safe_Skeleton_2_Vertex &v) {
			return v->point();
		})
		.def_property_readonly("time", [](const Safe_Skeleton_2_Vertex &v) {
			return v->time();
		});

	py::class_<Safe_Skeleton_2_Halfedge>(m, "SkeletonHalfedge")
		.def_property_readonly("opposite", [](const Safe_Skeleton_2_Halfedge &h) {
			return Safe_Skeleton_2_Halfedge{h.owner, *h->opposite()};
		})
		.def_property_readonly("next", [](const Safe_Skeleton_2_Halfedge &h) {
			return Safe_Skeleton_2_Halfedge{h.owner, *h->next()};
		})
		.def_property_readonly("is_border", [](const Safe_Skeleton_2_Halfedge &h) {
			return h->is_border();
		})
		.def_property_readonly("vertex", [](const Safe_Skeleton_2_Halfedge &h) {
			return Safe_Skeleton_2_Vertex{h.owner, *h->vertex()};
		})
		.def_property_readonly("is_bisector", [](const Safe_Skeleton_2_Halfedge &h) {
			return h->is_bisector();
		});

	py::class_<Safe_Skeleton_2_Face>(m, "SkeletonFace")
		.def_property_readonly("halfedge", [](const Safe_Skeleton_2_Face &h) {
			return Safe_Skeleton_2_Halfedge{h.owner, *h->halfedge()};
		});

	sub.def("create_interior_straight_skeleton", [](
		const Polygon_2 &polygon) -> Skeleton_2_Ref {
		return to_std(CGAL::create_interior_straight_skeleton_2(polygon, Kernel()));
	});

	sub.def("create_interior_straight_skeleton", [](
		const Polygon_with_holes_2 &polygon) -> Skeleton_2_Ref {
		return to_std(CGAL::create_interior_straight_skeleton_2(polygon));
	});

	sub.def("create_exterior_straight_skeleton", [](
		const Polygon_2 &polygon, double max_offset) -> Skeleton_2_Ref {
		return to_std(CGAL::create_exterior_straight_skeleton_2(
			Kernel::FT(max_offset), polygon, Kernel()));
	});
}

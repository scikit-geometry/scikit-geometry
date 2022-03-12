#include "skgeom.hpp"
#include "funcs.hpp"

#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/polygon_mesh_to_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>

typedef CGAL::Surface_mesh<Point_3>                                 Mesh;
typedef Mesh::Vertex_index                                          V;
typedef Mesh::Face_index                                            F;

namespace PMP = CGAL::Polygon_mesh_processing;

template <typename Key, typename Val>
Mesh::Property_map<Key, Val> add_property_map (Mesh& mesh, std::string name, const Val default_val) {
    Mesh::Property_map<Key, Val> pmap;
    bool created;
    std::tie(pmap, created) = mesh.add_property_map<Key, Val>(name, default_val);
    if (!created) {
        throw std::runtime_error("Property map already exists");
    }
    return pmap;
}

template <typename Key, typename Val>
Val get_property_value(const Mesh::Property_map<Key, Val>& pmap, const Key& key) {
    return pmap[key];
}

template <typename Key, typename Val>
py::array_t<Val> get_property_values(const Mesh::Property_map<Key, Val>& pmap, const std::vector<Key>& keys) {
    size_t nk = keys.size();
    py::array_t<Val, py::array::c_style> vals({int(nk)});
    auto r = vals.mutable_unchecked<1>();

    for (size_t i = 0; i < nk; i++) {
        r(i) = pmap[keys[i]];
    }
    return vals;
}

template <typename Key, typename Val>
void set_property_value(Mesh::Property_map<Key, Val>& pmap, const Key& key, const Val val) {
    pmap[key] = val;
}

template <typename Key, typename Val>
void set_property_values(
        const Mesh::Property_map<Key, Val>& pmap, const std::vector<Key>& keys, const std::vector<Val>& vals)
{
    size_t nk = keys.size();
    size_t nv = vals.size();
    if (nk != nv) {
        throw std::runtime_error("Key and value array sizes do not match");
    }
    for (size_t i = 0; i < nk; i++) {
        pmap[keys[i]] = vals[i];
    }
}

void init_mesh(py::module &m) {
    py::class_<V>(m, "Vertex");
    py::class_<F>(m, "Face");

    py::class_<Mesh::Property_map<V, ssize_t> >(m, "VertexIntProperty")
        .def("__getitem__", [](const Mesh::Property_map<V, ssize_t> pmap, const V& vert) {
            return get_property_value(pmap, vert);
        })
        .def("__getitem__", [](const Mesh::Property_map<V, ssize_t> pmap, const std::vector<V>& verts) {
            return get_property_values(pmap, verts);
        })
        .def("__setitem__", [](Mesh::Property_map<V, ssize_t>& pmap, const V& vert, const ssize_t val) {
            set_property_value(pmap, vert, val);
        })
        .def("__setitem__", [](Mesh::Property_map<V, ssize_t>& pmap, const std::vector<V>& verts, const std::vector<ssize_t>& vals) {
            set_property_values(pmap, verts, vals);
        })
    ;

    py::class_<Mesh>(m, "Mesh")
        .def(py::init<>())
        .def(py::init([](
                py::array_t<double> &verts,
                std::vector<std::vector<size_t>>& faces,
                const bool orient
            ) {
            // Convert verts to Point_3
            auto v = verts.unchecked<2>();
            if (v.shape(1) != 3) {
                throw std::runtime_error("vertices need to be 3 dimensional");
            }
            const ssize_t nv = v.shape(0);
            std::vector<Point_3> points;
            points.reserve(nv);
            for (ssize_t i = 0; i < nv; i++) {
                points.emplace_back(Point_3(v(i, 0), v(i, 1), v(i, 2)));
            }

            if (orient) {
                bool success = PMP::orient_polygon_soup(points, faces);
                if (!success) {
                    throw std::runtime_error("Polygon orientation failed");
                }
            }

            Mesh mesh;
            PMP::polygon_soup_to_polygon_mesh(points, faces, mesh);
            return mesh;
        }))
        .def("to_polygon_soup", [](const Mesh& mesh) {
            std::vector<Point_3> verts;
            std::vector<std::vector<size_t>> faces;
            PMP::polygon_mesh_to_polygon_soup(mesh, verts, faces);

            // convert points to arrays
            const size_t nv = mesh.number_of_vertices();
            py::array_t<double, py::array::c_style> verts_out({nv, size_t(3)});
            auto rv = verts_out.mutable_unchecked<2>();
            for (size_t i = 0; i < nv; i++) {
                Point_3 p = verts[i];
                for (auto j = 0; j < 3; j++) {
                    rv(i, j) = CGAL::to_double(p[j]);
                }
            }

            // ditto faces
            const size_t nf = mesh.number_of_faces();
            py::array_t<size_t, py::array::c_style> faces_out({nf, size_t(3)});
            auto rf = faces_out.mutable_unchecked<2>();
            for (size_t i = 0; i < nf; i++) {
                for (size_t j = 0; j < 3; j++) {
                    rf(i, j) = faces[i][j];
                }
            }
            return std::make_tuple(verts_out, faces_out);
        })

        .def_property_readonly("n_vertices", [](const Mesh& mesh) { return mesh.number_of_vertices(); })
        .def_property_readonly("n_faces", [](const Mesh& mesh) { return mesh.number_of_faces(); })

        .def_property_readonly("vertices", [](const Mesh& mesh) {
            std::vector<V> verts;
            verts.reserve(mesh.number_of_vertices());
            for (V v : mesh.vertices()) {
                verts.emplace_back(v);
            }
            return verts;
        })
        .def_property_readonly("faces", [](const Mesh& mesh) {
            std::vector<F> faces;
            faces.reserve(mesh.number_of_faces());
            for (F f : mesh.faces()) {
                faces.emplace_back(f);
            }
            return faces;
        })

        .def("add_vertex_property", &add_property_map<V, ssize_t>)

        .def("corefine", [](Mesh& mesh1, Mesh& mesh2){
            PMP::corefine(mesh1, mesh2);
        })
        .def("difference", [](Mesh& mesh1, Mesh& mesh2) {
            Mesh result;
            bool success = PMP::corefine_and_compute_difference(mesh1, mesh2, result);
            if (!success) {
                throw std::runtime_error("Boolean operation failed.");
            }
            return result;
        })
        .def("union", [](Mesh& mesh1, Mesh& mesh2) {
            Mesh result;
            bool success = PMP::corefine_and_compute_union(mesh1, mesh2, result);
            if (!success) {
                throw std::runtime_error("Boolean operation failed.");
            }
            return result;
        })
        .def("intersection", [](Mesh& mesh1, Mesh& mesh2) {
            Mesh result;
            bool success = CGAL::Polygon_mesh_processing::corefine_and_compute_union(mesh1, mesh2, result);
            if (!success) {
                throw std::runtime_error("Boolean operation failed.");
            }
            return result;
        })
    ;

}

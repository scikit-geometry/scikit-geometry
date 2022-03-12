#include "skgeom.hpp"
#include "funcs.hpp"

#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/polygon_mesh_to_polygon_soup.h>

typedef CGAL::Surface_mesh<Point_3>                                 Mesh;
typedef Mesh::Vertex_index                                          V;
typedef Mesh::Face_index                                            F;
typedef std::map<std::string, py::array_t<double>&>                 NamedProps;


void add_vertex_property(Mesh& mesh, std::string name, const py::array_t<double>& vals) {
    Mesh::Property_map<V, double> prop_map;
    bool created;
    std::tie(prop_map, created) = mesh.add_property_map<V, double>(name, 0.0);
    if (!created) {
        throw std::runtime_error("failed to construct property map for " + name);
    }

    auto vals_r = vals.unchecked<1>();
    size_t i = 0;

    for (V v : mesh.vertices()) {
        prop_map[v] = vals_r(i);
        i++;
    }
}


void add_face_property(Mesh& mesh, std::string name, const py::array_t<double>& vals) {
    Mesh::Property_map<F, double> prop_map;
    bool created;
    std::tie(prop_map, created) = mesh.add_property_map<F, double>(name, 0.0);
    if (!created) {
        throw std::runtime_error("failed to construct property map for " + name);
    }

    auto vals_r = vals.unchecked<1>();
    size_t i = 0;

    for (F f : mesh.faces()) {
        prop_map[f] = vals_r(i);
        i++;
    }
}

py::array_t<double> vertex_property(const Mesh& mesh, const std::string& name) {
    Mesh::Property_map<V, double> pmap;
    bool found;
    std:tie(pmap, found) = mesh.property_map<V, double>(name);
    if (!found) {
        throw std::runtime_error("Unrecognized vertex property " + name);
    }

    ssize_t nv = mesh.number_of_vertices();
    py::array_t<double, py::array::c_style> vdata({nv});
    auto r = vdata.mutable_unchecked<1>();

    size_t i = 0;
    for (V v : mesh.vertices()) {
        r(i) = pmap[v];
        i++;
    }
    return vdata;
}

py::array_t<double> face_property(const Mesh& mesh, const std::string& name) {
    Mesh::Property_map<F, double> pmap;
    bool found;
    std:tie(pmap, found) = mesh.property_map<F, double>(name);
    if (!found) {
        throw std::runtime_error("Unrecognized vertex property " + name);
    }

    ssize_t nf = mesh.number_of_faces();
    py::array_t<double, py::array::c_style> fdata({nf});
    auto r = fdata.mutable_unchecked<1>();

    size_t i = 0;
    for (F f : mesh.faces()) {
        r(i) = pmap[f];
        i++;
    }
    return fdata;
}


void init_mesh(py::module &m) {
    py::class_<V>(m, "Vertex");
    py::class_<F>(m, "Face");

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
                for (size_t j = 0; j < 3; j++) {
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

        .def("add_vertex_property", &add_vertex_property)
        .def("add_face_property", &add_face_property)
        .def("vertex_properties", [](const Mesh& mesh) {
            return mesh.properties<V>();
        })
        .def("face_properties", [](const Mesh& mesh) {
            return mesh.properties<F>();
        })
        .def("vertex_property", &vertex_property)
        .def("face_property", &face_property)
        .def("corefine", [](Mesh& mesh1, Mesh& mesh2){
            CGAL::Polygon_mesh_processing::corefine(mesh1, mesh2);
        })
        .def("difference", [](Mesh& mesh1, Mesh& mesh2) {
            Mesh *result = new Mesh;
            bool ok = CGAL::Polygon_mesh_processing::corefine_and_compute_difference(mesh1, mesh2, *result);
            return result;
        })
        .def("union", [](Mesh& mesh1, Mesh& mesh2) {
            Mesh *result = new Mesh;
            bool ok = CGAL::Polygon_mesh_processing::corefine_and_compute_union(mesh1, mesh2, *result);
            return result;
        })
        .def("intersection", [](Mesh& mesh1, Mesh& mesh2) {
            Mesh *result = new Mesh;
            bool ok = CGAL::Polygon_mesh_processing::corefine_and_compute_union(mesh1, mesh2, *result);
            return result;
        })
    ;

}

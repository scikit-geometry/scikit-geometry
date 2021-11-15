#include "skgeom.hpp"
#include "funcs.hpp"

#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Polygon_mesh_processing/corefinement.h>

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


void build_mesh(Mesh& mesh, const py::array_t<double>& verts, const py::array_t<int>& faces) {
    // Add vertices
    auto v_r = verts.unchecked<2>();

    if (v_r.shape(1) != 3) {
        throw std::runtime_error("vertices need to be 3 dimensional");
    }
    const ssize_t nv = v_r.shape(0);
    std::vector<V> vidxs;

    for (ssize_t i = 0; i < nv; i++) {
        V vi = mesh.add_vertex(Point_3(v_r(i, 0), v_r(i, 1), v_r(i, 2)));
        vidxs.push_back(vi);
    }

    // Add faces
    auto v_f = faces.unchecked<2>();
    if (v_f.shape(1) != 3) {
        throw std::runtime_error("vertices need to be 3 dimensional");
    }

    const ssize_t nf = v_f.shape(0);
    for (ssize_t i = 0; i < nf; i++) {
        mesh.add_face(vidxs[v_f(i, 0)], vidxs[v_f(i, 1)], vidxs[v_f(i, 2)]);
    }
}


py::array_t<double> vertices(const Mesh& mesh) {
    size_t nv = mesh.number_of_vertices();
    py::array_t<double, py::array::c_style> verts({nv, size_t(3)});
    auto r = verts.mutable_unchecked<2>();

    size_t i = 0;
    for (V v : mesh.vertices()) {
        Point_3 p = mesh.point(v);
        r(i, 0) = CGAL::to_double(p[0]);
        r(i, 1) = CGAL::to_double(p[1]);
        r(i, 2) = CGAL::to_double(p[2]);
        i++;
    }
    assert(i == nv);
    return verts;
}


py::array_t<int> faces(const Mesh& mesh) {
    size_t nf = mesh.number_of_faces();
    py::array_t<int, py::array::c_style> faces({nf, size_t(3)});
    auto r = faces.mutable_unchecked<2>();

    size_t i = 0;
    for (Mesh::Face_index f : mesh.faces()) {
        size_t j = 0;
        for(V v : vertices_around_face(mesh.halfedge(f), mesh)){
            r(i, j) = v;
            j++;
        }
        i++;
    }

    return faces;
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


void init_mesh(py::module &m) {
    py::class_<Mesh>(m, "Mesh")
        .def(py::init<>())
        .def(py::init([](const py::array_t<double> &verts, const py::array_t<int> &faces) {
            Mesh *mesh = new Mesh;
            build_mesh(*mesh, verts, faces);
            return mesh;
        }))
        .def("vertices", &vertices)
        .def("faces", &faces)
        .def("add_vertex_property", &add_vertex_property)
        .def("vertex_properties", [](const Mesh& mesh) {
            return mesh.properties<V>();
        })
        .def("vertex_property", &vertex_property)
    ;

    m.def("corefine", [](Mesh& mesh1, Mesh& mesh2){
        return CGAL::Polygon_mesh_processing::corefine(mesh1, mesh2);
    });
}

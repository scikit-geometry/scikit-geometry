#include "pygal.hpp"
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/IO/Nef_polyhedron_iostream_3.h>
#include <CGAL/minkowski_sum_3.h>

typedef CGAL::Polyhedron_3<Kernel>                  Polyhedron_3;

typedef CGAL::Nef_polyhedron_3<Kernel>				Nef_polyhedron_3;

Polyhedron_3 polyhedron_minkowski_sum_3(Polyhedron_3& p, Polyhedron_3& q) {
	// convert Polyhedron to NEF
	Nef_polyhedron_3 a(p);
	Nef_polyhedron_3 b(q);
	Nef_polyhedron_3 summed = CGAL::minkowski_sum_3(a, b);
	cout << summed << endl;
	Polyhedron_3 result;
	summed.convert_to_polyhedron(result);
	return result;
}

void init_3d_minkowski(py::module& m) {
	m.def("minkowski_sum_3", &polyhedron_minkowski_sum_3);
}
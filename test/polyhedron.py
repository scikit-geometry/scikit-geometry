import sys
sys.path.append('.')

import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt
import numpy as np

from skgeom.draw import *
from skgeom import *


fn = 'assets/teapot.obj'
# with open('./test/teapot.obj') as fi:
p = polyhedron_from_string(fn)

aabb = aabb.AABB(p)
print(aabb)

inter = aabb.do_intersect(Ray3(Point3(0, 0, 0), Point3(100, 100, 100)))
print(inter)


def draw(p):
	fig = plt.figure()

	faces = []
	for facet in p.facets:
		face = []
		first = facet.halfedge()
		point = first.vertex().point()
		face.append([float(point.x()), float(point.y()), float(point.z())])
		he = first.next()
		while he is not first:
			point = he.vertex().point()
			face.append([float(point.x()), float(point.y()), float(point.z())])
			he = he.next()
		faces.append(face)

	arr = np.asarray(faces)
	ax = Axes3D(fig)
	ax.add_collection3d(mplot3d.art3d.Poly3DCollection(faces, alpha=0.3, edgecolor='red'))

	all_points = arr.reshape(-1, 9)
	xlim = np.min(all_points[:, 0]), np.max(all_points[:, 0])
	ylim = np.min(all_points[:, 1]), np.max(all_points[:, 1])
	zlim = np.min(all_points[:, 2]), np.max(all_points[:, 2])
	lims = np.asarray([xlim, ylim, zlim])
	lims = np.min(lims[:, 0]), np.max(lims[:, 1])
	ax.set_xlim3d(*lims)
	ax.set_ylim3d(*lims)
	ax.set_zlim3d(*lims)
	plt.show()

seg = Segment3(Point3(1, 1, 1), Point3(4, 4, 4))

print(aabb.all_intersections(seg))

draw(p)

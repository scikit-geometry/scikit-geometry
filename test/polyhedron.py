import sys
sys.path.append('.')

import numpy as np
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits import mplot3d
import matplotlib.pyplot as plt

from draw import *
from pygal import *




fn = './test/teapot.obj'
# with open('./test/teapot.obj') as fi:
p = polyhedron_from_string(fn)

aabb = aabb.AABB(p)
print(aabb)

inter = aabb.do_intersect(Ray_3(Point_3(0, 0, 0), Point_3(100, 100, 100)))
print(inter)


def draw(p):
	fig = plt.figure()
	ax = fig.add_subplot(111, projection='3d')

	faces = []
	for facet in p.facets:
		face = []
		print(facet)
		first = facet.halfedge()
		point = first.vertex().point()
		face.append([to_double(point.x()), to_double(point.y()), to_double(point.z())])
		he = first.next()
		while he is not first:
			point = he.vertex().point()
			face.append([to_double(point.x()), to_double(point.y()), to_double(point.z())])
			he = he.next()
		faces.append(face)
	print(faces)
	ax.add_collection3d(mplot3d.art3d.Poly3DCollection(faces))
	# x.append(to_double(point.x()))
	# y.append(to_double(point.y()))
	# z.append(to_double(point.z()))

	# ax.scatter(x, y, z)
	plt.axis('equal')
	plt.show()

seg = Segment_3(Point_3(1, 1, 1), Point_3(4, 4, 4))

print(aabb.all_intersections(seg))

import IPython
IPython.embed()

print(p)

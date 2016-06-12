import sys
sys.path.append('.')

from draw import *
from pygal import *
import random as rnd

a = Point(0, 0)
b = Point(4, 4)
c = Point(0, 4)
d = Point(4, 0)

# print(a)
s = Segment(a, b)
p = Segment(c, d)
print(type(2))
print(barycenter(a, 2, b, 12))

# points = [pygal.Point(rnd.uniform(-10, 10), rnd.uniform(-10, 10)) for i in range(0, 100)]
# print(points)
# draw_list(points)
# plt.hold(True)
# cvh = pygal.convex_hull.graham_andrew(points)
# draw_list(cvh, as_poly=True)
# plt.axis('equal')
# plt.show()

# print("HULL: ", cvh)
# print(s)
#
# print(pygal.intersection(s, p))
#
poly = pygal.Polygon([a, c, b, d])
print(poly)
print("Poly a simple: ", poly.is_simple())
print(poly.vertices)
for v in poly.vertices:
	print(v)

# h = [pygal.Point(1, 1), pygal.Point(1, 2), pygal.Point(2, 1)]

# for v in poly.vertices:
# 	print(v)
#
# print(list(poly.vertices));
print(poly.edges)
for e in poly.edges:
	print(e)
# print(list(poly.edges));
#
# pwh = pygal.Polygon_with_holes(poly, [pygal.Polygon(h)])
# print(pwh)
#
# poly_a = pygal.Polygon(list(reversed([a,c,b,d])))
# poly_b = pygal.Polygon(list(reversed(h)))
# print("Poly b simple: ", poly_b.is_simple())
#
# print(pygal.boolean_set.join(poly_a, poly_b))
# # print(dir(pygal))
arr = Arrangement()
arr.insert_non_intersecting_curve(s)
# arr.insert_non_intersecting_curve(p)
print('Mkay')
# # print(arr.halfedges)
for he in arr.halfedges:
	print("Superhe", he, he.curve())
# 	#print(he.curve().source().point())
# 	print(he.curve().source())
# # print("HE after iter", he)
# #
# # fh = arr.remove_edge(he)
# # print(fh)
# for he in arr.halfedges:
# 	print("Superhe", he, he.curve())

# visi = pygal.Rotational_sweep_visibility(arr)

# print(pygal.BezierCurve(a,b,c,d))
# t = BezierCurve(a,b,c,d)
# bezl = []
# i = 0
# while i < 1:
# 	print(i)
# 	bezl.append(t.get(i))
# 	i += 0.001
# draw_list(bezl)
# plt.show()
# print(t.get(0.5))
# print(t.get(0.6))
# print(t.get(0.7))
# ponbez = t.get(0.12)
# print(l, h)

# print(pygal.to_double(ponbez.x()), pygal.to_double(ponbez.y()))
# out = visi.compute_visibility(pygal.Point(3, 5), h)
# print(out)

# print(visi)


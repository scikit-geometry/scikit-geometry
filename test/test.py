import sys
sys.path.append('.')

from skgeom import *
from skgeom.draw import *
import skgeom
import random as rnd

a = Point2(0, 0)
b = Point2(4, 4)
c = Point2(0, 4)
d = Point2(4, 0)

# print(a)
s = Segment2(a, b)
p = Segment2(c, d)
print(type(2))
print(barycenter(a, 2, b, 12))

points = [Point2(rnd.uniform(-10, 10), rnd.uniform(-10, 10)) for i in range(0, 100)]

draw(points)
cvh = skgeom.convex_hull.graham_andrew(points)
draw(Polygon(cvh))
plt.axis('equal')
plt.show()

print(skgeom.intersection(s, p))
poly = skgeom.Polygon([a, c, b, d])

print(poly)
print("Poly a simple: ", poly.is_simple())
print(poly.vertices)

for v in poly.vertices:
	print(v)

h = [Point2(1, 1), Point2(1, 2), Point2(2, 1)]

for v in poly.vertices:
	print(v)

print(list(poly.vertices));

for e in poly.edges:
	print(e)

print(list(poly.edges));

pwh = skgeom.PolygonWithHoles(poly, [skgeom.Polygon(h)])
print(pwh)

poly_a = skgeom.Polygon(list(reversed([a,c,b,d])))
poly_b = skgeom.Polygon(list(reversed(h)))
print("Poly b simple: ", poly_b.is_simple())

print(skgeom.boolean_set.join(poly_a, poly_b))

arr = skgeom.arrangement.Arrangement()
arr.insert_non_intersecting_curve(s)
arr.insert_non_intersecting_curve(p)

print(arr.halfedges)

for he in arr.halfedges:
	print(he, he.curve())
	print(he.curve().source())
	
	# fh = arr.remove_edge(he)

visi = skgeom.RotationalSweepVisibility(arr)

# print(skgeom.BezierCurve(a,b,c,d))
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

# print(skgeom.to_double(ponbez.x()), skgeom.to_double(ponbez.y()))
# out = visi.compute_visibility(skgeom.Point(3, 5), h)
# print(out)

# print(visi)


import unittest
from skgeom import Point2, Bbox2, Vector2, Line2, intersection, do_intersect

class BasicTest(unittest.TestCase):
	def test_point(self):
		p1, p2, p3 = Point2(5, 5), Point2(2, 3), Point2(5, 5)

		self.assertTrue(p1 is p1)
		self.assertTrue(p1 is not p3)
		self.assertEqual(p1, p3)
		self.assertNotEqual(p1, p2)

		self.assertEqual(float(p1.x()), 5.)
		# self.assertEqual(p1.x(), 5)

		vec = p1 - p2
		self.assertTrue(type(vec) is Vector2)
		self.assertEqual(float(vec.x()), 3)
		self.assertEqual(float(vec.y()), 2)

		p4 = Point2(8, 7)
		self.assertEqual(p1 + vec, p4)

		self.assertTrue(p1 > p2)
		self.assertTrue(p2 < p1)
		self.assertTrue(p3 <= p1)
		self.assertTrue(p2 <= p1)
		self.assertTrue(p3 >= p1)

		self.assertEqual(p1.bbox(), Bbox2(p1.x(), p1.y(), p1.x(), p1.y()))
		self.assertEqual(p1.dimension(), 2)

	def test_line(self):
		p1, p2, p3 = Point2(0, 0), Point2(1, 0), Point2(0, 1)
		l1, l2 = Line2(p1, p2), Line2(p1, p3)

		self.assertEqual(l1, l1)
		self.assertTrue(l1 is l1)
		self.assertNotEqual(l1, l2)

		ip = intersection(l1, l2)
		self.assertEqual(ip, Point2(0, 0))

		self.assertTrue(do_intersect(l1, l2))

		il = intersection(l1, l1)
		self.assertEqual(l1, il)
		
if __name__ == '__main__':
    unittest.main()

# import all contents from C++ extension module
import unittest
from ._pygal import *

class BasicTest(unittest.TestCase):
	def test_point(self):
		p1 = Point2(5, 5)
		p2 = Point2(2, 7)
		p3 = Point2(5, 5)

		self.assertFalse(p1 is p3)
		self.assertTrue(p1 is p1)
		self.assertEqual(p1, p1)
		self.assertEqual(p1, p3)
		self.assertNotEqual(p1, p2)

		self.assertEqual(p1.x(), 5)
		vec = p1 - p2

		self.assertEqual(vec.x(), 3)


	def test_line(self):
		line1 = Line2(Point2(0, 0), Point2(1, 0))
		line2 = Line2(Point2(0, 0), Point2(0, 1))

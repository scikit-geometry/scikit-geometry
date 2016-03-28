import sys
sys.path.append('.')

import unittest

from draw import *
from pygal import *
from pygal.pygal.arrangement import *
import IPython


class PCATests(unittest.TestCase):
    def setUp(self):

        self.segments = [
            Segment(Point(0, 0), Point(0, 4)),
            Segment(Point(0, 4), Point(4, 4)),
            Segment(Point(4, 4), Point(4, 0)),
            Segment(Point(4, 0), Point(0, 0)),
        ]
        self.hole_s = [
            Segment(Point(2, 2), Point(2, 3)),
            Segment(Point(2, 3), Point(3, 3)),
            Segment(Point(3, 3), Point(3, 2)),
            Segment(Point(3, 2), Point(2, 2)),
        ]

    def test_arrangement(self):
        self.arr = Arrangement()
        print(self.arr.insert_non_intersecting_curves(self.segments))
        p = self.arr.find(Point(0, 0))
        self.assertEqual(type(p), Vertex)
        self.assertEqual(p.point(), Point(0, 0))
        h = self.arr.find(Point(0, 2))
        self.assertEqual(type(h), Halfedge)
        self.assertEqual(h.curve(), Segment(Point(0, 0), Point(0, 4)))
        for v in self.arr.vertices:
            print(v)

        print(self.arr.insert_non_intersecting_curves(list(reversed(self.hole_s))))
        f = self.arr.find(Point(1.5, 1.5))
        self.assertEqual(type(f), Face)
        print(f.holes.__next__())
        for hole in f.holes:
            for v in hole:
                print(v)
        self.assertFalse(f.is_unbounded())
        self.assertEqual(f.number_of_holes(), 0)
        self.assertEqual(f.number_of_isolated_vertices(), 0)
        # self.assertEqual(h.curve(), Segment(Point(0, 0), Point(0, 4)))
        f = self.arr.find(Point(-10, -10))
        self.assertEqual(type(f), Face)
        self.assertTrue(f.is_unbounded())

        self.arr.remove_vertex(self.arr.find(Point(0, 0)))
        self.arr.remove_vertex(self.arr.find(Point(0, 0)))



if __name__ == '__main__':
    unittest.main()

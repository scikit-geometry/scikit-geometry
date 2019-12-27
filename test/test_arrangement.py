import unittest

from skgeom import *
from skgeom.utils import *

import IPython

class ArrangementTests(unittest.TestCase):
    def setUp(self):

        self.segments = [
            Segment2(Point2(0, 0), Point2(0, 4)),
            Segment2(Point2(0, 4), Point2(4, 4)),
            Segment2(Point2(4, 4), Point2(4, 0)),
            Segment2(Point2(4, 0), Point2(0, 0)),
        ]
        self.hole_s = [
            Segment2(Point2(2, 2), Point2(2, 3)),
            Segment2(Point2(2, 3), Point2(3, 3)),
            Segment2(Point2(3, 3), Point2(3, 2)),
            Segment2(Point2(3, 2), Point2(2, 2)),
        ]

    def test_arrangement(self):
        self.arr = arrangement.Arrangement()
        print(self.arr.insert_non_intersecting_curves(self.segments))
        p = self.arr.find(Point2(0, 0))
        self.assertEqual(type(p), arrangement.Vertex)
        self.assertEqual(p.point(), Point2(0, 0))
        h = self.arr.find(Point2(0, 2))
        self.assertEqual(type(h), arrangement.Halfedge)
        print(h.curve())
        self.assertEqual(h.curve(), Segment2(Point2(0, 0), Point2(0, 4)))

        print(self.arr.vertices)
        for v in self.arr.vertices:
            print(v)
            print(v.point())

        print(self.arr.insert_non_intersecting_curves(list(reversed(self.hole_s))))
        f = self.arr.find(Point2(1.5, 1.5))
        self.assertEqual(type(f), arrangement.Face)

        print(f.holes.__next__())
        for hole in f.holes:
            first = next(hole)
            print("Hole: ", first.curve())
            v = next(hole)
            while v != first:
                print(v.curve())
                v = next(hole)

        self.assertFalse(f.is_unbounded())
        self.assertEqual(f.number_of_holes(), 1)
        self.assertEqual(f.number_of_isolated_vertices(), 0)
        self.assertEqual(h.curve(), Segment2(Point2(0, 0), Point2(0, 4)))

        f = self.arr.find(Point2(-10, -10))
        self.assertEqual(type(f), arrangement.Face)
        self.assertTrue(f.is_unbounded())

        t = self.arr.find(Point2(0, 0))
        print(t.point())

        # does not remvoe a point which is a corner
        self.assertFalse(self.arr.remove_vertex(self.arr.find(Point2(0, 0))))

        # does remove lonely point
        self.arr.insert_point(Point2(12, 12))
        self.assertTrue(self.arr.remove_vertex(self.arr.find(Point2(12, 12))))

    def test_visibility(self):
        outer = [Segment2(Point2(-100, -100), Point2(-100, 100)),
                 Segment2(Point2(-100, 100), Point2(100, 100)),
                 Segment2(Point2(100, 100), Point2(100, -100)),
                 Segment2(Point2(100, -100), Point2(-100, -100))]

        segments = [
            Segment2(Point2(0, 0), Point2(0, 4)),
            Segment2(Point2(2, 4), Point2(10, 4)),
            Segment2(Point2(3, 4), Point2(-8, 0)),
            Segment2(Point2(1, 0), Point2(0, 5)),
        ]
        arr = arrangement.Arrangement()
        for s in outer:
            arr.insert(s)
        for s in segments:
            arr.insert(s)
        
        # segs = segments_from_points((0, 4), (0,0), (3,2), (4,0), (4, 4), (1, 2))
        # arr.insert_non_intersecting_curves(segs)
        # q = Point2(0.5, 2)
        # face = arr.find(q)

        arr.address()
        vs = RotationalSweepVisibility(arr)
        # vx = vs.compute_visibility(q, face)

        q = Point2(0.1, 4.1)
        face = arr.find(q)

        # we need to find the halfedge which has query_point == target
        for he in arr.halfedges:
            print(he.curve())
        for he in arr.vertices:
            print(he.point())

        he = arr.halfedges
        edge = next(he)

        vx = vs.compute_visibility(edge.target().point(), edge)

        from matplotlib import pyplot as plt
        for he in arr.halfedges:
            plt.plot([he.curve().source().x(), he.curve().target().x()],
                     [he.curve().source().y(), he.curve().target().y()])

        for he in vx.halfedges:
            plt.plot([he.curve().source().x(), he.curve().target().x()],
                     [he.curve().source().y(), he.curve().target().y()], 'k:')
        plt.scatter(q.x(), q.y())
        plt.show()


if __name__ == '__main__':
    unittest.main()

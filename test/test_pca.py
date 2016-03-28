import sys
sys.path.append('.')

import unittest

from draw import *
from pygal import *
from pygal import principal_component_analysis as pca
import IPython


class PCATests(unittest.TestCase):
    def setUp(self):
        self.points = [
            Point(1, 5),
            Point(3, 2),
            Point(4, 5),
            Point(4, 6),
            Point(1.1223, 4.2),
        ]

    def test_centroid(self):
        self.assertTrue(squared_distance(
            pca.centroid(self.points), 
            Point(2.62446, 4.44)) < 0.0001
        )

    def test_leastSquares(self):
        self.assertTrue(
            pca.linear_least_squares_fitting(self.points),
            Line(-1.0904, 1, -1.5783))

    def test_BoundingBox(self):
        self.assertEqual(pca.bounding_box(self.points), 
            Iso_Rectangle(Point(1, 2), Point(4, 6)))

    def test_Barycenter(self):
        weighted_points = list(zip(self.points, [1.3, 4, 1, 2, 4]))
        self.assertTrue(squared_distance(
            pca.barycenter(weighted_points), 
            Point(2.42189, 3.92683)) < 0.0001)


if __name__ == '__main__':
    unittest.main()

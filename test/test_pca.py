import sys
sys.path.append('.')

import unittest

from skgeom.draw import *
from skgeom import *
from skgeom import principal_component_analysis as pca

class PCATests(unittest.TestCase):
    def setUp(self):
        self.points = [
            Point2(1, 5),
            Point2(3, 2),
            Point2(4, 5),
            Point2(4, 6),
            Point2(1.1223, 4.2),
        ]

    def test_centroid(self):
        self.assertTrue(squared_distance(
            centroid(self.points), 
            Point2(2.62446, 4.44)) < 0.0001
        )

    def test_leastSquares(self):
        self.assertTrue(
            pca.linear_least_squares_fitting(self.points),
            Line2(-1.0904, 1, -1.5783))

    def test_Barycenter(self):
        weighted_points = list(zip(self.points, [1.3, 4, 1, 2, 4]))
        self.assertTrue(squared_distance(
            barycenter(weighted_points), 
            Point2(2.42189, 3.92683)) < 0.0001)


if __name__ == '__main__':
    unittest.main()

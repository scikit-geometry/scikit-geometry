import numpy as np
import pytest

import skgeom as sg
from skgeom import principal_component_analysis as pca

class TestPCA:
    @pytest.fixture
    def points(self):
        return [
            sg.Point2(1, 5),
            sg.Point2(3, 2),
            sg.Point2(4, 5),
            sg.Point2(4, 6),
            sg.Point2(1.1223, 4.2),
        ]

    def test_centroid(self, points):
        assert sg.squared_distance(sg.centroid(points), sg.Point2(2.62446, 4.44)) < 1e-4

    def test_least_squares(self, points):
        pca_line = pca.linear_least_squares_fitting(points)
        fit_line = sg.Line2(-0.736996, 0.675897, -1.06677)
        assert np.isclose(float(pca_line.a()), float(fit_line.a()))
        assert np.isclose(float(pca_line.b()), float(fit_line.b()))
        assert np.isclose(float(pca_line.c()), float(fit_line.c()))

    def test_barycenter(self, points):
        weighted_points = list(zip(points, [1.3, 4, 1, 2, 4]))
        assert sg.squared_distance(sg.barycenter(weighted_points), sg.Point2(2.42189, 3.92683)) < 1e-4

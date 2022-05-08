import numpy as np
import pytest

import skgeom as sg

class TestCH:
    @pytest.fixture
    def inner_arr_points(self):
        return np.random.uniform(-10, 10, size=(100, 2))
    
    @pytest.fixture
    def inner_points(self, inner_arr_points):
        return [sg.Point2(*p) for p in inner_arr_points]
    
    @pytest.fixture
    def outer_arr_points(self):
        return np.array([
            [-10.1, -10.1],
            [-10.1,  10.1],
            [ 10.1,  10.1],
            [ 10.1, -10.1]
        ])
    
    @pytest.fixture
    def outer_points(self, outer_arr_points):
        return [sg.Point2(*p) for p in outer_arr_points]

    def test_graham_andrew(self, inner_points, outer_points, inner_arr_points, outer_arr_points):
        # Test using list of points input
        cvh = sg.convex_hull.graham_andrew(inner_points + outer_points)
        for p in cvh:
            assert p in outer_points

        # Test using numpy input
        cvh_np = sg.convex_hull.graham_andrew(np.concatenate((inner_arr_points, outer_arr_points)))
        assert cvh_np == cvh
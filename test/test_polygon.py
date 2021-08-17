import numpy as np
import pytest
import skgeom as sg

# TODO: push_back, append, coords, orientation, reverse_orientation
# TODO: random_polygon, bbox
# TODO: add separate file for polygon with holes
class TestPolygon:
    
    @pytest.fixture
    def points(self):
        return [sg.Point2(0, 0), sg.Point2(0, 4), sg.Point2(4, 4), sg.Point2(4, 0)]
    
    @pytest.fixture
    def polygon(self, points):
        return sg.Polygon(points)

    def test_size_area_centroid(self, polygon, points):
        assert len(polygon) == len(points)
        assert abs(float(polygon.area())) == 16
        assert sg.centroid(polygon) == sg.Point2(2, 2)
    
    def test_simple_convex(self, points, polygon):
        assert polygon.is_simple()
        assert polygon.is_convex()
        
        comp_poly = sg.Polygon([points[0], points[2], points[1], points[3]])
        assert not comp_poly.is_simple()
        assert not comp_poly.is_convex()

    def test_vertices_edges(self, points, polygon):
        for v in polygon.vertices:
            assert v in points
        for i, e in enumerate(polygon.edges):
            assert e == sg.Segment2(points[i], points[(i + 1) % len(points)])

    def test_on_side(self, polygon):
        test_points = np.array([[-1, -1], [1, 1], [0,2]])
        
        # Test Point2 inputs
        outside_pt = sg.Point2(*test_points[0])
        inside_pt = sg.Point2(*test_points[1])
        boundary_pt = sg.Point2(*test_points[2])
        assert polygon.on_side(outside_pt) == -1
        assert polygon.on_side(inside_pt) == 1
        assert polygon.on_side(boundary_pt) == 0

        # Test float inputs
        assert polygon.on_side(*test_points[0]) == -1
        assert polygon.on_side(*test_points[1]) == 1
        assert polygon.on_side(*test_points[2]) == 0
        
        # Test vectorized input
        assert np.all(polygon.on_side(*test_points.T) == np.array([-1, 1, 0]))
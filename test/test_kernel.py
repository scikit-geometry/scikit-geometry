import pytest
import skgeom as sg

# TODO: missing Vector2
# TODO: missing Ray2
# TODO: missing Direction2
# TODO: missing Bbox2
# TODO: missing IsoRectangle2
# TODO: missing Circle2
# TODO: missing Transformation2
# TODO: missing Transformation3
# TODO: missing Point3
# TODO: missing Line3
# TODO: missing Plane3
# TODO: missing Segment3
# TODO: missing Vector3
# TODO: missing Ray3
# TODO: missing Triangle3
# TODO: squared_distance, intersection, do_intersect for all of these
class TestKernel:

    @pytest.fixture
    def line_points(self):
        return sg.Point2(0, 0), sg.Point2(1, 0), sg.Point2(0, 1)
    
    def test_point2(self):
        p1, p2, p3 = sg.Point2(5, 5), sg.Point2(2, 3), sg.Point2(5, 5)

        assert p1 is p1
        assert p1 is not p3
        assert p1 == p3
        assert p1 != p2
        assert p1.x() == 5

        vec = p1 - p2
        assert type(vec) is sg.Vector2
        assert vec.x() == 3
        assert vec.y() == 2

        p4 = sg.Point2(8, 7)
        assert p1 + vec == p4

        assert p1 > p2
        assert p2 < p1
        assert p3 <= p1
        assert p2 <= p1
        assert p3 >= p1

        assert p1.bbox() == sg.Bbox2(p1.x(), p1.y(), p1.x(), p1.y())
        assert p1.dimension() == 2

    def test_line2(self, line_points):
        # Create lines from points
        p1, p2, p3 = line_points
        l1, l2 = sg.Line2(p1, p2), sg.Line2(p1, p3)

        # Test line equality
        assert l1 == l1
        assert l1 is l1
        assert l1 != l2

        # Test line intersection
        assert sg.do_intersect(l1, l2)
        ip = sg.intersection(l1, l2)
        assert ip == p1

        # Test self-intersection
        il = sg.intersection(l1, l1)
        assert l1 == il

    # TODO: flesh out Segment2 tests
    def test_segment2(self, line_points):
        p1, p2, p3 = line_points
        s = sg.Segment2(p1, p2)
        p = sg.Segment2(p1, p3)
        assert sg.intersection(s, p) == p1

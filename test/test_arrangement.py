import pytest

import skgeom as sg

# TODO: missing some insert tests, unbounded_face, address for Arrangement class
# TODO: missing incident_halfedges for Vertex
# TODO: missing twin, face for Halfedge
# TODO: missing has_outer_ccb, outer_ccb, isolated_vertices for Face
# TODO: Add error tests as well
class TestArrangement:
    @pytest.fixture
    def outer_points(self):
        return [
            sg.Point2(0, 0),
            sg.Point2(0, 4),
            sg.Point2(4, 4),
            sg.Point2(4, 0),
        ]

    @pytest.fixture
    def hole_points(self):
        return [
            sg.Point2(2, 2),
            sg.Point2(2, 3),
            sg.Point2(3, 3),
            sg.Point2(3, 2),
        ]

    @pytest.fixture
    def outer_segments(self, outer_points):
        return [
            sg.Segment2(outer_points[0], outer_points[1]),
            sg.Segment2(outer_points[1], outer_points[2]),
            sg.Segment2(outer_points[2], outer_points[3]),
            sg.Segment2(outer_points[3], outer_points[0]),
        ]

    @pytest.fixture
    def hole_segments(self, hole_points):
        return [
            sg.Segment2(hole_points[0], hole_points[1]),
            sg.Segment2(hole_points[1], hole_points[2]),
            sg.Segment2(hole_points[2], hole_points[3]),
            sg.Segment2(hole_points[3], hole_points[0]),
        ]

    @pytest.fixture
    def arr(self, outer_segments):
        arr = sg.arrangement.Arrangement()
        arr.insert_non_intersecting_curves(outer_segments)
        return arr

    # Test initialization by checking vertices, halfedges, faces
    def test_init(self, arr, outer_points, outer_segments):
        assert len(list(arr.vertices)) == len(outer_points)
        for v in arr.vertices:
            assert v.point() in outer_points

        for h in arr.halfedges:
            assert h.curve() in outer_segments

        assert len(list(arr.faces)) == 2

    #  Test find methods
    def test_find(self, arr, outer_points):
        p = arr.find(outer_points[0])
        assert type(p) is sg.arrangement.Vertex
        assert p.point() == outer_points[0]
        h = arr.find(sg.Point2(0, 2))
        assert type(h) is sg.arrangement.Halfedge

        # Test point in unbounded face
        f = arr.find(sg.Point2(-10, -10))
        assert type(f) is sg.arrangement.Face
        assert f.is_unbounded

    # Test halfedge methods
    def test_halfedge(self, arr, outer_segments, outer_points):
        h = arr.find(sg.Point2(0, 2))
        assert h.curve() == outer_segments[0]
        assert h.prev().curve() == outer_segments[1]
        assert h.next().curve() == outer_segments[3]
        assert h.source().point() == outer_points[1]
        assert h.target().point() == outer_points[0]

    # Test face methods
    def test_faces(self, arr):
        num_bounded = 0
        num_unbounded = 0
        for f in arr.faces:
            assert f.number_of_isolated_vertices == 0
            num_bounded += not f.is_unbounded
            num_unbounded += f.is_unbounded
        assert num_unbounded == 1
        assert num_bounded == 1

    # Test inserting square hole into outer square
    def test_holes(self, arr, hole_segments):
        arr.insert_non_intersecting_curves(list(reversed(hole_segments)))
        f = arr.find(sg.Point2(1.5, 1.5))
        assert type(f) == sg.arrangement.Face
        assert not f.is_unbounded
        assert f.number_of_holes == 1
        assert f.number_of_isolated_vertices == 0

        # Iterate over holes and test hole segments circulator
        for hole in f.holes:
            first = next(hole)
            assert first.curve() in hole_segments
            v = next(hole)
            while v != first:
                assert v.curve() in hole_segments
                v = next(hole)

    def test_remove(self, arr, outer_points):
        # should not remove a point which is a corner
        assert not arr.remove_vertex(arr.find(outer_points[0]))

        # does remove lonely point when inserted
        lone_pt = sg.Point2(12, 12)
        arr.insert_point(lone_pt)
        assert arr.remove_vertex(arr.find(lone_pt))

        # Remove halfedge
        h = arr.find(sg.Point2(0, 2))
        f = arr.remove_edge(h)
        assert type(f) is sg.arrangement.Face
        assert f.is_unbounded

    def test_errors(self, arr, outer_segments):
        # Can't insert multiple of same segments
        with pytest.raises(RuntimeError):
            arr.insert_non_intersecting_curves(outer_segments)

        # Can't remove point that is not in arrangement
        with pytest.raises(TypeError):
            arr.remove_vertex(arr.find(sg.Point2(12, 12)))

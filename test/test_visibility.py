import pytest

import skgeom as sg
import skgeom.utils as utils

# TODO: missing compute_visibility for point/face
# TODO: missing TriangularExpansionVisibility tests
class TestVisibility:

    @pytest.fixture
    def bounds(self):
        return [(-100, -100), (-100, 100), (100, 100), (100, -100)]

    @pytest.fixture
    def segments(self):
        return [
            sg.Segment2(sg.Point2(0, 0), sg.Point2(0, 4)),
            sg.Segment2(sg.Point2(2, 4), sg.Point2(10, 4)),
            sg.Segment2(sg.Point2(3, 4), sg.Point2(-8, 0)),
            sg.Segment2(sg.Point2(1, 0), sg.Point2(0, 5)),
        ]

    def test_rsv(self, bounds, segments):
        arr = sg.arrangement.Arrangement()
        arr.insert(utils.segments_from_points(*bounds))
        arr.insert(segments)

        # Check visibility of a few points from the corner
        vs = sg.RotationalSweepVisibility(arr)
        edge = next(arr.halfedges)
        vx = vs.compute_visibility(edge.target().point(), edge)
        assert vx.find(sg.Point2(*bounds[2])).is_unbounded()
        assert not vx.find(sg.Point2(-99, -99)).is_unbounded()

        # Uncomment for plot
        # from matplotlib import pyplot as plt
        # for he in arr.halfedges:
        #     plt.plot(
        #         [he.curve().source().x(), he.curve().target().x()],
        #         [he.curve().source().y(), he.curve().target().y()],
        #     )

        # for he in vx.halfedges:
        #     plt.plot(
        #         [he.curve().source().x(), he.curve().target().x()],
        #         [he.curve().source().y(), he.curve().target().y()],
        #         "k:",
        #     )
        # plt.scatter(edge.target().point().x(), edge.target().point().y())
        # plt.savefig("test_visibility.png")
        # plt.show()

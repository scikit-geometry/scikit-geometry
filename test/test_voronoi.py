import numpy as np
import skgeom as sg
from skgeom import voronoi

# TODO: missing Delaunay/Triangulation tests
# TODO: missing VD_Face, VD_Vertex, VD_Halfedge tests
class TestVoronoi:

    def test_voronoi(self):
        npoints = np.random.rand(100, 2) * 20 - 10
        points = [sg.Point2(*r) for r in npoints]
        vdiag = voronoi.VoronoiDiagram()
        for p in points:
            vdiag.insert(p)

        assert vdiag.is_valid
        assert list(vdiag.sites) == points
        assert vdiag.number_of_faces() == len(points)
        for _ in vdiag.finite_edges:
            pass

        # can uncomment for vis
        # from matplotlib import pyplot as plt
        # plt.scatter(npoints[:, 0], npoints[:, 1])
        # for he in vdiag.edges:
        #     source, target = he.source(), he.target()
        #     if source and target:
        #         plt.plot(
        #             [source.point().x(), target.point().x()],
        #             [source.point().y(), target.point().y()],
        #         )
        # plt.axis("equal")
        # plt.gca().set_adjustable("box")
        # plt.gca().set_xlim([-10, 10])
        # plt.gca().set_ylim([-10, 10])
        # # plt.savefig("voronoi.png")
        # plt.show()

        vdiag.clear()
        assert len(list(vdiag.vertices)) == 0
        assert len(list(vdiag.edges)) == 0
        assert len(list(vdiag.sites)) == 0
        assert len(vdiag.finite_edges) == 0
        assert vdiag.number_of_vertices() == 0
        assert vdiag.number_of_faces() == 0
        assert vdiag.number_of_halfedges() == 0

import numpy as np
import skgeom
import unittest
from skgeom import voronoi
from matplotlib import pyplot as plt

class VoronoiTest(unittest.TestCase):

    def test_voronoi(self):
        npoints = np.random.rand(100, 2) * 20 - 10
        points = []
        for r in npoints:
            print(r)
            points.append(skgeom.Point2(*r))

        vdiag = voronoi.VoronoiDiagram()
        
        for p in points:
            vdiag.insert(p)

        xx = vdiag.finite_edges
        for el in xx:
            print(el)
        for he in vdiag.edges:
            source, target = he.source(), he.target()
            if source and target:
                plt.plot([source.point().x(), target.point().x()], [source.point().y(), target.point().y()])
        plt.scatter(npoints[:, 0], npoints[:, 1])

        plt.axis('equal')
        plt.gca().set_adjustable("box")
        plt.gca().set_xlim([-10, 10])
        plt.gca().set_ylim([-10, 10])
        plt.show()

if __name__ == '__main__':
    unittest.main()
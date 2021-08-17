import os
import pytest

import skgeom as sg

# TODO: This needs a lot of work - for now just reformatted the current
# file for pytest, but basically just tests init
class TestPolyhedron:
    
    @pytest.fixture
    def polyhedron(self):
        fn = os.path.join(os.path.dirname(os.path.abspath(__file__)), "assets", "teapot.obj")
        return sg.polyhedron_from_string(fn)

    def test_aabb_ray_intersect(self, polyhedron):
        aabb = sg.aabb.AABB(polyhedron)
        inter = aabb.do_intersect(sg.Ray3(sg.Point3(0, 0, 0), sg.Point3(100, 100, 100)))
        seg = sg.Segment3(sg.Point3(1, 1, 1), sg.Point3(4, 4, 4))
        print(aabb.all_intersections(seg))
        sg.draw.draw(polyhedron)
    
    def test_draw(self, polyhedron):

        faces = []
        for facet in polyhedron.facets:
            face = []
            first = facet.halfedge()
            point = first.vertex().point()
            face.append([float(point.x()), float(point.y()), float(point.z())])
            he = first.next()
            while he is not first:
                point = he.vertex().point()
                face.append([float(point.x()), float(point.y()), float(point.z())])
                he = he.next()
            faces.append(face)

        # uncomment for vis
        # import numpy as np
        # from mpl_toolkits.mplot3d import Axes3D
        # from mpl_toolkits import mplot3d
        # import matplotlib.pyplot as plt
        # fig = plt.figure()
        # arr = np.asarray(faces)
        # ax = Axes3D(fig, auto_add_to_figure=False)
        # fig.add_axes(ax)
        # ax.add_collection3d(mplot3d.art3d.Poly3DCollection(faces, alpha=0.3, edgecolor="red"))

        # all_points = arr.reshape(-1, 9)
        # xlim = np.min(all_points[:, 0]), np.max(all_points[:, 0])
        # ylim = np.min(all_points[:, 1]), np.max(all_points[:, 1])
        # zlim = np.min(all_points[:, 2]), np.max(all_points[:, 2])
        # lims = np.asarray([xlim, ylim, zlim])
        # lims = np.min(lims[:, 0]), np.max(lims[:, 1])
        # ax.set_xlim3d(*lims)
        # ax.set_ylim3d(*lims)
        # ax.set_zlim3d(*lims)
        # plt.show()

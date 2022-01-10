import pytest

from skgeom import Mesh
from numpy import array, cos, sin,  pi


def tetrahedron(scale=1.0, rot_z=0.0):
    verts = scale * array([[1, 1, 1], [-1, 1, -1], [1, -1, -1], [-1, -1, 1]], dtype='float')

    if rot_z:
        rot = array([[cos(rot_z), -sin(-rot_z), 0], [sin(rot_z), cos(-rot_z), 0], [0, 0, 1]])
        verts = verts @ rot

    faces = array([[2, 1, 0], [2, 3, 1], [3, 2, 0], [1, 3, 0]], dtype='int')

    return verts, faces


def test_simple_mesh():
    verts, faces = tetrahedron()
    _mesh = Mesh(verts, faces)


def test_corefine():
    m0 = Mesh(*tetrahedron())
    m1 = Mesh(*tetrahedron(scale=0.9,  rot_z=pi/3))
    m0.corefine(m1)


@pytest.mark.parametrize('op', ['difference', 'union', 'intersection'])
def test_binary_op(op):
    m0 = Mesh(*tetrahedron())
    m1 = Mesh(*tetrahedron(scale=0.9, rot_z=pi / 3))
    result = getattr(m0, op)(m1)
    assert isinstance(result, Mesh)

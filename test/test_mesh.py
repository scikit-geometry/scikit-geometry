import pytest
import os

from tempfile import TemporaryDirectory
from numpy import array, cos, sin,  pi, ones, arange
from numpy.testing import assert_array_equal

from skgeom import Mesh


def tetrahedron(scale=1.0, rot_z=0.0):
    verts = scale * array([[1, 1, 1], [-1, 1, -1], [1, -1, -1], [-1, -1, 1]], dtype='float')

    if rot_z:
        rot = array([[cos(rot_z), -sin(-rot_z), 0], [sin(rot_z), cos(-rot_z), 0], [0, 0, 1]])
        verts = verts @ rot

    faces = array([[2, 1, 0], [2, 3, 1], [3, 2, 0], [1, 3, 0]], dtype='int')

    return verts, faces


@pytest.fixture
def mesh():
    return Mesh(*tetrahedron(), True)


def test_scalar_property(mesh):
    v = mesh.vertices[0]
    pmap = mesh.add_vertex_property('foo', -1)
    assert pmap[v] == -1
    pmap[v] = 1
    assert pmap[v] == 1
    assert pmap[mesh.vertices[1]] == -1


def test_vector_property(mesh):
    verts = mesh.vertices
    pmap = mesh.add_vertex_property('foo', 1)
    assert_array_equal(pmap[verts], ones(mesh.n_vertices))

    vals = arange(mesh.n_vertices)
    pmap[verts] = vals
    assert_array_equal(pmap[verts], vals)


@pytest.mark.parametrize('ext', ('off', 'ply'))
def test_io(mesh, ext):
    with TemporaryDirectory() as d:
        file = os.path.join(d, 'mesh.' + ext)
        getattr(mesh, 'write_' + ext)(file)
        _ = Mesh(file)


def test_corefine():
    m0 = Mesh(*tetrahedron(), True)
    m1 = Mesh(*tetrahedron(scale=0.9,  rot_z=pi/3), True)
    m0.corefine(m1)


@pytest.mark.parametrize('op', ['difference', 'union', 'intersection'])
def test_binary_op(op):
    m0 = Mesh(*tetrahedron(), True)
    m1 = Mesh(*tetrahedron(scale=0.9, rot_z=pi / 3), True)
    result = getattr(m0, op)(m1)
    assert isinstance(result, Mesh)

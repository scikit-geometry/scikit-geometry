# scikit-geometry

![scikit-geometry logo](docs/source/_static/scikit-geometry.svg)
[![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/scikit-geometry/scikit-geometry/master?filepath=notebooks)

*A Python Geometric Algorithms Library*

The scikit-geometry package contains a whole lot of geometric algorithms. For the moment, scikit-geometry derives most of it’s functionality from the mature and stable Computational Geometry Algorithms Library (CGAL) package, a collection of geometric algorithms written in C++.

Basic Geometric Types

We aim to cleanly encapsulate the important building blocks of a geometric library. As such we have implementations for the basic geometric types:

- `Point2`, `Point3`
- `Segment2`, `Ray2`, `Line2`, `Segment3`, `Ray3`, `Line3`
- `Vector2`, `Vector3`
- `Circle2`
- `IsoRectangle2`, `Bbox2`
- `Plane3`, `Triangle3`
- `Polyhedron3`

All sorts of operations can be performed with these basic building blocks. For example, one can construct a new segment from two points, then test against intersection with another segment, or a line.

```
import skgeom as sg
a = sg.Point2(5, 3)
b = sg.Point2(10, 8)
print(a, b)

>>> PointC2(5, 3), PointC2(10, 8)
```

Now we can create a vector from two points by subtracting one from another.

```
v = a - b
print(v)

>>> VectorC2(-5, -5)
```

Or a segment between two points, where a is the source and b the target.

```
s = sg.Segment2(a, b)
print(s)

>>> Segment_2(PointC2(5, 3), PointC2(10, 8))
```

## Documentation

The docs are hosted on github: [https://scikit-geometry.github.io/scikit-geometry](https://scikit-geometry.github.io/scikit-geometry)

It is built from Jupyter notebooks that can be found under the `/docs` subdirectory.

## Installation

### Conda

The package is published for Linux, OS X and Windows to the `conda-forge` channel. Users of conda can simply install via:

```
conda install scikit-geometry -c conda-forge
```

### Building from source

For users who use other package managers, the package has to be built from source. Therefore the dependencies for `CGAL` and `boost` have to be installed or updated:

#### Ubuntu 20.04+

```
sudo apt-get install cgal-dev libboost-all-dev
pip install -e . -v  # takes few minutes, grab a coffee ...
```

#### Ubuntu 18.04 (bionic)

Unfortunately, the Ubuntu packages for `CGAL` and `boost` are outdated. Thus the sources have to be downloaded separately (i.e. without `apt-get`):

1. For `CGAL`, download the [CGAL-5.4.tar.xz](https://github.com/CGAL/cgal/releases/download/v5.4/CGAL-5.4.tar.xz) archive and unpack it.
2. For `boost`, download the [boost_1_79_0.tar.gz](https://boostorg.jfrog.io/artifactory/main/release/1.79.0/source/boost_1_79_0.tar.gz) archive and unpack it.

Adjust the `setup.py` to following:

```python
# ...
include_dirs = [
    "./include/",
    "./src/docs/",
    "<download-path>/CGAL-5.4/include/",  # <- NEW
    "<download-path>/boost_1_79_0/",  ,  # <- NEW
    # Path to pybind11 headers
    get_pybind_include(),
    get_pybind_include(user=True),
]
# ...
```

Finally you can start the installation: 

```
pip install -e . -v
```

To make sure the installation was successful run:

```
echo -e "import skgeom; print(skgeom.Point2(0, 5))" | python
```

## License

This software is licensed under the LGPL-3 license. See the [LICENSE](LICENSE) file for details.

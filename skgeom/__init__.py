# import all contents from C++ extension module
from ._skgeom import *

try:
    from . import draw
except ImportError:
    import warnings
    warnings.warn(
        "The draw module requires the `matplotlib` package to be installed"
    )

from ._version import version_info, __version__
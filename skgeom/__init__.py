# import all contents from C++ extension module
from ._skgeom import *

try:
    from . import draw
except ImportError:
    pass

from ._version import version_info, __version__
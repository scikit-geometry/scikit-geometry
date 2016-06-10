import sys
sys.path.append('.')

from draw import *
from pygal import *

fn = './test/teapot.obj'
# with open('./test/teapot.obj') as fi:
p = polyhedron_from_string(fn)

import IPython
IPython.embed()

print(p)
from matplotlib import pyplot as plt
from matplotlib import cm
import matplotlib.patches as patches
from matplotlib.path import Path
from pygal.pygal import to_double

plt = plt

def draw(elem, plt=plt):
	if type(elem) == Polygon:
		draw_polygon(elem)

def to_list_of_tuples(iterable):
	return [(to_double(p.x()), to_double(p.y())) for p in iterable]

def draw_poly(poly=None, vertices=None, plt=plt, facecolor='none', plot_vertices=True):
	if poly:
		vertices = to_list_of_tuples(poly.vertices) + [(0, 0)]
	else:
		vertices = to_list_of_tuples(vertices) + [(0, 0)]
	length = len(vertices)
	# generate codes
	codes = [Path.MOVETO] + [Path.LINETO] * (length - 2) + [Path.CLOSEPOLY]

	# check if poly has holes ...
	try:
		for hole in poly.holes:
			new_vs =[(to_double(p.x()), to_double(p.y())) for p in hole.vertices] + [(0, 0)]
			new_length = len(new_vs)
			codes += [Path.MOVETO] + [Path.LINETO] * (new_length - 2) + [Path.CLOSEPOLY]
			vertices += new_vs
	except:
		pass

	path = Path(vertices, codes)
	patch = patches.PathPatch(path, facecolor=facecolor, lw=2)

	plt.gca().add_patch(patch)
	if plot_vertices:
		for v in vertices:
			plt.gca().add_patch(patches.Circle(v, 0.05, facecolor='red'))
			
	plt.gca().relim()
	# update ax.viewLim using the new dataLim
	plt.gca().autoscale_view()

def draw_list(l, as_poly=False, plt=plt, **kwargs):
	if as_poly:
		draw_poly(vertices=l)
	else:
		for p in l:
			draw_point(p)

def draw_point(point, **kwargs):	
	kkwargs = {
		'facecolor': 'red'
	}
	kkwargs.update(kwargs)

	plt.gca().add_patch(patches.Circle((
		to_double(point.x()), to_double(point.y())), 0.05, **kkwargs))


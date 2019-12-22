from ._skgeom import Segment2, Point2

def to_point(arg):
    if isinstance(arg, Point2):
        return arg
    if isinstance(arg, (tuple, list)):
        return Point2(*arg)
    raise TypeError("Could not convert {} to Point2".format(arg))

def segments_from_points(*args):
    l = []
    args = [to_point(a) for a in args]

    for a, b in zip(args[:-1], args[1:]):
        l.append(Segment2(a, b))

    l.append(Segment2(args[-1], args[0]))
    return l
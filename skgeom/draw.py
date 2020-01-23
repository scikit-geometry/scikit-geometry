from matplotlib import pyplot as plt
from matplotlib import cm
import matplotlib.patches as patches
from matplotlib.path import Path

import skgeom

import numpy as np
import math


def inner_subplots_adjust(axes, display_range=0):
    xmin, xmax = axes.get_xbound()
    ymin, ymax = axes.get_ybound()
    plt.xlim(xmin - display_range, xmax + display_range)
    plt.ylim(ymin - display_range, ymax + display_range)


def draw_point(point, **kwargs):
    plt.scatter([float(point.x())], [float(point.y())], **kwargs)


def draw_list(point_list, **kwargs):
    for point in point_list:
        draw_point(point, **kwargs)


def draw_vector(
    vector,
    source_x=0,
    source_y=0,
    display_range=None,
    vector_force=1,
    units="xy",
    angles="xy",
    scale_units="xy",
    scale=1,
    **kwargs
):
    fig, ax = plt.gcf(), plt.gca()
    vector_x = float(vector.x())
    vector_y = float(vector.y())

    q = ax.quiver(
        np.array(source_x),
        np.array(source_y),
        np.array(vector_x * vector_force),
        np.array(vector_y * vector_force),
        units=units,
        angles=angles,
        scale_units=scale_units,
        scale=scale,
        **kwargs
    )

    if display_range:
        inner_subplots_adjust(ax, display_range=display_range)
        ax.set_aspect("equal")
    else:
        plt.autoscale(True)


def draw_ray(
    ray,
    stop=0,
    num=100,
    arrow=False,
    visible_point=True,
    display_range=5,
    arrow_length=3,
    color="#1f77b4",
    units="xy",
    angles="xy",
    scale_units="xy",
    scale=1,
    **kwargs
):
    def set_xmargin(ax, left=0.0, right=0.0):
        ax.set_xmargin(0)
        ax.autoscale_view()
        lim = ax.get_xlim()
        delta = np.diff(lim)
        left = lim[0] - delta * left
        right = lim[1] + delta * right
        ax.set_xlim(left, right)

    fig, ax = plt.gcf(), plt.gca()
    source_x = float(ray.source().x())
    source_y = float(ray.source().y())
    direction_x = float(ray.direction().dx()) + source_x
    direction_y = float(ray.direction().dy()) + source_y

    vector = ray.to_vector()

    if source_x > direction_x:
        is_superior = True
        if stop >= source_x or stop == 0:
            stop = source_x - display_range
            x = np.linspace(stop, source_x, num)
        else:
            x = np.linspace(stop, source_x, num)
    else:
        is_superior = False
        if stop <= source_x or stop == 0:
            stop = source_x + display_range
            x = np.linspace(source_x, stop, num)
        else:
            x = np.linspace(source_x, stop, num)

    line = ray.supporting_line()
    y = (-line.a() * x - line.c()) / line.b()

    if arrow:
        draw_vector(
            vector,
            source_x=source_x,
            source_y=source_y,
            vector_force=arrow_length,
            color=color,
        )

    ax.plot(x, y, color=color, **kwargs)
    if visible_point:
        draw_point(ray.source(), color=color)

    if is_superior:
        set_xmargin(ax, left=0, right=0.05)

    else:
        set_xmargin(ax, left=0.05, right=0)


def draw_direction(
    direction,
    source_x=0,
    source_y=0,
    display_range=None,
    units="xy",
    angles="xy",
    scale_units="xy",
    scale=1,
    **kwargs
):
    fig, ax = plt.gcf(), plt.gca()
    direction_x = float(direction.dx())
    direction_y = float(direction.dy())

    normalized_x = direction_x / math.sqrt(direction_x ** 2 + direction_y ** 2)
    normalized_y = direction_y / math.sqrt(direction_x ** 2 + direction_y ** 2)

    q = ax.quiver(
        np.array(source_x),
        np.array(source_y),
        np.array(normalized_x),
        np.array(normalized_y),
        units=units,
        angles=angles,
        scale_units=scale_units,
        scale=scale,
        **kwargs
    )

    if display_range:
        inner_subplots_adjust(ax, display_range=display_range)
        ax.set_aspect("equal")
    else:
        plt.autoscale(True)


def draw_bbox(bbox, display_range=5, fill=False, **kwargs):
    fig, ax = plt.gcf(), plt.gca()
    x_min = float(bbox.xmin())
    x_max = float(bbox.xmax())
    y_min = float(bbox.ymin())
    y_max = float(bbox.ymax())

    plt.gca().add_patch(
        patches.Rectangle(
            (x_min, y_min), x_max - x_min, y_max - y_min, fill=fill, **kwargs
        )
    )

    plt.xlim(x_min - display_range, x_max + display_range)
    plt.ylim(y_min - display_range, x_max + display_range)


def draw_line(line, start=-5, stop=5, num=100, **kwargs):
    fig, ax = plt.gcf(), plt.gca()
    x = np.linspace(start, stop, num)
    y = (-line.a() * x - line.c()) / line.b()

    ax.plot(x, y, **kwargs)
    ax.margins(0)


def draw_iso_rectangle(rectangle, display_range=5, fill=False, **kwargs):
    fig, ax = plt.gcf(), plt.gca()
    x_min = float(rectangle.xmin())
    x_max = float(rectangle.xmax())
    y_min = float(rectangle.ymin())
    y_max = float(rectangle.ymax())

    plt.gca().add_patch(
        patches.Rectangle(
            (x_min, y_min), x_max - x_min, y_max - y_min, fill=fill, **kwargs
        )
    )

    plt.xlim(x_min - display_range, x_max + display_range)
    plt.ylim(y_min - display_range, y_max + display_range)


def draw_segment(segment, visible_point=True, color="#1f77b4", **kwargs):
    fig, ax = plt.gcf(), plt.gca()
    plt.plot(
        [segment.source().x(), segment.target().x()],
        [segment.source().y(), segment.target().y()],
        color=color,
        **kwargs
    )
    if visible_point:
        draw_list([segment.source(), segment.target()], color=color, **kwargs)


def draw_circle(circle, display_range=5, fill=False, **kwargs):
    fig, ax = plt.gcf(), plt.gca()
    center_x = float(circle.center().x())
    center_y = float(circle.center().y())
    radius = math.sqrt(float(circle.squared_radius()))

    circle_displayed = plt.Circle((center_x, center_y), radius, fill=fill, **kwargs)

    ax.add_artist(circle_displayed)
    ax.set_aspect("equal")

    plt.xlim(center_x - radius - display_range, center_x + radius + display_range)
    plt.ylim(center_y - radius - display_range, center_y + radius + display_range)


def to_list_of_tuples(iterable):
    return [(float(p.x()), float(p.y())) for p in iterable]


def draw_polygon(
    polygon=None,
    polygon_with_holes=None,
    vertices=None,
    plt=plt,
    facecolor="lightgray",
    point_color="none",
    line_width=2,
    plot_vertices=True,
    aspect_ratio=1,
    alpha=1,
):
    fig, ax = plt.gcf(), plt.gca()
    vertices = to_list_of_tuples(polygon.vertices) + [(0, 0)]

    if plot_vertices:
        for v in polygon.vertices:
            draw_point(v, color=point_color)

    codes = [Path.MOVETO] + [Path.LINETO] * (len(vertices) - 2) + [Path.CLOSEPOLY]

    if polygon_with_holes:
        for hole in polygon_with_holes.holes:
            if plot_vertices:
                for v in hole.vertices:
                    draw_point(v, color=point_color)

            hole_vertices = list(to_list_of_tuples(hole.vertices)) + [(0, 0)]
            codes.extend([Path.MOVETO] + [Path.LINETO] * (len(hole_vertices) - 2) + [Path.CLOSEPOLY])
            vertices.extend(hole_vertices)

    path = Path(vertices, codes)
    plt.gca().add_patch(patches.PathPatch(path, facecolor=facecolor, lw=line_width, alpha=alpha))

    plt.gca().relim()
    plt.gca().autoscale_view()
    if aspect_ratio is not None:
        plt.gca().set_aspect(aspect_ratio)


def draw_polygon_with_hole(polygon_with_hole, **kwargs):
    draw_polygon(polygon_with_hole.outer_boundary(), polygon_with_holes=polygon_with_hole, **kwargs)


def draw_polygon_set(polygon_set, **kwargs):
    for polygon in polygon_set.polygons:
        draw_polygon_with_hole(polygon, **kwargs)


def draw_voronoi(
    voronoi,
    display_range=3,
    visible_point=True,
    edges_color="blue",
    sites_color="red",
    vertices_color="None",
    finite_edges_color="blue",
    aspect_ratio=1,
):
    fig, ax = plt.gcf(), plt.gca()
    for edge in voronoi.edges:
        source, target = edge.source(), edge.target()
        if source and target:
            plt.plot(
                [source.point().x(), target.point().x()],
                [source.point().y(), target.point().y()],
                color=edges_color,
            )

    bbox = skgeom.Bbox2()
    for site in voronoi.sites:
        bbox += site.bbox()
        draw_point(site, color=sites_color)

    for vertice in voronoi.vertices:
        draw_point(vertice.point(), color=vertices_color)

    for el in voronoi.finite_edges:
        draw(el, visible_point=visible_point, color=finite_edges_color)

    plt.xlim(bbox.xmin() - display_range, bbox.xmax() + display_range)
    plt.ylim(bbox.ymin() - display_range, bbox.ymax() + display_range)

    if aspect_ratio is not None:
        plt.gca().set_aspect(aspect_ratio)


def draw(obj, **kwargs):
    if isinstance(obj, skgeom._skgeom.Point2):
        draw_point(obj, **kwargs)
    elif isinstance(obj, skgeom._skgeom.Vector2):
        draw_vector(obj, **kwargs)
    elif isinstance(obj, skgeom._skgeom.Ray2):
        draw_ray(obj, **kwargs)
    elif isinstance(obj, skgeom._skgeom.Direction2):
        draw_direction(obj, **kwargs)
    elif isinstance(obj, skgeom._skgeom.Bbox2):
        draw_bbox(obj, **kwargs)
    elif isinstance(obj, skgeom._skgeom.Line2):
        draw_line(obj, **kwargs)
    elif isinstance(obj, skgeom._skgeom.IsoRectangle2):
        draw_iso_rectangle(obj, **kwargs)
    elif isinstance(obj, skgeom._skgeom.Segment2):
        draw_segment(obj, **kwargs)
    elif isinstance(obj, skgeom._skgeom.Circle2):
        draw_circle(obj, **kwargs)
    elif isinstance(obj, skgeom._skgeom.Polygon):
        draw_polygon(obj, **kwargs)
    elif isinstance(obj, skgeom._skgeom.PolygonWithHoles):
        draw_polygon_with_hole(obj, **kwargs)
    elif isinstance(obj, skgeom._skgeom.PolygonSet):
        draw_polygon_set(obj, **kwargs)
    elif isinstance(obj, skgeom._skgeom.voronoi.VoronoiDiagram):
        draw_voronoi(obj, **kwargs)
    elif isinstance(obj, list):
        for el in obj:
            draw(el, **kwargs)

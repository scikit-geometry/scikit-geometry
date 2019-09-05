// Copyright (c) 2005  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of cgal-python; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; version 2.1 of the License.
// See the file LICENSE.LGPL distributed with cgal-python.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $Id: Py_Polyhedron_3_doc.h 159 2007-11-02 09:54:13Z spion $
// $URL: svn+ssh://scm.gforge.inria.fr/svn/cgal-python/trunk/cgal-python/bindings/Polyhedron/Py_Polyhedron_3_doc.h $
//
// Author(s)     : Naceur Meskini
//=========================================================================
#ifndef POLYHEDRON_3_DOCS_H
#define POLYHEDRON_3_DOCS_H
//=======================================================
// doc of Polyhedron_3 class
//=======================================================
const char* Polyhedron_3_doc = "\
	A polyhedral surface Polyhedron_3 consists of vertices V, edges E,\n\
	facets F and an incidence relation on them.\n\
	Creation:\n\
	P = Polyhedron_3()\n\
	the C++ documentation contains the wide explication of this class:\n\
	http://www.cgal.org/Manual/3.2/doc_html/cgal_manual/Polyhedron_ref/Class_Polyhedron_3.html";

const char* reserve_doc = "\
	P.reserve ( self , int v, int h, int f) -> void\n\
	reserve storage for v vertices, h halfedges, and f facets. \n\
	The reservation sizes are a hint for optimizing storage allocation. \n\
	If the capacity is already greater than the requested size nothing happens.";

const char* make_tetrahedron_doc = "\
	P.make_tetrahedron ( self) -> Halfedge\n\
	a tetrahedron is added to the polyhedral surface. \n\
	Returns a halfedge of the tetrahedron.\n\n\
	P.make_tetrahedron( self,Point_3 p1,Point_3 p2,Point_3 p3,Point_3 p4) -> Halfedge\n\
	a tetrahedron is added to the polyhedral surface with its vertices\n\
 	initialized to p1, p2, p3, and p4. \n\
	Returns that halfedge of the tetrahedron which incident vertex is\n\
 	initialized to p1. The incident vertex of the next halfedge is p2, \n\
	and the vertex thereafter is p3. \n\
	The remaining fourth vertex is initialized to p4.";

const char* make_triangle_doc = "\
	P.make_triangle( self ) -> Halfedge\n\
	a triangle with border edges is added to the polyhedral surface. \n\
	Returns a non-border halfedge of the triangle.\n\n\
	P.make_triangle( self ,Point_3 p1, Point_3 p2, Point_3 p3) -> Halfedge\n\
	a triangle with border edges is added to the polyhedral surface \n\
	with its vertices initialized to p1, p2, and p3. \n\
	Returns that non-border halfedge of the triangle which incident \n\
	vertex is initialized to p1.\n\
	The incident vertex of the next halfedge is p2, and the vertex\n\
	thereafter is p3.";

const char* get_allocator_doc = "\
";

const char* size_of_vertices_doc = "\
	P.size_of_vertices( self ) -> int \n\
	number of vertices.";

const char* size_of_halfedges_doc = "\
	P.size_of_halfedges( self ) -> int \n\
	number of halfedges (incl. border halfedges).";

const char* size_of_facets_doc = "\
	P.size_of_facets( self ) -> int \n\
	number of facets.";

const char* empty_doc = "\
	P.empty( self ) -> bool\n\
	returns true if P is empty.";

const char* capacity_of_vertices_doc = "\
	P.capacity_of_vertices( self ) -> int\n\
	space reserved for vertices.";

const char* capacity_of_halfedges_doc = "\
	P.capacity_of_halfedges( self ) -> int\n\
	space reserved for halfedges.";

const char* capacity_of_facets_doc = "\
	P.capacity_of_facets( self ) -> int\n\
	space reserved for facets.";

const char* bytes_doc = "\
	P.bytes( self ) -> int\n\
	bytes used for the polyhedron.";

const char* bytes_reserved_doc = "\
	P.bytes_reserved( self ) -> int \n\
	bytes reserved for the polyhedron.";

const char* traits_doc = "\
";
const char* is_closed_doc = "\
	P.is_closed( self) -> bool\n\
	returns true if there are no border edges.";

const char* is_pure_bivalent_doc = "\
	P.is_pure_bivalent( self) -> bool\n\
	returns true if all vertices have exactly two incident edges.";

const char* is_pure_trivalent_doc = "\
	P.is_pure_trivalent( self ) -> bool\n\
	returns true if all vertices have exactly three incident edges.";

const char* is_pure_triangle_doc = "\
	P.is_pure_triangle( self ) -> bool\n\
	returns true if all facets are triangles.";

const char* is_pure_quad_doc = "\
P.is_pure_quad( self ) -> bool\n\
	returns true if all facets are quadrilaterals.";

const char* is_triangle_doc = "\
P.is_triangle( self , Halfedge h) -> bool\n\
	true iff the connected component denoted by h is a triangle.";

const char* is_tetrahedron_doc = "\
	P.is_tetrahedron( self ,Halfedge h) -> bool\n\
	true iff the connected component denoted by h is a tetrahedron.";

const char* split_facet_doc = "\
	P.split_facet( self, Halfedge h, Halfedge g) -> Halfedge\n\
	splits the facet incident to h and g into two facets with a new \n\
	diagonal between the two vertices denoted by h and g respectively. \n\
	The second (new) facet is a copy of the first facet. \n\
	Returns h.next() after the operation, i.e., the new diagonal. \n\
	The new face is to the right of the new diagonal, the old face is \n\
	to the left. The time is proportional to the distance from h to g \n\
	around the facet.\n\
	Precondition: h and g are incident to the same facet. h != g .\n\
	h.next() != g and g.next() != h (no multi-edges).";

const char* join_facet_doc = "\
	P.join_facet( self ,Halfedge h) -> Halfedge\n\
	joins the two facets incident to h. \n\
	The facet incident to h.opposite() gets removed. \n\
	Both facets might be holes. \n\
	Returns the predecessor of h around the facet. \n\
	The invariant join_facet( split_facet( h, g)) returns h and keeps \n\
	the polyhedron unchanged. The time is proportional to the size of \n\
	the facet removed and the time to compute h.prev().\n\
	Precondition: The degree of both vertices incident to h is at \n\
	least three (no antennas).\n\
	Requirement: Supports_removal CGAL::Tag_true.";

const char* split_vertex_doc = "\
	P.split_vertex( self , Halfedge h, Halfedge g) -> Halfedge\n\
	splits the vertex incident to h and g into two vertices, \n\
	the old vertex remains and a new copy is created,and connects \n\
	them with a new edge. Let hnew be h.next().opposite() after the \n\
	split, i.e., a halfedge of the new edge. The split regroups the\n\
	halfedges around the two vertices. The halfedge sequence hnew,\n\
 	g.next().opposite(), ..., h remains around the old vertex, while\n\
	the halfedge sequence hnew.opposite(), h.next().opposite() \n\
	(before the split), ..., g is regrouped around the new vertex. \n\
	The split returns hnew, i.e., the new halfedge incident to the old vertex. \n\
	The time is proportional to the distance from h to g around the 	vertex.\n\
	Precondition: h and g are incident to the same vertex. h != g (antennas are not allowed).\n\
	Note: A special application of the split is \n\
	split_vertex(h.next().opposite()) which is equivalent to an edge \n\
	split of the halfedge h->next() that creates a new vertex on the\n\
	halfedge h.next(). See also split_edge(h) below.";

const char* join_vertex_doc = "\
	P.join_vertex( self , Halfedge h) -> Halfedge\n\
	joins the two vertices incident to h. \n\
	The vertex denoted by h.opposite() gets removed. \n\
	Returns the predecessor of h around the vertex, i.e., h.opposite().prev().\n\
	The invariant join_vertex( split_vertex( h, g)) returns h and keeps the\n\
 	polyhedron unchanged.The time is proportional to the degree of the vertex\n\
	removed and the time to compute h.prev() and h.opposite().prev().\n\
	Precondition: The size of both facets incident to h is at least four";

const char* split_edge_doc = "\
	P.split_edge( self , Halfedge h) -> Halfedge\n\
	splits the halfedge h into two halfedges inserting a new vertex \n\
	that is a copy of h.opposite().vertex(). \n\
	Is equivalent to split_vertex( h.prev(), h.opposite()). \n\
	The call of prev() can make this method slower than a direct call of\n\
	split_vertex() if the previous halfedge is already known and computing \n\
	it would be costly when the halfedge data structure does not support the\n\
	prev() member function. Returns the new halfedge hnew pointing to the \n\
	inserted vertex. The new halfedge is followed by the old halfedge, \n\
	i.e., hnew->next() == h.";

const char* flip_edge_doc = "\
	P.flip_edge( self , Halfedge h) -> Halfedge\n\
	performs an edge flip. It returns h after rotating the edge h one vertex \n\
	in the direction of the face orientation.\n\
	Precondition: h != Halfedge() and both facets incident to h are triangles.";

const char* create_center_vertex_doc = "\
	P.create_center_vertex( self , Halfedge h) -> Halfedge\n\
	barycentric triangulation of h.facet(). Creates a new vertex, a copy of\n\
	h.vertex(), and connects it to each vertex incident to h.facet() splitting\n\
	h->facet() into triangles. h remains incident to the original facet, all \n\
	other triangles are copies of this facet. Returns the halfedge h.next() \n\
	after the operation, i.e., a halfedge pointing to the new vertex. The time \n\
	is proportional to the size of the facet.\n\
	Precondition: h is not a border halfedge.";

const char* erase_center_vertex_doc = "\
	P.erase_center_vertex( self , Halfedge g) -> Halfedge\n\
	reverses create_center_vertex. Erases the vertex pointed to by g and all\n\
	incident halfedges thereby merging all incident facets. Only g.facet() \n\
	remains. The neighborhood of g.vertex() may not be triangulated, it can \n\
	have larger facets. Returns the halfedge g.prev(). Thus, the invariant \n\
	h == erase_center_vertex( create_center_vertex(h)) holds if h is not a \n\
	border halfedge. The time is proportional to the sum of the size of all\n\
	incident facets.\n\
	Precondition: None of the incident facets of g.vertex() is a hole. \n\
	There are at least two distinct facets incident to the facets that are\n\
	incident to g.vertex(). (This prevents the operation from collapsing a \n\
	volume into two facets glued together with opposite orientations, such as\n\
	would happen with any vertex of a tetrahedron.)";

const char* split_loop_doc = "\
	P.split_loop( self , Halfedge h,Halfedge i,Halfedge j) -> Halfedge\n\
	cuts the polyhedron into two parts along the cycle (h,i,j) (edge j \n\
	runs on the backside of the three dimensional figure above). Three new\n\
	vertices (one copy for each vertex in the cycle) and three new halfedges\n\
	(one copy for each halfedge in the cycle), and two new triangles are created.\n\
	h,i,j will be incident to the first new triangle. The return value will be \n\
	the halfedge incident to the second new triangle which is the copy of h.opposite().\n\
	Precondition: h,i,j denote distinct, consecutive vertices of the polyhedron\n\
	and form a cycle: i.e., h.vertex() == i.opposite().vertex(), ...,\n\
	j.vertex() == h.opposite().vertex(). The six facets incident to h,i,j are \n\
	all distinct.";

const char* join_loop_doc = "\
	P.join_loop( self , Halfedge h, Halfedge g) -> Halfedge\n\
	glues the boundary of the two facets denoted by h and g together and returns\n\
	h. Both facets and the vertices along the facet denoted by g gets removed.\n\
	Both facets may be holes. The invariant join_loop( h, split_loop( h, i, j))\n\
	returns h and keeps the polyhedron unchanged.\n\
	Precondition: The facets denoted by h and g are different and have equal\n\
	degree (i.e., number of edges).";

const char* make_hole_doc = "\
	P.make_hole( self, Halfedge h) -> Halfedge\n\
	removes the incident facet of h and changes all halfedges incident to the\n\
 	facet into border edges. Returns h.See erase_facet(h) for a more generalized variant.\n\
	Precondition: None of the incident halfedges of the facet is a border edge.";

const char* fill_hole_doc = "\
	P.fill_hole( self, Halfedge h) -> Halfedge\n\
	fills a hole with a newly created facet. Makes all border halfedges of the\n\
	hole denoted by h incident to the new facet. Returns h.\n\
	Precondition: h.is_border().";

const char* add_vertex_and_facet_to_border_doc = "\
	P.add_vertex_and_facet_to_border( self ,Halfedge h,Halfedge g) -> Halfedge\n\
	creates a new facet within the hole incident to h and g by connecting the \n\
	tip of g with the tip of h with two new halfedges and a new vertex and filling\n\
	this separated part of the hole with a new facet, such that the new facet is\n\
	incident to g. Returns the halfedge of the new edge that is incident to the\n\
	new facet and the new vertex.\n\
	Precondition: h.is_border(), g.is_border(), h != g, and g can be reached \n\
	along the same hole starting with h.";

const char* add_facet_to_border_doc = "\
	P.add_facet_to_border( self , Halfedge h ,Halfedge g) -> Halfedge\n\
	creates a new facet within the hole incident to h and g by connecting \n\
	the vertex denoted by g with the vertex denoted by h with a new halfedge \n\
	and filling this separated part of the hole with a new facet, such that the\n\
	new facet is incident to g. Returns the halfedge of the new edge that is\n\
	incident to the new facet.\n\
	Precondition: h.is_border(), g.is_border(), h != g, h.next() != g, and g can\n\
	be reached along the same hole starting with h.";

const char* erase_facet_doc = "\
	P.erase_facet( self , Halfedge h) -> void\n\
	removes the incident facet of h and changes all halfedges incident to the\n\
	facet into border edges or removes them from the polyhedral surface if they\n\
	were already border edges. If this creates isolated vertices they get removed\n\
	as well. See make_hole(h) for a more specialized variant. \n\
	Precondition: h.is_border() == false.";

const char* erase_connected_component_doc = "\
	P.erase_connected_component( self , Halfedge h) -> void\n\
	removes the vertices, halfedges, and facets that belong to the connected\n\
	component of h.";

const char* clear_doc = "\
	P.clear( self) -> void\n\
	removes all vertices, halfedges, and facets.";

const char* erase_all_doc = "\
";

const char* delegate_doc = "\
";

const char* size_of_border_halfedges_doc = "\
	P.size_of_border_halfedges( self ) -> int\n\
	number of border halfedges.\n\
	Precondition: last normalize_border() call still valid, see above.";

const char* size_of_border_edges_doc = "\
	P.size_of_border_edges( self ) -> int\n\
	number of border edges. Since each border edge of a polyhedral surface has\n\
	exactly one border halfedge, this number is equal to size_of_border_halfedges().\n\
	Precondition: last normalize_border() call still valid, see above.";

const char* normalized_border_is_valid_doc = "\
	P.normalized_border_is_valid( self , bool verbose = false) -> bool\n\
	returns true if the border halfedges are in normalized representation, which is\n\
	when enumerating all halfedges with the iterator: The non-border edges precede\n\
	the border edges and for border edges, the second halfedge is the border\n\
	halfedge. The halfedge iterator border_halfedges_begin() denotes the first border\n\
	edge. If verbose is true, statistics are printed to cerr.";

const char* normalize_border_doc = "\
";

const char* inside_out_doc = "\
	P.inside_out( self ) -> void\n\
	reverses facet orientations (incl. plane equations if supported).";

const char* is_valid_doc = "\
	P.is_valid( self , bool verbose = false, int level = 0) -> bool\n\
	returns true if the polyhedral surface is combinatorially consistent. \n\
	If verbose is true, statistics are printed to cerr. For level == 1 the \n\
	normalization of the border edges is checked too. This method checks in \n\
	particular level 3 of CGAL::Halfedge_data_structure_decorator::is_valid \n\
	from page reference and that each facet is at least a triangle and that \n\
	the two incident facets of a non-border edge are distinct.";

const char* vertices_doc = "\
";
const char* facets_doc = "\
";
const char* points_doc = "\
";
const char* edges_doc = "\
";
const char* border_edges_doc = "\
";
const char* planes_doc = "\
";
const char* halfedges_doc = "\
";
const char* border_halfedges_doc = "\
";

#endif  //POLYHEDRON_3_DOCS_H


Sqrt 3 subdivision was implemented.

Changes to the template code:

TriMesh.hpp:
1) in Half-Edge structure a field m_mpnt_ was added - its a pointer to the node that is used in subdivision scheme.
2) in Node structure an int field was added - its an index used for subdivision.
3) A utility function beta_func was declared.

Oblig4App.cpp:
1) Keys 'k' and 'l' were assigned to switch between wireframe/regular view.

TriMesh.cpp:
1) Connectivity function was implemented using lecure slides as reference.

TriMeshGL.cpp:
1) Normal function was implemented using lecture slides as reference.

TriMeshSqrt3.cpp:

Square root 3 subdivision was implemented. The algorithm was derived from lecture slides. No distinction between boundary and other vertices is present.
The code first assigns indices to existing nodes and then creates new nodes (midpoints) and fills the newly created m_mpnt_ field in Half-Edge structures. Then the old nodes' positions are adjusted. For greater readability functions beta and beta_func were implemented. After adjusting the old nodes the algorithm iterates over half-edges and creates a new triangle for each half-edge, thus ensuring 1->3 triangles multiplication.

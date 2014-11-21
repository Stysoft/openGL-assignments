/* -*- mode: C++; tab-width:4; c-basic-offset: 4; indent-tabs-mode:nil -*-
 *
 * Time-stamp: <2005-07-13 12:01:05 dyken>
 *
 * (C) 2005 Christopher Dyken, <dyken@cma.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#include "TriMesh.hpp"

#include <iostream>
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>

using std::string;
using std::vector;

namespace GfxUtil {

float beta(float n){
    float result = 4.0;
    result -= glm::cos((2.0*M_PI)/n);
    result /= 9.0 * n; 
    return result;
}

glm::vec3 generate_midpoint(glm::vec3 p,glm::vec3 q,glm::vec3 r){
    glm::vec3 barymetric_middle(0,0,0);
    barymetric_middle = (p+q+r);
    barymetric_middle /= 3.0;
    return barymetric_middle;
}

glm::vec3 TriMesh::beta_func(GLuint i, float n){
    glm::vec3 final_pos(0.0,0.0,0.0);
    float b = beta(n);
    final_pos = m_nodes[i].m_pos_;
    final_pos *= (1.0 - (n*b));

    TriMesh::HalfEdge* edge = m_nodes[i].m_he_;
    final_pos += (edge -> m_next_ -> m_from_ -> m_pos_) * b;

    // iterating over other neighbours; 
    // twin->next is an edge originating from the same vertex and pointing to a neighbour
    for(;;){
        edge = edge -> getVtxRingNext();
        //check for boundary
        if (edge == NULL) break;
        //check if it's the end
        if (edge == m_nodes[i].m_he_) break;
        final_pos += (edge -> m_next_ -> m_from_ -> m_pos_) * b;
    }
    return final_pos;
}

TriMesh* TriMesh::subdivideSqrt3() {
  vector<glm::vec3> points;
  vector<int> indices;

  GLuint i, index = 0;
  vector<Node*> midpoints;
  // Generate a new set of points and indices using the sqrt(3) subdivision scheme
  // unskip

  // First we assign all the nodes their indices.
  for(i=0;i<m_nodes.size();++i){
    m_nodes[i].ind = index;
    index++;
  }
  std::cout << "Assigned indexes to the old nodes. \n";

  // Now we generate midpoints for each triangle and make a new node that each halfedge of the triangle points to.
  for(i=0;i<m_triangles.size();++i){
    Node* a = m_triangles[i]-> m_he_ -> m_from_;
    Node* b = m_triangles[i]-> m_he_ -> m_next_ -> m_from_;
    Node* c = m_triangles[i]-> m_he_ -> m_next_ -> m_next_ -> m_from_;
    
    glm::vec3 midpoint = generate_midpoint(a->m_pos_,b->m_pos_,c->m_pos_);
    Node* mid = new Node(midpoint);
    mid->ind = index;
    index++;
    
    midpoints.push_back(mid);
    // Halfedges of the triangle should point to the newly generated midpoint.
    m_triangles[i]-> m_he_ -> m_mpnt_ = mid;
    m_triangles[i]-> m_he_ -> m_next_ -> m_mpnt_ = mid;
    m_triangles[i]-> m_he_ -> m_next_ -> m_next_ -> m_mpnt_ = mid;
  }
  
  std::cout << "Generated the midpoints. \n";

  // We update the original vertices.
  for(i=0;i<m_nodes.size();++i){
    // We count the neighbours of the vertex.
    float count = 1.0;
    HalfEdge* edge = m_nodes[i].m_he_;
    for(;;){
        edge = edge -> getVtxRingNext(); 
        if (edge == m_nodes[i].m_he_ || edge == NULL) break;
        count += 1.0;
    }
    //TODO boundary nodes
    // We apply the beta function.
    glm::vec3 updated_position = beta_func(i, count);
    m_nodes[i].m_pos_ = updated_position;
  }

  std::cout << "Updated the old vertices. \n";

  // We create the list of vertices.
  for(i=0;i<m_nodes.size();++i){
    points.push_back(m_nodes[i].m_pos_);
  }

  // We can add the midpoints to the rest of the nodes.
  for(i=0;i<midpoints.size();++i){
    points.push_back(midpoints[i]->m_pos_);
  }

  std::cout << "Created vertices list. \n";

  // Now we're ready to create the list of indices.
  // For each half-edge a triangle source -> my midpoint -> twins midpoint is created.
  for(i=0; i<m_halfedges.size(); i++){
    indices.push_back(m_halfedges[i]->m_from_->ind);
    indices.push_back(m_halfedges[i]->m_mpnt_->ind);
    indices.push_back(m_halfedges[i]->m_twin_-> m_mpnt_->ind);
  }

  std::cout << "finished subdivision! \n";
  return new TriMesh(points, indices); 
}

}  // GfxUtil

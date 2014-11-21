/* -*- mode: C++; tab-width:4; c-basic-offset: 4; indent-tabs-mode:nil -*-
 *
 * Time-stamp: <2005-07-13 12:01:05 dyken>
 *
 * (C) 2005 Christopher Dyken, <dyken@cma.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#include "TriMesh.hpp"

#include <stdexcept>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::string;
using std::vector;

namespace GfxUtil {

void TriMesh::render() {  
  // Bind VBO's
  glBindBuffer(GL_ARRAY_BUFFER, m_vertices_vbo_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices_vbo_id);
 
  // Setup the strides, offsets etc.
  glVertexPointer(3, GL_FLOAT, sizeof(Node), BUFFER_OFFSET(0));
  glNormalPointer(GL_FLOAT, sizeof(Node), BUFFER_OFFSET(12));
  
  // Enable the above arrays
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  
  // Call the drawing function
  glDrawElements(GL_TRIANGLES, 3*m_triangles.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  
  // Unbind VBOs
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  // Disable arrays
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}

void TriMesh::prepareBufferObjects(const vector<int>& indices) { 
  glGenBuffers(1, &m_vertices_vbo_id);
  glGenBuffers(1, &m_indices_vbo_id);

  // Bind VBOs and provide data to them  
  glBindBuffer(GL_ARRAY_BUFFER, m_vertices_vbo_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices_vbo_id);
  
  glBufferData(GL_ARRAY_BUFFER, m_nodes.size() * sizeof(Node), &m_nodes[0], GL_STATIC_DRAW);    
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TriMesh::computeNormals() {
  // skip
  // Calculate triangle normals
  // Calculate vertex normals by averaging triange normals
  // unskip
  for(int i = 0; i<m_triangles.size(); i++){
    Node* a = m_triangles[i]-> m_he_ -> m_from_;
    Node* b = m_triangles[i]-> m_he_ -> m_next_ -> m_from_;
    Node* c = m_triangles[i]-> m_he_ -> m_next_ -> m_next_ -> m_from_;
    m_triangles[i]->m_N_ = glm::normalize(glm::cross(  
    (b->m_pos_ - a->m_pos_), (c->m_pos_ - a->m_pos_)
    ));
  }

  //vertex normals
  for(int i = 0; i< m_nodes.size(); i++){
	//Node node = m_nodes[i];
    float count = 1.0;
    glm::vec3 normal(0.0,0.0,0.0);
    HalfEdge* edge = m_nodes[i].m_he_;
    normal += edge->getTriangle()->m_N_;
    for(;;){
        edge = edge -> getVtxRingNext(); 
        if (edge == NULL) break;
        if (edge == m_nodes[i].m_he_) break;
        normal += edge->getTriangle()->m_N_;
        count += 1.0;
    }
    
    normal /= count;
    m_nodes[i].m_N_ = normal;
  }

}

}  // GfxUtil

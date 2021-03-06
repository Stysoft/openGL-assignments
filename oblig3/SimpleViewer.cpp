/* $Id: SimpleViewer.cpp, v1.1 2011/09/20$
 *
 * Author: Christopher Dyken, <dyken@cma.uio.no>
 * Reviewed by: Bartlomiej Siwek, <bartloms@ifi.uio.no>
 *
 * Distributed under the GNU GPL.
 */
#define _USE_MATH_DEFINES
#include "SimpleViewer.hpp"

#include <cmath>
#include <stdexcept>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GfxUtil {
  
SimpleViewer::SimpleViewer() {
  setViewVolume(glm::vec3(0.0f, 0.0f, 0.0f), 1.74);
}

void SimpleViewer::setWindowSize(int w, int h) {
  m_window_width = w;
  m_window_height = h;
  m_window_aspect = (float)w / (float)h;
}

void SimpleViewer::setViewVolume(glm::vec3 origin, float radius) {
  // skip
  // set viewvolume related variables...
  m_viewvolume_origin = origin;
  m_viewvolume_radius = radius;
  m_viewvolume_epsilon = 0.01;
  // unskip
}

void SimpleViewer::rotationBegin(int x, int y) {
  // skip
  // handle beginning of the rotation...
  m_unit_mousepos_i = getNormalizedCoords(x,y);
  m_rotation_mousepos_i = getPointOnUnitSphere(m_unit_mousepos_i);

  m_rotation_orientation_i = m_camera_orientation;
  m_state = ROTATING;
  
  // unskip
}

void SimpleViewer::panBegin(int x, int y) {
  // skip
  // handle beginning of panning... (OPTIONAL)
  // unskip
}

void SimpleViewer::zoomBegin(int x, int y) {
  // skip
  // handle beginning of zoom... (OPTIONAL)
  // unskip
}

void SimpleViewer::resetState(int /*x*/, int /*y*/) {
  m_state = NONE;
}

void SimpleViewer::motion(int x, int y) {
  // skip
  // handle mouse motion while one of the mouse buttons is down...
  m_rotation_mousepos_c = getPointOnUnitSphere(getNormalizedCoords(x,y));
  glm::fquat temp_quat = getGreatCircleRotation(m_rotation_mousepos_i, m_rotation_mousepos_c);
  m_camera_orientation = temp_quat * m_rotation_orientation_i;

  // getting the angle and axis
  glm::vec3 cross_prod = glm::cross(m_rotation_mousepos_i, m_rotation_mousepos_c);
  float norm = glm::length(cross_prod);
  if(norm == 0.0)
    norm = 0.001; // hack to avoid division by 0
  m_rotation_axis = cross_prod/norm;    
  float dot_prod = glm::dot(m_rotation_mousepos_i, m_rotation_mousepos_c);
  m_rotation_angle = glm::acos(dot_prod);
  


  // unskip
}

glm::mat4x4 SimpleViewer::getProjectionMatrix() {
  glm::mat4x4 result;
  
  // skip
  // compute projection matrix...
   result = glm::frustum(1.0,-1.0,1.0, -1.0, 0.6, 5.0);

  // unskip
  
  return result;
}

glm::mat4x4 SimpleViewer::getModelViewMatrix() {
  glm::mat4x4 result(1.0f);
  
  // skip 
  // compute modelview matrix (a rotation should be preceded by a translation of the camera)...
  
  result  = glm::translate(result, glm::vec3(0.0,0.0,-3.0));
  result *= glm::mat4_cast(m_camera_orientation);
  

  // unskip
  
  return result;
}

void SimpleViewer::renderDebugGraphics() {
  // This is a debug method that uses the deprecated API
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-m_window_aspect, m_window_aspect, -1.0f, 1.0f, -1.0f, 1.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  switch(m_state) {
    case ROTATING:
      glLineWidth(3.0);

      glColor3f(0.4, 0.4, 0.4);
      glBegin(GL_LINE_LOOP);
      for(size_t i=0; i<50; i++)
        glVertex2f(glm::cos(2.0*M_PI*i/(float)50), glm::sin(2.0*M_PI*i/(float)50));
      glEnd();

      glBegin(GL_LINES);
      glColor3f(1.0, 0.5, 0.5);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3fv(&m_rotation_axis[0]);

      glColor3f(0.5, 1.0, 0.5);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3fv(&m_rotation_mousepos_i[0]);

      glColor3f(0.5, 0.5, 1.0);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3fv(&m_rotation_mousepos_c[0]);

      glEnd();
      break;
    default:
      // Do nothing.
      break;
  }
}



glm::vec2 SimpleViewer::getNormalizedCoords(int x, int y) {
  // skip
  // compute the position of mouse in a coordinate system convinet for us...
  float x_p = ((float)x/m_window_width) - 0.5; 
  float y_p = 0.5 - ((float)y/m_window_height);
  
  return glm::vec2(x_p, y_p);
  // unskip
}

glm::vec3 SimpleViewer::getPointOnUnitSphere(glm::vec2 p) {
  // skip
  // project mouse position in a convinient coordinate system to a unit sphere...
  float r = sqrt (p[0] * p[0] + p[1] * p[1]);
  if (r < 0.5){
    return glm::vec3(2.0 * p[0], 2.0 * p[1], sqrt(1.0 - 4*r*r));
  }
  return glm::vec3(p[0]/r,p[1]/r,0);

  // unskip
}

// This function computes the quaternion corresponding to the great circle rotation between two vectors
glm::fquat SimpleViewer::getGreatCircleRotation(glm::vec3 a, glm::vec3 b) {
  glm::vec3 axis = glm::cross(a, b);
  float length = glm::length(axis);
  if(length > std::numeric_limits<float>::epsilon()) {
    axis = axis / length;

    float angle = glm::acos(glm::dot(a,b));
    float c = glm::cos(0.5*angle);
    float s = glm::sin(0.5*angle);
    return glm::fquat(c, s*axis[0], s*axis[1], s*axis[2]);
  } else {
    return glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);
  }
}

// This function computes the quaternion corresponding to the rotation by a give angle around given axis
glm::fquat SimpleViewer::getAxisAngleRotation(glm::vec3& axis, float angle) {
  float c = glm::cos(0.5f * angle);
  float s = glm::sin(0.5f * angle);
  return glm::fquat(c, s*axis[0], s*axis[1], s*axis[2]);
}

} // namespace GfxUtil

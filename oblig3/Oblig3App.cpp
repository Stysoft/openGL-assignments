/* $Id: Oblig3App.cpp, v1.1 2011/09/20$
 *
 * Author: Atgeirr F Rasmussen <atgeirr@sintef.no>
 * Reviewed by: Bartlomiej Siwek, <bartloms@ifi.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#include "Oblig3App.hpp"

#include <iostream>
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <streambuf>

#include "OpenGLError.hpp"
#include "ReadTextfile.hpp"

void Oblig3App::initGL() {
  glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  
  m_sphere_.init(64, 64, 1.5f);
  
  m_texture_.loadTex("EarthDiffuse.dds");
	m_texture_.SetParameters();
  
  setupShaders();
  setupLightParameters();
}

void Oblig3App::display() {
  // Clear buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up projection matrix
  glMatrixMode(GL_PROJECTION);
  glm::mat4x4 projection_matrix = m_viewer_.getProjectionMatrix();
  glLoadMatrixf(glm::value_ptr(projection_matrix));

  // Set the matrix mode to Modelview
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  //Light stationary with regard to camera (because MV matrix is empty)
  GLfloat light0_position[] = { 2.0f,  2.0f,  2.0f,  1.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  
  // skip
  // Activate the sahader
	m_shader_.Activate();
	
  // Bind the texture and bind it to the uniform in the shader
	GLuint tex_coords, sampler_loc;
	//glBindTexture(GL_TEXTURE_2D, m_texture_.GetTextureId());
	sampler_loc = m_shader_.GetUniform("DiffuseTex");
	glUniform1i(sampler_loc,m_texture_.GetTextureId());
	
  // Set the value of the SpecularColor uniform in the FragmentShader
	GLuint spec_color = m_shader_.GetUniform("SpecularColor");
	glUniform4f(spec_color,0.5,0.5,0.5,0.1);
  //unskip
  
  // Render the sphere
  glm::mat4x4 model_view_matrix = m_viewer_.getModelViewMatrix();
  glLoadMatrixf(glm::value_ptr(model_view_matrix));
  m_sphere_.render();

  // Deactivate the shader
  m_shader_.Deactivate();

  // DEBUG
  //m_viewer_.renderDebugGraphics();

  glutSwapBuffers();
  CHECK_OPENGL;
}

void Oblig3App::reshape(int w, int h) {
  // skip
  glViewport(0, 0, w, h);
  m_viewer_.setWindowSize(w,h);
  // unskip
}

void Oblig3App::keyboard(unsigned char key, int /*x*/, int /*y*/) {
  if (key == 'q') {
    std::exit(0);
  }
	if (key == 'f') {
    std::cout << comp_log_frag;
  }
	if (key == 'v') {
    std::cout << comp_log_vert;
  }
	if (key == 'o') {
    setupShaders();
  }
	if (key == 'a') {
    std::cout << m_shader_.DumpAttributes();
  }
	if (key == 'u') {
    std::cout << m_shader_.DumpUniforms();
	
  }
}

void Oblig3App::mouse(int b, int s, int x, int y) {
  // skip
  if (s == GLUT_DOWN) {
    switch (b) {
      case GLUT_LEFT_BUTTON:   m_viewer_.rotationBegin(x, y); break;
      case GLUT_MIDDLE_BUTTON: m_viewer_.panBegin(x, y); break;
      case GLUT_RIGHT_BUTTON:  m_viewer_.zoomBegin(x, y); break;
    }
  } else {
    m_viewer_.resetState(x,y);
  }
  // unskip
  glutPostRedisplay();
}

void Oblig3App::motion(int x, int y) {
  // skip
  m_viewer_.motion(x, y);
  // unskip
  glutPostRedisplay();
}


int Oblig3App::displayMode() {
  return GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH;
}

std::string Oblig3App::windowName() {
  return "Oblig 3";
}

void Oblig3App::setupShaders() {
  // skip
  // Load and compile the shader files (see Shader class)
	std::ifstream fs_file("MyShader.frag");
	std::string frag_shader((std::istreambuf_iterator<char>(fs_file)),
                 std::istreambuf_iterator<char>());
	std::ifstream vs_file("MyShader.vert");
	std::string vert_shader((std::istreambuf_iterator<char>(vs_file)),
                 std::istreambuf_iterator<char>());


	m_shader_.CompileShader(GL_FRAGMENT_SHADER, frag_shader);
	m_shader_.CompileShader(GL_VERTEX_SHADER, vert_shader);

  	comp_log_frag = m_shader_.GetCompileLog(GL_FRAGMENT_SHADER);
	comp_log_vert = m_shader_.GetCompileLog(GL_VERTEX_SHADER);
// Link the shader program
	m_shader_.LinkProgram();
	

  // unskip
}

void Oblig3App::setupLightParameters() {
  //Light atributes
  GLfloat black_light[] = {  0.0f,  0.0f,  0.0f,  1.0f };
  GLfloat white_light[] = {  1.0f,  1.0f,  1.0f,  1.0f };
  
  //Setup light 0
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  black_light);
  
  // Enable the light
  glEnable(GL_LIGHT0); 
}

/* $Id: oblig1_gasket.cpp, v1.0 2011/09/07$
 *
 * Author: Bartlomiej Siwek, <bartloms@ifi.uio.no>
 * 
 * Distributed under the GNU GPL.
 */

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>

#include <glm/glm.hpp>

#include "OpenGLError.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Vertex structure
struct Vertex { 
  Vertex(float x, float y)
      : position(x, y) {
  }
  
  Vertex(const glm::vec2 &p) 
      : position(p) {
  }
  
  glm::vec2 position;
};


// Global variables
static std::vector<Vertex> vertices;
static std::vector<unsigned int> indices;
static int level = 0;


// glm::lerp gives an error 'lerp is not a member of glm'
glm::vec2 lerp(glm::vec2 &a, glm::vec2 &b, float weight){
  return glm::vec2( (1.0-weight)*a[0] + weight*b[0], (1.0-weight)*a[1] + weight*b[1] );
}

// Vertex buffer object IDs
 GLuint varrayID;
const GLuint vertexbufferID = 0;
const GLuint indexbufferID = 1;
const GLuint NUM_BUFFERS = 2;
GLuint buffers[NUM_BUFFERS];

// ... end of your code ... 

void sierpinskiGasket(unsigned int a, unsigned int b, unsigned int c, unsigned int level) {
  if(level == 0) {
	indices.push_back(a);
	indices.push_back(b);
	indices.push_back(c);
  } else {
    // Generate new vertices and call this function recursively

/*
*         C
*        /\
*       E  F
*      /    \
*     A-- D--B    
*/

	Vertex ver_d(lerp(vertices[a].position,vertices[b].position,0.5)); //vertex D halves |AB|
	Vertex ver_e(lerp(vertices[a].position,vertices[c].position,0.5)); //vertex E halves |AC|
	Vertex ver_f(lerp(vertices[b].position,vertices[c].position,0.5)); //vertex F halves |BC|

	unsigned int d, e, f;
	// add new vertices to the vector	
	vertices.push_back(ver_d);
	d = vertices.size()-1;
	vertices.push_back(ver_e);
	e = vertices.size()-1;
	vertices.push_back(ver_f);
	f = vertices.size()-1;

	// recursively call the function for triangles A - D - E, D - B - F and E - F - C
    // the awesome ASCII art shows that it is indeed CCW.

	sierpinskiGasket(a, d, e, level-1);
    sierpinskiGasket(d, b, f, level-1);
	sierpinskiGasket(e, f, c, level-1);

    // ... end of your code ... 
  }
}

void rebuildGasket() {
  // Clear the data arrays
  vertices.clear();
  indices.clear();
  
  // Insert basic vertices
  vertices.push_back(Vertex(-1.0f, -1.0f));
  vertices.push_back(Vertex( 1.0f, -1.0f));
  vertices.push_back(Vertex( 0.0f,  1.0f));
  
  // Start the recursion
  sierpinskiGasket(0, 1, 2, level);
}

void bindDataToBuffers() {
  // Bind VBOs and provide data to them

  glBindVertexArray(varrayID);
  glBindBuffer(GL_ARRAY_BUFFER, buffers[vertexbufferID]);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * 
	sizeof(Vertex),
	&vertices[0],
	GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[indexbufferID]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
	indices.size() * sizeof(GLuint)
	,&indices[0],
	GL_STATIC_DRAW);
  
  // Unbind the VBO's after we are done (NOT REALLY NECESSARY)
  //glBindBuffer(GL_ARRAY_BUFFER, 0);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  
  CHECK_OPENGL;
}

void myInit()
{
  // Initialize the gasket
  rebuildGasket();
  
  // Generate VBO ids
  // ... insert your code here ...
  glGenVertexArrays(1, &varrayID);
  glGenBuffers(NUM_BUFFERS,buffers);
  
  // ... end of your code ... 
   
  // Bind data to buffers
  bindDataToBuffers();

  // Initialize OpenGL specific part
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glColor3f(1.0f, 1.0f, 1.0f); // setting active color to white

  
  CHECK_OPENGL;
}

void myKeyboard(unsigned char key, int /* x */, int /* y */)
{
  switch(key) {
    case '+':
      level++;
      // Rebuild gasket and bind data to buffers
      // ... insert your code here ...
	  rebuildGasket();
	  bindDataToBuffers();
      // ... end of your code ... 
      break;
    case '-':
      level = level > 0 ? level-1 : 0;
      // Rebuild gasket and bind data to buffers
      // ... insert your code here ...
	  rebuildGasket();
	  bindDataToBuffers();
      // ... end of your code ... 
      break;
  }
  
  glutPostRedisplay();
}

void myDisplay()
{
  // Clear the backgound
  glClear(GL_COLOR_BUFFER_BIT);
 
  // Bind VBO's and call a drawing function
  // ... insert your code here ...
  
  glEnableClientState(GL_VERTEX_ARRAY);
  bindDataToBuffers();
  //glVertexPointer(3,GL_FLOAT, 0, &vertices[0]);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);
  // ... end of your code ... 
  
  // Switch the buffer
  glFlush();
  glutSwapBuffers();
  
  CHECK_OPENGL;
}

void myShutdown() {
  // Delete VBOs
  // ... insert your code here ...
  glDeleteBuffers(2,buffers);
  glDeleteVertexArrays(1, &varrayID); 
  // ... end of your code ... 
}

int main(int argc, char **argv)
{
  // Initialization of GLUT
  glutInit(&argc, argv);
  glutInitContextVersion(3, 1);
  glutInitContextFlags(GLUT_DEBUG);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(512, 512);
  glutCreateWindow( __FILE__ );

  // Init GLEW	
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    return 1;
  } 

  if(!GLEW_VERSION_3_1) {
    std::cerr << "Driver does not support OpenGL 3.1" << std::endl;  
    return 1;
  }
  
  // Attach handlers
  glutDisplayFunc(myDisplay);
  glutKeyboardFunc(myKeyboard);
  
  // A nasty trick to get a shutdown handler
  atexit(myShutdown);
  
  // Application specific initialization
  myInit();
  
  // Run the GLUT main loop
  glutMainLoop();
  return 0;
}

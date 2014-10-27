#include "Texture.hpp"

#include <stdexcept>
#include <map>
#include <GL/glew.h>
#include <gli/gli.hpp>
#include <gli/gtx/gl_texture2d.hpp>
#include <GL/gl.h>

namespace GfxUtil {

Texture::Texture()
    : m_texture_id_(0) {
}

Texture::~Texture() {
  if(m_texture_id_ != 0) {
    // skip
    // Insert texture deletion here
    // unskip
  }
}

GLuint Texture::loadTex(std::string filename) { 
	GLuint TextureName;
	if(m_texture_id_ == 0) {
    gli::texture2D Texture(gli::loadStorageDDS(filename));
    glGenTextures(1, &TextureName);
    assert(!Texture.empty());
    glBindTexture(GL_TEXTURE_2D, TextureName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, GLint(Texture.levels() - 1));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
    glTexImage2D(GL_TEXTURE_2D, 
        0, 
        GL_RGB8, 
        GLsizei(Texture[0].dimensions().x), 
        GLsizei(Texture[0].dimensions().y), 
        0, 
        GL_BGRA, 
        GL_UNSIGNED_BYTE, 
        Texture[0].data());
	}
	return TextureName;
}

void Texture::SetParameters() {
  // Load only if we havent done so already
    // skip
    // Use gli::createTexture2D to load and create texture name and bind it NOT WORKING

    // Set wrapping to repear in bith S and T directions
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set linear interpolation for magnification and linear-mipmap-linear for minification
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // Construct mipmaps
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    // unskip
  
}

GLuint Texture::GetTextureId() {
  return m_texture_id_;
}

}  // GfxUtil

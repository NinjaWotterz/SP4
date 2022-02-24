#ifndef DEPTH_FBO_H
#define DEPTH_FBO_H

#include <GL/glew.h>

class DepthFBO
{
public:
    DepthFBO();
    ~DepthFBO();

    bool Init(unsigned width, unsigned height);
    void BindForWriting();
    void BindForReading(GLenum textureUnit);

	GLuint GetFBO() {return m_fbo;}
	GLuint GetTexture() {return m_texture;}
	unsigned GetWidth() {return m_width;}
	unsigned GetHeight() {return m_height;}

private:
    GLuint m_fbo;		// The frame buffer object, to be written to
    GLuint m_texture;	// Depth texture, to be sampled later in your shader
	unsigned m_width;
	unsigned m_height;
};

#endif

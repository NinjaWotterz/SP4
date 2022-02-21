#include "DepthFBO.h"
#include <iostream>

DepthFBO::DepthFBO()
{
	//Allocate memory for frame buffer object and texture
	glGenFramebuffers(1, &m_fbo);
	glGenTextures(1, &m_texture);
}

DepthFBO::~DepthFBO()
{
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteTextures(1, &m_texture);
}

bool DepthFBO::Init(unsigned width, unsigned height)
{
	m_width = width;
	m_height = height;

	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
 
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0); //increase 1024 if needed
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_texture, 0);
 
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
 
	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer error" << std::endl;
		return false;
	}
	return true;
}

void DepthFBO::BindForWriting()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, m_width, m_height);
}

void DepthFBO::BindForReading(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
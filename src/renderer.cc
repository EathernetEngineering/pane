#include "renderer.h"

#include <stdexcept>

#include <GL/gl.h>

namespace pane {
bool Renderer::s_bGLInitialized = false;

Renderer::Renderer()
 : m_bInitialized(false)
{
}

Renderer::~Renderer() {
	this->Shutdown();
}

void Renderer::Init() {
	if (!s_bGLInitialized) {
		GLenum ec = glewInit();
		if (ec != GLEW_OK) {
			throw std::runtime_error("Failed to initialize GLEW");
		}
		s_bGLInitialized = true;
	}
}

void Renderer::Shutdown() {
	if (!m_bInitialized) {
		return;
	}
}

void Renderer::RenderFrame() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.0f, 0.8f, 1.0f);
}
}


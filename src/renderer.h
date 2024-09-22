#ifndef CEE_PANE_RENDERER_H_
#define CEE_PANE_RENDERER_H_

#include <GL/glew.h>

namespace pane {
class Renderer {
public:
	Renderer();
	~Renderer();

	void Init();
	void Shutdown();

	void UpdateImage(const void* pPixels);
	void RenderFrame();

private:
	static void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);

private:
	bool m_bInitialized;

	GLuint m_uVAO;
	GLuint m_uVBO;
	GLuint m_uIBO;
	GLuint m_uTexture;
	GLuint m_uShaderProgram;

	static bool s_bGLInitialized;
};
}

#endif


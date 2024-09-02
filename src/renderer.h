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

	void RenderFrame();

private:
	bool m_bInitialized;

	static bool s_bGLInitialized;
};
}

#endif


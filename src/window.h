#ifndef CEE_PANE_WINDOW_H_
#define CEE_PANE_WINDOW_H_

#include <string>

#include <cstdint>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace pane {
class Window {
public:
	Window();
	~Window();

	void Init(uint32_t nWidth, uint32_t nHeight, const std::string& sTitle);
	void Shutdown();

	bool ShouldClose();

	void SwapBuffers();
	void PollEvents();

private:
	static void ErrorCallback(int ec, const char* desc);

private:
	uint32_t m_nWidth, m_nHeight;
	std::string m_sTitle;

	GLFWwindow* m_hWindow;

	static bool s_bGLFWInitialized;
	static uint32_t s_nWindowCount;
};
}

#endif


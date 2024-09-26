#ifndef CEE_PANE_WINDOW_H_
#define CEE_PANE_WINDOW_H_

#include <string>
#include <memory>
#include <deque>

#include <cstdint>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "event.h"

namespace pane {
class Window {
public:
	Window();
	~Window();

	void Init(uint32_t nWidth, uint32_t nHeight, const std::string& sTitle);
	void Shutdown();

	bool ShouldClose();

	void SwapBuffers();
	std::shared_ptr<Event> PollEvents(int32_t* nEvents = nullptr);
	std::shared_ptr<Event> PeekEvents(int32_t* nEvents = nullptr);

	std::string GetTitle() const { return m_sTitle; }
	void GetSize(uint32_t* nWidth, uint32_t* nHeight) const { *nWidth = m_nWidth; *nHeight = m_nHeight; }
	uint32_t GetWidth() const { return m_nWidth; }
	uint32_t GetHeight() const { return m_nHeight; }
	void GetPosition(int32_t* nX, int32_t* nY) const { glfwGetWindowPos(m_hWindow, nX, nY); }

	void SetTitle(const std::string& sTitle);
	void SetSize(uint32_t nWidth, uint32_t nHeight);
	void SetWidth(uint32_t nWidth);
	void SetHeight(uint32_t nHeight);
	void SetPosition(int32_t nX, int32_t nY);

private:
	static void ErrorCallback(int ec, const char* desc);

private:
	uint32_t m_nWidth, m_nHeight;
	std::string m_sTitle;

	GLFWwindow* m_hWindow;

	std::deque<std::shared_ptr<Event>> m_xEventQueue;

private:
	static bool s_bGLFWInitialized;
	static uint32_t s_nWindowCount;
};
}

#endif


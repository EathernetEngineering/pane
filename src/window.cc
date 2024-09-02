#include "window.h"
#include <GLFW/glfw3.h>

#include <stdexcept>

namespace pane {

bool Window::s_bGLFWInitialized = false;
uint32_t Window::s_nWindowCount = 0;

Window::Window()
 : m_nWidth(0), m_nHeight(0), m_sTitle(), m_hWindow(0)
{
}

Window::~Window() {
	this->Shutdown();
}

void Window::Init(uint32_t nWidth, uint32_t nHeight, const std::string& sTitle) {
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_sTitle = sTitle;

	if (!s_bGLFWInitialized) {
		if (!glfwInit()) {
			throw std::runtime_error("Failed to initialize GLFW!");
		}

		glfwSetErrorCallback(Window::ErrorCallback);

		s_bGLFWInitialized = true;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_hWindow = glfwCreateWindow(m_nWidth, m_nHeight, m_sTitle.c_str(), NULL, NULL);
	
	if (!m_hWindow) {
		throw std::runtime_error("Failed to create window!");
	}

	glfwMakeContextCurrent(m_hWindow);
	++s_nWindowCount;
}

void Window::Shutdown() {
	if (s_nWindowCount == 0 && s_bGLFWInitialized == true) {
		glfwTerminate();
		s_bGLFWInitialized = false;
	}

	if (!m_hWindow) {
		return;
	}

	glfwDestroyWindow(m_hWindow);
	m_hWindow = nullptr;
	--s_nWindowCount;
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(m_hWindow);
}

void Window::SwapBuffers() {
	glfwSwapBuffers(m_hWindow);
}

void Window::PollEvents() {
	glfwPollEvents();
}

void Window::ErrorCallback(int ec, const char* desc) {
	fprintf(stderr, "GLFW Error (%i): %s\n", ec, desc);
}
}


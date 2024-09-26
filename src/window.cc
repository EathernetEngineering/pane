#include "window.h"
#include "event.h"
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

	glfwSetWindowUserPointer(m_hWindow, this);

	glfwSetWindowSizeCallback(m_hWindow, [](GLFWwindow* window, int width, int height){
		Window* paneWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

		paneWindow->m_nWidth = width;
		paneWindow->m_nHeight = height;

		paneWindow->m_xEventQueue.push_back(std::make_shared<WindowResizeEvent>(width, height));
	});

	glfwSetWindowPosCallback(m_hWindow, [](GLFWwindow* window, int xpos, int ypos){
		Window* paneWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

		paneWindow->m_xEventQueue.push_back(std::make_shared<WindowPositionEvent>(xpos, ypos));
	});

	glfwSetWindowCloseCallback(m_hWindow, [](GLFWwindow* window){
		Window* paneWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

		paneWindow->m_xEventQueue.push_back(std::make_shared<WindowCloseEvent>());
	});

	glfwSetWindowFocusCallback(m_hWindow, [](GLFWwindow* window, int focused){

		Window* paneWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

		if (focused) {
			paneWindow->m_xEventQueue.push_back(std::make_shared<WindowFocusEvent>());
		} else {
			paneWindow->m_xEventQueue.push_back(std::make_shared<WindowUnfocusEvent>());
		}
	});

	glfwSetKeyCallback(m_hWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods){
		Window* paneWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

		if (action == GLFW_PRESS) {
			paneWindow->m_xEventQueue.push_back(std::make_shared<KeyDownEvent>(key));
		} else if (action == GLFW_RELEASE) {
			paneWindow->m_xEventQueue.push_back(std::make_shared<KeyUpEvent>(key));
		} else if (action == GLFW_REPEAT) {
			paneWindow->m_xEventQueue.push_back(std::make_shared<KeyRepeatEvent>(key));
		}
	});
	
	glfwSetCursorPosCallback(m_hWindow, [](GLFWwindow* window, double xpos, double ypos){
		Window* paneWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

		paneWindow->m_xEventQueue.push_back(std::make_shared<MouseMoveEvent>(xpos, ypos));
	});

	glfwSetMouseButtonCallback(m_hWindow, [](GLFWwindow* window, int button, int action, int mods){
		Window* paneWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

		if (action == GLFW_PRESS) {
			paneWindow->m_xEventQueue.push_back(std::make_shared<ButtonDownEvent>(button));
		} else if (action == GLFW_RELEASE) {
			paneWindow->m_xEventQueue.push_back(std::make_shared<ButtonUpEvent>(button));
		}
	});

	glfwSetScrollCallback(m_hWindow, [](GLFWwindow* window, double xoffset, double yoffset){
		Window* paneWindow = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));

		paneWindow->m_xEventQueue.push_back(std::make_shared<ScrollEvent>(xoffset, yoffset));
	});

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
	glfwPollEvents();
}

std::shared_ptr<Event> Window::PollEvents(int32_t* nEvents) {
	if (nEvents != nullptr) {
		*nEvents = m_xEventQueue.size();
	}
	if (m_xEventQueue.size() > 0) {
		std::shared_ptr<Event> e = m_xEventQueue.front();
		m_xEventQueue.pop_front();
		return e;
	}
	return nullptr;
}

std::shared_ptr<Event> Window::PeekEvents(int32_t* nEvents) {
	if (nEvents != nullptr) {
		*nEvents = m_xEventQueue.size();
	}
	if (m_xEventQueue.size() > 0)
		return m_xEventQueue.front();
	else
		return nullptr;
}

void Window::SetTitle(const std::string& sTitle) {
	m_sTitle = sTitle;
	glfwSetWindowTitle(m_hWindow, m_sTitle.c_str());
}

void Window::SetSize(uint32_t nWidth, uint32_t nHeight) {
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	glfwSetWindowSize(m_hWindow, m_nWidth, m_nHeight);
}

void Window::SetWidth(uint32_t nWidth) {
	m_nWidth = nWidth;
	glfwSetWindowSize(m_hWindow, m_nWidth, m_nHeight);

}

void Window::SetHeight(uint32_t nHeight) {
	m_nHeight = nHeight;
	glfwSetWindowSize(m_hWindow, m_nWidth, m_nHeight);

}

void Window::SetPosition(int32_t nX, int32_t nY) {
	glfwSetWindowPos(m_hWindow, nX, nY);
}

void Window::ErrorCallback(int ec, const char* desc) {
	fprintf(stderr, "GLFW Error (%i): %s\n", ec, desc);
}
}


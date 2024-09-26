#include "renderer.h"

#include <stdexcept>
#include <format>

#include <GL/gl.h>

#include "ppu.h"

namespace pane {
static const char* g_sVertexShaderGlsl = 
	"#version 450 core\n"
	"\n"
	"layout (location = 0) in vec2 a_Pos;\n"
	"layout (location = 1) in vec2 a_TexCoords;\n"
	"\n"
	"layout (location = 0) out vec2 v_TexCoords;\n"
	"\n"
	"void main() {\n"
	"	gl_Position = vec4(a_Pos, 0.0f, 1.0f);\n"
	"	v_TexCoords = a_TexCoords;\n"
	"}\n"
	"\n";
static const char* g_sFragmentShaderGlsl =
	"#version 450 core\n"
	"\n"
	"layout (location = 0) out vec4 oDiffuseColor;\n"
	"\n"
	"layout (location = 0) in vec2 v_TexCoords;\n"
	"layout (binding = 0) uniform sampler2D u_Sampler;\n"
	"\n"
	"void main() {\n"
	"	oDiffuseColor = texture(u_Sampler, v_TexCoords);\n"
	"}\n"
	"\n";

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
		glDebugMessageCallback(Renderer::DebugCallback, this);
		glEnable(GL_DEBUG_OUTPUT);
	} else {
		throw std::runtime_error("Attempting to initialize renderer twice!");
	}

	// Vertex format X, Y, U, V
	float pVertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
	};
	uint16_t pIndices[] = {
		0, 1, 2, 2, 3, 0
	};

	glGenVertexArrays(1, &m_uVAO);
	glBindVertexArray(m_uVAO);

	glGenBuffers(1, &m_uVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_uVBO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void*>(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 16, pVertices, GL_STATIC_DRAW);
	glGenBuffers(1, &m_uIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * 6, pIndices, GL_STATIC_DRAW);

	glGenTextures(1, &m_uTexture);
	glBindTexture(GL_TEXTURE_2D, m_uTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, PANE_NES_VISIBLE_IMAGE_WIDTH, PANE_NES_VISIBLE_IMAGE_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	GLuint uVertexShader;
	GLuint uFragmentShader;
	int32_t bSuccess;

	uVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(uVertexShader, 1, &g_sVertexShaderGlsl, nullptr);
	glCompileShader(uVertexShader);
	glGetShaderiv(uVertexShader, GL_COMPILE_STATUS, &bSuccess);
	if (!bSuccess) {
		char sCompileErrorMessage[512];
		glGetShaderInfoLog(uVertexShader, 512, nullptr, sCompileErrorMessage);
		throw std::runtime_error(std::string(sCompileErrorMessage));
	}

	uFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(uFragmentShader, 1, &g_sFragmentShaderGlsl, nullptr);
	glCompileShader(uFragmentShader);
	glGetShaderiv(uFragmentShader, GL_COMPILE_STATUS, &bSuccess);
	if (!bSuccess) {
		char sCompileErrorMessage[512];
		glGetShaderInfoLog(uFragmentShader, 512, nullptr, sCompileErrorMessage);
		throw std::runtime_error(std::string(sCompileErrorMessage));
	}

	m_uShaderProgram = glCreateProgram();
	glAttachShader(m_uShaderProgram, uVertexShader);
	glAttachShader(m_uShaderProgram, uFragmentShader);
	glLinkProgram(m_uShaderProgram);
	glGetProgramiv(m_uShaderProgram, GL_LINK_STATUS, &bSuccess);
	if (!bSuccess) {
		char sLinkErrorMessage[512];
		glGetProgramInfoLog(m_uShaderProgram, 512, nullptr, sLinkErrorMessage);
		throw std::runtime_error(std::string(sLinkErrorMessage));
	}

	glDeleteShader(uVertexShader);
	glDeleteShader(uFragmentShader);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Renderer::Shutdown() {
	if (!m_bInitialized) {
		return;
	}
}

void Renderer::UpdateImage(const void* pPixels) {
	glBindTexture(GL_TEXTURE_2D, m_uTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, PANE_NES_VISIBLE_IMAGE_WIDTH, PANE_NES_VISIBLE_IMAGE_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, pPixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::RenderFrame() {
	glClearColor(0.2f, 0.0f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_uShaderProgram);
	glBindTexture(GL_TEXTURE_2D, m_uTexture);
	glBindVertexArray(m_uVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void Renderer::DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM) {
		throw std::runtime_error(std::format("Error Message: {}", message));
	}
}
}


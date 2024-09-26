set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

set(PANE_CXX_SOURCES main.cc cpu.cc mmu.cc ppu.cc window.cc renderer.cc emulator.cc)
add_executable(pane ${PANE_CXX_SOURCES})

target_link_libraries(pane GLEW::glew ${OPENGL_LIBRARIES} glfw glm)

if(UNIX AND NOT APPLE)
	install(TARGETS pane RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif()


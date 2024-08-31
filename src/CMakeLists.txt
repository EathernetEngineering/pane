set(NANJAE_CXX_SOURCES main.cc cpu.cc ram.cc)

add_executable(nanjae ${NANJAE_CXX_SOURCES})

if(UNIX AND NOT APPLE)
	install(TARGETS nanjae RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif()


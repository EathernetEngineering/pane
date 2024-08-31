cmake_minimum_required(VERSION 3.5.0)

project(nanjae VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(WIN32)
	set(PLATFORM "windows")
	message(FATAL_ERROR "Cannot build installer for this unsupported platform")
elseif(APPLE)
	set(PLATFORM "mac")
	message(FATAL_ERROR "Cannot build installer for this unsupported platform")
elseif(UNIX)
	set(PLATFORM "linux")
else()
	message(FATAL_ERROR "Cannot build installer for this unsupported platform")
endif()

include(GNUInstallDirs)

add_subdirectory(src/)


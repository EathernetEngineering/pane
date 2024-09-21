#include <stdexcept>
#include <iostream>

#include <cstdlib>

#include "emulator.h"

int main(int argc, char** argv) {

	pane::Emulator emu;

	try {
		emu.Init();
	} catch (const std::runtime_error& e) {
		std::cout << "Initialization error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	try {
		emu.Run();
	} catch (const std::runtime_error& e) {
		std::cout << "Emulation error: " << e.what() << std::endl;
		emu.Shutdown();
		return EXIT_FAILURE;
	}

	emu.Shutdown();

	return EXIT_SUCCESS;
}

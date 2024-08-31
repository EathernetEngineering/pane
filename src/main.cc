#include <iostream>
#include <cstdlib>
#include <memory>

#include "cpu.h"

int main(int argc, char** argv) {
	std::shared_ptr<nanjae::CPU> cpu = std::make_shared<nanjae::CPU>();
	std::shared_ptr<nanjae::RAM> ram = std::make_shared<nanjae::RAM>();

	cpu->SetRAM(ram);

	try {
		while (1) {
			cpu->Execute();
		}
	} catch (std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	return EXIT_SUCCESS;
}

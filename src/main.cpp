#include "project.h"

int main(void) {
	glengine engine;

	if (engine.initialize() == -1)
		return -1;

	engine.initializeResources();

	engine.run();

	engine.terminate();

	return 0;
}

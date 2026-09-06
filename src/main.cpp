#include "project.h"

int main(void) {
	project::dataCPU gameDataCPU;
	project::dataGPU gameDataGPU;

	if (project::initResourcesCPU(gameDataCPU) == -1) 	return -1;
	project::initResourcesGPU(gameDataGPU);

	project::loop(gameDataCPU, gameDataGPU);

	project::terminate();

	return 0;
}

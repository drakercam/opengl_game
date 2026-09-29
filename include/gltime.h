#ifndef GLTIME_H
#define GLTIME_H

#include <cstdint>

struct gltime {
	double last = 0.0;
	double delta = 0.0;
	double total = 0.0;
};

void gltimeUpdate(gltime& t);

struct gltimer {
	double duration = 0.0;
	double elapsed = 0.0;
	bool active = false;
	bool repeat = false;
};

void gltimerStart(gltimer& t, double duration, bool repeat);
void gltimerStop(gltimer& t);
void gltimerReset(gltimer& t);
bool gltimerUpdate(gltimer& t, double delta);

#endif

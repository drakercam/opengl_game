#include "gltime.h"

#include <SDL3/SDL.h>

void gltimeUpdate(gltime& t) {
    double now = (double)SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();
    
    if (t.last == 0.0) {
		t.last = now;
		return;
    }
    
    t.delta = now - t.last;
    t.last = now;
    t.total += t.delta;
}

void gltimerStart(gltimer& t, double duration, bool repeat) {
	t.duration = duration;
	t.elapsed = 0.0;
	t.active = true;
	t.repeat = repeat;
}

void gltimerStop(gltimer& t) {
	t.active = false;
}

void gltimerReset(gltimer& t) {
	t.elapsed = 0.0;
	t.active = false;
}

bool gltimerUpdate(gltimer& t, double delta) {
	if (!t.active) return false;
	
	t.elapsed += delta;
	
	if (t.elapsed >= t.duration) {
		
		if (t.repeat) {
			t.elapsed -= t.duration;
		}
		else {
			t.active = false;
		}
		
		return true;
	}
	
	return false;
}


#include "gltools.h"

#include <iostream>

static bool windowClose = false;
static SDL_GLContext glContext = nullptr;

static float mouseDeltaX = 0.0f;
static float mouseDeltaY = 0.0f;

void setVersionGL(int major, int minor) {

#ifdef __EMSCRIPTEN__
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	
#else

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

#endif

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

SDL_Window* createWindow(int width, int height, const char* title) {
	return SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
}

void setContextCurrent(SDL_Window* window) {
	glContext = SDL_GL_CreateContext(window);
	if (!glContext) {
		SDL_Log("Failed to create OpenGL context: %s", SDL_GetError());
		return;
	}
	
	SDL_GL_MakeCurrent(window, glContext);
	SDL_GL_SetSwapInterval(1);
}

void enableGL(int settingGL) {
	glEnable(settingGL);
}

void disableGL(int settingGL) {
	glDisable(settingGL);
}

int maxNumberVertexAttrGL(void) {
	int nrAttributes;

    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    return nrAttributes;
}

bool windowShouldClose(void) {
	return windowClose;
}

void clearColor(vec4 color) {
	glClearColor(color.x, color.y, color.z, color.w);
}

void clear(int bufferBit) {
	glClear(bufferBit);
}

void swapBuffers(SDL_Window* window) {
	SDL_GL_SwapWindow(window);
}

void processEvent(const SDL_Event& event) {
	switch (event.type) {
		case SDL_EVENT_QUIT:
			windowClose = true;
			break;
				
		case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			glViewport(0, 0, event.window.data1, event.window.data2);
			break;
				
		case SDL_EVENT_MOUSE_MOTION:
			mouseDeltaX += event.motion.xrel;
			mouseDeltaY += event.motion.yrel;
			break;
	}
}

void beginInputFrame(void) {
	mouseDeltaX = 0.0f;
	mouseDeltaY = 0.0f;
}

bool IsKeyPressed(int key) {
	const bool* keyboard = SDL_GetKeyboardState(nullptr);
	
	return keyboard[key];
}

void setMouseCapture(SDL_Window* window, bool enabled) {
	SDL_SetWindowRelativeMouseMode(window, enabled);
}

float getMouseDeltaX(void) {
	return mouseDeltaX;
}

float getMouseDeltaY(void) {
	return mouseDeltaY;
}

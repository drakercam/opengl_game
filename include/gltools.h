#ifndef GLTOOLS_H
#define GLTOOLS_H

#include <SDL3/SDL.h>
#include "glapi.h"
#include "glmath.h"

int loadSDL3();
void setVersionGL(int major, int minor);
SDL_Window* createWindow(int width, int height, const char* title);
void setContextCurrent(SDL_Window* window);
void enableGL(int settingGL);
void disableGL(int settingGL);
int maxNumberVertexAttrGL(void);
void clearColor(vec4 color);
void clear(int bufferBit);
void swapBuffers(SDL_Window* window);
void processEvent(const SDL_Event& event);
void beginInputFrame(void);
bool IsKeyPressed(int key);
void setMouseCapture(SDL_Window* window, bool enabled);
float getMouseDeltaX(void);
float getMouseDeltaY(void);

#endif

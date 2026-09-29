#ifndef GLAPI_H
#define GLAPI_H

#ifdef __EMSCRIPTEN__

	#include <GLES3/gl3.h>

#else

#define GL_GLEXT_PROTOTYPES
	#include <GL/gl.h>
	#include <GL/glext.h>

#endif

#endif

#include "engine.h"

void engine::initialize() {
    setVersionGL(3, 3);

    window = createWindow(1366, 768, "untitled");
    if (!window){
        terminate();
        return;
    }

    setContextCurrent(window);

    enableGL(GL_DEPTH_TEST);
    enableGL(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    std::cout << "GL Vendor: " << vendor << std::endl;
    std::cout << "GL Renderer: " << renderer << std::endl;

    // initialize free type
    if (FT_Init_FreeType(&ft)) {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
    }

    setMouseCapture(window, true);
}

void engine::terminate() {
	if (ft) {
		FT_Done_FreeType(ft);
		ft = nullptr;
	}
    
    if (window) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}
}

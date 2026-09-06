#include "project.h"
#include "glbuffer.h"
#include "glprimitives.h"
#include "glshader.h"
#include "gltools.h"

void project::initResourcesGPU(project::dataGPU& gpuData) {

    std::vector<vertex> vertices{

        // Front face (+Z)
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {}, {}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}, {}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}, {}},

        // Back face (-Z)
        {{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {}, {}},
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}, {}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
        {{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}, {}},

        // Left face (-X)
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {}, {}},
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}, {}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}, {}},
        // Right face (+X)
        {{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {}, {}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}, {}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
        {{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}, {}},

        // Top face (+Y)
        {{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {}, {}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}, {}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
        {{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}, {}},

        // Bottom face (-Y)
        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}, {}, {}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}, {}, {}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {}, {}},
        {{-0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}, {}, {}}
    };

    std::vector<unsigned int> indices = {

        // Front
        0, 1, 2,
        2, 3, 0,

        // Back
        4, 5, 6,
        6, 7, 4,

        // Left
        8, 9, 10,
        10, 11, 8,

        // Right
        12, 13, 14,
        14, 15, 12,

        // Top
        16, 17, 18,
        18, 19, 16,

        // Bottom
        20, 21, 22,
        22, 23, 20
    };

    buffer::emplace(gpuData.textures, "../resources/GigaChad.jpg");
    buffer::emplace(gpuData.textures, "../resources/grainy.jpg");
    std::vector<GLuint> textureRefs = { 0, 1 };

    buffer::emplace(gpuData.meshes, vertices, indices, textureRefs);
    buffer::emplace(gpuData.shaders, file::read("../shaders/basic_vertex1.glsl").c_str(), file::read("../shaders/basic_frag1.glsl").c_str());

    shader::bind(buffer::get(gpuData.shaders, 0));
    shader::intLoad(shader::getUniformLocation(buffer::get(gpuData.shaders, 0), "tex1"), 0);
    shader::intLoad(shader::getUniformLocation(buffer::get(gpuData.shaders, 0), "tex2"), 1);
    shader::unbind();
}

int project::initResourcesCPU(project::dataCPU& cpuData) {

    if (!gltools::loadGLFW()) {
        return -1;
    }

    gltools::setVersion(3, 3);

    cpuData.window = gltools::createWindow(1280, 720, "project");
    if (!cpuData.window){

        std::cout << "Failed to create glfw window" << std::endl;
        return -1;
    }

    gltools::setContextCurrent(cpuData.window);

    if (!gltools::loadGlad()) {
        std::cout << "Couldn't load opengl" << std::endl;
        project::terminate();
        return -1;
    }

    gltools::enable(GL_DEPTH_TEST);

    // glfwSetCursorPosCallback(cpuData.window, gltools::mouseCallback);

    return 0;
}

void project::update(project::dataCPU& cpuData, gltime::data& time) {

    return;
}

void project::loop(project::dataCPU& cpuData, project::dataGPU& gpuData) {

    vec3 cameraPosition{0.0f, 0.0f, 3.0f};
    vec3 cameraTarget{0.0f, 0.0f, 0.0f};
    vec3 up{0.0f, 1.0f, 0.0f};
    vec3 cameraDirection{vec3::normalize(cameraPosition - cameraTarget)};

    camera::data cam{cameraPosition, cameraTarget, cameraDirection, {0.0f, 0.0f, -1.0f}, vec3::normalize(vec3::cross(up, cameraDirection)), up};
    gltools::setInputMode(cpuData.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    mat4 modelMatrix, rotation, translation, scale;
    mat4 viewMatrix, projectionMatrix;

    mat4::identity(projectionMatrix);
    mat4::getProjection(projectionMatrix, ops::degreesToRadians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

    cube::data cube{{0, 1}};
    triangle::data triangle{{0, 1}};
    rectangle::data rectangle{{0, 1}};

    gltime::data t;
    while (gltools::windowShouldClose(cpuData.window)){

        gltime::update(t);

        gltools::clearColor({0.5f, 0.0f, 0.7f, 1.0f});
        gltools::clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera::input(cpuData.window, cam, t);
        camera::inputMouse(cpuData.window, cam);
        camera::update(viewMatrix, cam, t);

        shader::bind(buffer::get(gpuData.shaders, 0));

        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 6; ++j) {
                mat4::identity(modelMatrix);
                mat4::translate(modelMatrix, {float(i * 0.5f - 1.25f), 0.0f, j * 0.5f});
                mat4::rotate(modelMatrix, ops::degreesToRadians(glfwGetTime()*45.0f), {0.0f, 1.0f, 0.0f});
                mat4::scale(modelMatrix, {0.5f, 0.5f, 0.5f});

                shader::mat4Load(shader::getUniformLocation(buffer::get(gpuData.shaders, 0), "model"), modelMatrix);
                shader::mat4Load(shader::getUniformLocation(buffer::get(gpuData.shaders, 0), "view"), viewMatrix);
                shader::mat4Load(shader::getUniformLocation(buffer::get(gpuData.shaders, 0), "projection"), projectionMatrix);

                mesh::draw(buffer::get(gpuData.meshes, 0), buffer::get(gpuData.shaders, 0), gpuData.textures);
            }
        }

        mat4::identity(modelMatrix);
        mat4::translate(modelMatrix, {0.0f, 0.0f, -10.0f});
        mat4::rotate(modelMatrix, ops::degreesToRadians(glfwGetTime()*45.0f), {0.0f, 1.0f, 0.0f});
        mat4::scale(modelMatrix, {0.5f, 0.5f, 0.5f});

        shader::mat4Load(shader::getUniformLocation(buffer::get(gpuData.shaders, 0), "model"), modelMatrix);
        shader::mat4Load(shader::getUniformLocation(buffer::get(gpuData.shaders, 0), "view"), viewMatrix);
        shader::mat4Load(shader::getUniformLocation(buffer::get(gpuData.shaders, 0), "projection"), projectionMatrix);

        cube::draw(cube, buffer::get(gpuData.shaders, 0), gpuData.textures);

        mat4::identity(modelMatrix);
        mat4::translate(modelMatrix, {1.0f, 0.0f, -10.0f});
        mat4::rotate(modelMatrix, ops::degreesToRadians(glfwGetTime()*45.0f), {0.0f, 1.0f, 0.0f});
        mat4::scale(modelMatrix, {0.5f, 0.5f, 0.5f});

        shader::mat4Load(shader::getUniformLocation(buffer::get(gpuData.shaders, 0), "model"), modelMatrix);

        triangle::draw(triangle, buffer::get(gpuData.shaders, 0), gpuData.textures);

        mat4::identity(modelMatrix);
        mat4::translate(modelMatrix, {-1.0f, 0.0f, -10.0f});
        mat4::rotate(modelMatrix, ops::degreesToRadians(glfwGetTime()*45.0f), {0.0f, 1.0f, 0.0f});
        mat4::scale(modelMatrix, {0.5f, 0.5f, 0.5f});

        shader::mat4Load(shader::getUniformLocation(buffer::get(gpuData.shaders, 0), "model"), modelMatrix);

        rectangle::draw(rectangle, buffer::get(gpuData.shaders, 0), gpuData.textures);

        shader::unbind();

        gltools::swapBuffers(cpuData.window);
        gltools::pollEvents();
    }
}

void project::terminate() {

    glfwTerminate();
}

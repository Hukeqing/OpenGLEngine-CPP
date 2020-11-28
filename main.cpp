#include "src/stdopengl.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    float vertices[] = {
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f
    };
    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Window window;
    Filter filter(vertices, 24, {3, 3}, indices, 6);
    Renderer renderer(R"(D:\Code\C++\OpenGLEngine\src\main\shader\basic.vs)",
                      R"(D:\Code\C++\OpenGLEngine\src\main\shader\basic.fs)");
    Object obj{};
    obj.setFilter(&filter);
    obj.setRenderer(&renderer);
    window.addObj(&obj);
    window.run();
    return 0;
}
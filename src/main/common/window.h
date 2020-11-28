//
// Created by HKQ on 2020/11/27.
//

#ifndef OPENGL_ENGINE_WINDOW_H
#define OPENGL_ENGINE_WINDOW_H

#include "../define.h"
#include "object.h"

class Window {
private:
    int width, height;
    string title;
    GLFWwindow *window{};
    vector<Object *> objectList;

    bool init() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
        if (window == nullptr) {
            cerr << "Failed to create GLFW window" << endl;
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            cout << "Failed to initialize GLAD" << endl;
            return false;
        }
        glViewport(0, 0, width, height);
//        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        return true;
    }

    bool loop() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for (auto &obj : objectList) obj->render();

//            processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

public:
    Window() : width(800), height(600) {}

    Window(int w, int h, string &t) : width(w), height(h), title(t) {}

    bool run() {
        if (!init()) return false;
        while (!glfwWindowShouldClose(window)) loop();
        glfwTerminate();
        return true;
    }

    void addObj(Object *obj) {
        objectList.push_back(obj);
    }
};

#endif //OPENGL_ENGINE_WINDOW_H

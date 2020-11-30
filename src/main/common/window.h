//
// Created by HKQ on 2020/11/27.
//

#ifndef OPENGL_ENGINE_WINDOW_H
#define OPENGL_ENGINE_WINDOW_H

#include "../define.h"
#include "../component/direction_light.h"
#include "../component/point_light.h"
#include "../component/spot_light.h"

#include "object.h"
#include "camera.h"

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

class Window {
private:
    int width, height;
    string title;
    GLFWwindow *window{};


    vector<Object *> objectList;
    vector<DirectionLight *> directionLights;
    vector<PointLight *> pointLights;
    vector<SpotLight *> spotLights;
    Camera *camera{};

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
        glEnable(GL_BLEND);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        return true;
    }

    bool loop() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear((unsigned) GL_COLOR_BUFFER_BIT | (unsigned) GL_DEPTH_BUFFER_BIT);

        for (auto &obj : objectList)
            obj->render(camera->getView(), camera->getProjection(),
                        camera->transform.getPosition(),
                        directionLights, pointLights, spotLights);
        camera->getTransform().setRotation(0, (float) glfwGetTime() * 10 - 90, 0);
//            processInput(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

public:
    Window() : width(800), height(600) {}

    Window(int w, int h, string &t) : width(w), height(h), title(t) {}

    void setCamera(Camera *c) {
        camera = c;
    }

    bool run() {
        if (!init()) return false;

        glEnable(GL_DEPTH_TEST);

        while (!glfwWindowShouldClose(window)) loop();
        std::cout << glGetError() << std::endl;
        glfwTerminate();
        return true;
    }

    void addObj(Object *obj) {
        objectList.push_back(obj);
    }

    void addDirLight(DirectionLight *light) { directionLights.push_back(light); }

    void addPointLight(PointLight *light) { pointLights.push_back(light); }

    void addSpotLight(SpotLight *light) { spotLights.push_back(light); }
};

#endif //OPENGL_ENGINE_WINDOW_H

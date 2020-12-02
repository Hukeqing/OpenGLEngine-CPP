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

void inputKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

void inputScrollCallback(GLFWwindow *window, double xOffset, double yOffset);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

class Window {
    friend void inputKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

    friend void inputScrollCallback(GLFWwindow *window, double xOffset, double yOffset);

    friend void framebuffer_size_callback(GLFWwindow *window, int width, int height);

//public:
private:
    static bool glfwInitComplete;
    static unordered_map<GLFWwindow *, Window *> winId2winClass;
    int width, height;
    string title;
    GLFWwindow *window{};

    vector<Object *> objectList;
    vector<DirectionLight *> directionLights;
    vector<PointLight *> pointLights;
    vector<SpotLight *> spotLights;
    Camera *camera{};

    function<void()> startFunc = nullptr, updateFunc = nullptr;
    function<void(int, int)> onWindowResize = nullptr, onMouseScroll = nullptr;
    function<void(int, bool)> onKeyEvent = nullptr;
    float lastTime{}, deltaTime{};

    static void initGL() {
        if (glfwInitComplete) return;
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        glfwInitComplete = true;
    }

    bool init() {
        initGL();
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
        return true;
    }

    bool loop() {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear((unsigned) GL_COLOR_BUFFER_BIT | (unsigned) GL_DEPTH_BUFFER_BIT);

        for (auto &obj : objectList)
            obj->render(camera->getView(),
                        camera->getProjection(),
                        camera->transform.getPosition(),
                        directionLights,
                        pointLights,
                        spotLights);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

public:
    Window() : width(800), height(600) {}

    Window(int w, int h, string &t) : width(w), height(h), title(t) {}

    void setCamera(Camera *c) { camera = c; }

    bool run() {
        if (!init()) return false;
        winId2winClass.insert({window, this});

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_FRAMEBUFFER_SRGB);
        glEnable(GL_BLEND);

        glViewport(0, 0, width, height);

        glfwSetKeyCallback(window, inputKeyCallback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        if (startFunc != nullptr) startFunc();

        lastTime = (float) glfwGetTime();

        while (!glfwWindowShouldClose(window)) {
            auto tmp = (float) glfwGetTime();
            deltaTime = tmp - lastTime;
            lastTime = tmp;
            if (updateFunc != nullptr) updateFunc();
            loop();
        }

        cout << glGetError() << endl;
        glfwTerminate();
        return true;
    }

    void close() {
        glfwSetWindowShouldClose(window, true);
    }

    /*-----add thing into window-----*/
    void addObj(Object *obj) { objectList.push_back(obj); }

    void addDirLight(DirectionLight *light) { directionLights.push_back(light); }

    void addPointLight(PointLight *light) { pointLights.push_back(light); }

    void addSpotLight(SpotLight *light) { spotLights.push_back(light); }

    /*-----set runtime behave-----*/
    void setStart(function<void()> &f) { startFunc = f; }

    void setUpdate(function<void()> &f) { updateFunc = f; }

    void setOnWindowResize(function<void(int, int)> &f) { onWindowResize = f; }

    void setOnKeyEvent(const function<void(int, bool)> &f) { onKeyEvent = f; }
};

unordered_map<GLFWwindow *, Window *> Window::winId2winClass;
bool Window::glfwInitComplete = false;

void inputKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    auto winClass = Window::winId2winClass.find(window)->second;
    if (winClass->onKeyEvent != nullptr) winClass->onKeyEvent(key, action);
}

void inputScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    auto winClass = Window::winId2winClass.find(window)->second;
    if (winClass->onMouseScroll != nullptr) winClass->onMouseScroll(xOffset, yOffset);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    auto winClass = Window::winId2winClass.find(window)->second;
    if (winClass->onWindowResize != nullptr) winClass->onWindowResize(width, height);
}

#endif //OPENGL_ENGINE_WINDOW_H

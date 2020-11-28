//
// Created by HKQ on 2020/11/28.
//

#ifndef OPENGL_ENGINE_CAMERA_H
#define OPENGL_ENGINE_CAMERA_H

#include "../define.h"
#include "../component/transform.h"

class Camera {
    friend Window;
private:
    float left{}, right{}, top{}, bottom{}, nearPlane{}, farPlane{}, fov{};
    bool useOrthographic = false;
    Transform transform;

    glm::mat4 getView() {
        return glm::lookAt(transform.getPosition(),
                           transform.getPosition() + transform.getForward(),
                           transform.getUp());
    }

    glm::mat4 getProjection() const {
        return useOrthographic ?
               glm::ortho(left, right, bottom, top, nearPlane, farPlane) :
               glm::perspective(glm::radians(fov), (right - left) / (top - bottom), nearPlane, farPlane);
    }

public:
    Camera() = default;

    explicit Camera(bool flag) : useOrthographic(flag) {}

    Camera(float l, float r, float t, float b, float n = 0.1f, float f = 1000.0f) :
            left(l), right(r), top(t), bottom(b), nearPlane(n), farPlane(f), useOrthographic(true) {}

    Camera(float w, float h, float fv, float n = 0.1f, float f = 1000.0f) :
            left(0), right(w), top(0), bottom(h), nearPlane(n), farPlane(f), fov(fv), useOrthographic(false) {}

    Transform &getTransform() { return transform; }
};

#endif //OPENGL_ENGINE_CAMERA_H

//
// Created by HKQ on 2020/11/28.
//

#ifndef OPENGL_ENGINE_TRANSFORM_H
#define OPENGL_ENGINE_TRANSFORM_H

#include "../define.h"

class Transform {
    friend Object;
    vector3 position{}, rotation{}, scale{}, forward{}, up{}, left{};

    void update() {
        forward = vector3(0);
        forward.x = -glm::sin(glm::radians(rotation.y));
        forward.y = glm::sin(glm::radians(rotation.x));
        forward.z = glm::cos(glm::radians(rotation.x)) * glm::cos(glm::radians(rotation.y));
        forward = glm::normalize(forward);
        left = glm::normalize(glm::cross(vector3(0, 1, 0), forward));
        up = glm::normalize(glm::cross(forward, left));
    }

    glm::mat4 getModel() const {
        glm::mat4 model(1);
        model = glm::translate(model, position);
        model = glm::rotate(model, rotation.x, vector3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotation.y, vector3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.z, vector3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, scale);
        return model;
    }

public:
    Transform() {
        update();
    }

    void setPosition(float x, float y, float z) {
        position.x = x;
        position.y = y;
        position.z = z;
    }

    void setRotation(float x, float y, float z) {
        rotation.x = x;
        rotation.y = y;
        rotation.z = z;
        update();
    }

    void setScale(float x, float y, float z) {
        scale.x = x;
        scale.y = y;
        scale.z = z;
    }

    vector3 getPosition() const { return position; }

    vector3 getRotation() const { return rotation; }

    vector3 getScale() const { return scale; }

    vector3 getForward() const { return forward; }

    vector3 getUp() const { return up; }

    vector3 getLeft() const { return left; }
};

#endif //OPENGL_ENGINE_TRANSFORM_H

//
// Created by HKQ on 2020/11/28.
//

#ifndef OPENGL_ENGINE_TRANSFORM_H
#define OPENGL_ENGINE_TRANSFORM_H

#include "../define.h"

class Transform {
    friend Object;
    glm::vec3 position{}, rotation{}, scale{}, forward{}, up{}, left{};

    void update() {
        forward = glm::vec3(0);
        forward.x = -glm::sin(glm::radians(rotation.y));
        forward.y = glm::sin(glm::radians(rotation.x));
        forward.z = glm::cos(glm::radians(rotation.x)) * glm::cos(glm::radians(rotation.y));
        forward = glm::normalize(forward);
        left = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
        up = glm::normalize(glm::cross(forward, left));
    }

    glm::mat4 getModel() const {
        glm::mat4 model(1);
        model = glm::scale(model, scale);
        model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, position);
        return model;
    }

public:
    Transform() {
        update();
    }

    void setPosition(int x, int y, int z) {
        position.x = x;
        position.y = y;
        position.z = z;
    }

    void setRotation(int x, int y, int z) {
        rotation.x = x;
        rotation.y = y;
        rotation.z = z;
        update();
    }

    void setScale(int x, int y, int z) {
        scale.x = x;
        scale.y = y;
        scale.z = z;
    }

    glm::vec3 getPosition() const { return position; }

    glm::vec3 getRotation() const { return rotation; }

    glm::vec3 getScale() const { return scale; }

    glm::vec3 getForward() const { return forward; }

    glm::vec3 getUp() const { return up; }

    glm::vec3 getLeft() const { return left; }
};

#endif //OPENGL_ENGINE_TRANSFORM_H

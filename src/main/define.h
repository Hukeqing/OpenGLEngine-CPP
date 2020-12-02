//
// Created by HKQ on 2020/11/28.
//

#ifndef OPENGL_ENGINE_DEFINE_H
#define OPENGL_ENGINE_DEFINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <bits/stdc++.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Window;

class Filter;

class Renderer;

class Object;

class Camera;

class Transform;

class Light;

class DirectionLight;

class PointLight;

class SpotLight;

class Material;

/**
 * typedef name
 */

typedef glm::vec3 vector3;
typedef glm::vec2 vector2;

/**
 * enum
 */
enum KeyType {
    Default = 0,
    Press = 1,
    On = 2,
    Release = 3
};
#endif //OPENGL_ENGINE_DEFINE_H

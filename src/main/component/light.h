//
// Created by HKQ on 2020/11/28.
//

#ifndef OPENGL_ENGINE_LIGHT_H
#define OPENGL_ENGINE_LIGHT_H

#include "../define.h"

class Light {
public:
    vector3 color;
    float ambient, diffuse, specular;

    Light() = default;

    Light(float a, float d, float s, const vector3 c) : ambient(a), diffuse(d), specular(s), color(c) {}
};

#endif //OPENGL_ENGINE_LIGHT_H

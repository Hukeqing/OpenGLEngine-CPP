//
// Created by HKQ on 2020/11/28.
//

#ifndef OPENGL_ENGINE_DIRECTION_LIGHT_H
#define OPENGL_ENGINE_DIRECTION_LIGHT_H

#include "../define.h"
#include "light.h"

class DirectionLight : public Light {
public:
    vector3 direction;

    DirectionLight() = default;

    DirectionLight(float a, float d, float s, const vector3 c, const vector3 &dir) : Light(a, d, s, c),
                                                                                     direction(dir) {}
};

#endif //OPENGL_ENGINE_DIRECTION_LIGHT_H

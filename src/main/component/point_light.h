//
// Created by HKQ on 2020/11/28.
//

#ifndef OPENGL_ENGINE_POINT_LIGHT_H
#define OPENGL_ENGINE_POINT_LIGHT_H

#include "../define.h"
#include "light.h"

class PointLight : public Light {
public:
    vector3 position;
    float constant, linear, quadratic;

    PointLight() = default;

    PointLight(float a, float d, float s, const vector3 c,
               const vector3 &pos, float con, float lin, float qua) : Light(a, d, s, c), position(pos), constant(con),
                                                                      linear(lin), quadratic(qua) {}
};

#endif //OPENGL_ENGINE_POINT_LIGHT_H

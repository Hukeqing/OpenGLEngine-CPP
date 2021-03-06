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
};

#endif //OPENGL_ENGINE_POINT_LIGHT_H

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
};

#endif //OPENGL_ENGINE_DIRECTION_LIGHT_H

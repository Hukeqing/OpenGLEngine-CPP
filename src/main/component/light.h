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
};

#endif //OPENGL_ENGINE_LIGHT_H

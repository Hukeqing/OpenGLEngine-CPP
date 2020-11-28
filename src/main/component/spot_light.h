//
// Created by HKQ on 2020/11/28.
//

#ifndef OPENGL_ENGINE_SPOT_LIGHT_H
#define OPENGL_ENGINE_SPOT_LIGHT_H

#include "../define.h"
#include "light.h"

class SpotLight : public Light {
public:
    glm::vec3 position, direction;
    float cutOff, outerCutOff;
    float constant, linear, quadratic;
};


#endif //OPENGL_ENGINE_SPOT_LIGHT_H

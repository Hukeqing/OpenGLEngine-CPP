//
// Created by HKQ on 2020/11/28.
//

#ifndef OPENGL_ENGINE_SPOT_LIGHT_H
#define OPENGL_ENGINE_SPOT_LIGHT_H

#include "../define.h"
#include "light.h"

class SpotLight : public Light {
public:
    vector3 position, direction;
    float cutOff, outerCutOff;
    float constant, linear, quadratic;

    SpotLight() = default;

    SpotLight(float a, float d, float s, const vector3 c,
              const vector3 &pos, const vector3 &dir,
              float co, float oco, float con, float lin, float qua) : Light(a, d, s, c), position(pos), direction(dir),
                                                                      cutOff(co), outerCutOff(oco),
                                                                      constant(con), linear(lin), quadratic(qua) {}
};


#endif //OPENGL_ENGINE_SPOT_LIGHT_H

//
// Created by HKQ on 2020/11/27.
//

#ifndef OPENGL_ENGINE_OBJECT_H
#define OPENGL_ENGINE_OBJECT_H

#include "../define.h"
#include "../component/filter.h"
#include "../component/renderer.h"
#include "../component/transform.h"

class Object {
    friend Window;
private:
    bool complete = false;
    Filter *filter{};
    Renderer *renderer{};
    Transform *transform{};

    void render(const glm::mat4 &view, const glm::mat4 &projection,
                const glm::vec3 viewPos,
                const vector<DirectionLight *> &directionLights,
                const vector<PointLight *> &pointLights,
                const vector<SpotLight *> &spotLights) {
        if (!complete) if (!init()) exit(-1);
        renderer->use(transform->getModel(), view, projection, viewPos, directionLights, pointLights, spotLights);
        filter->draw();
//        renderer->unUse();
    }

    bool init() {
        if (filter == nullptr || renderer == nullptr || transform == nullptr) return false;
        if (!renderer->init()) return false;
        filter->build();
        complete = true;
        return true;
    }

public:
    Object() = default;

    void setFilter(Filter *f) { filter = f; }

    void setRenderer(Renderer *r) { renderer = r; }

    void setTransform(Transform *t) { transform = t; }

    Transform &getTransform() { return *transform; }
};

#endif //OPENGL_ENGINE_OBJECT_H

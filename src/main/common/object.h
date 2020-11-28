//
// Created by HKQ on 2020/11/27.
//

#ifndef OPENGL_ENGINE_OBJECT_H
#define OPENGL_ENGINE_OBJECT_H

#include "../define.h"
#include "../component/filter.h"
#include "../component/renderer.h"

class Object {
    friend Window;
private:
    bool complete = false;
    Filter *filter{};
    Renderer *renderer{};

    void render() {
        if (!complete) if (!init()) exit(-1);
        renderer->use();
        filter->draw();
    }

    bool init() {
        if (!renderer->init()) return false;
        filter->build();
        complete = true;
        return true;
    }

public:
    Object() = default;

    void setFilter(Filter *f) {
        filter = f;
    }

    void setRenderer(Renderer *r) {
        renderer = r;
    }
};

#endif //OPENGL_ENGINE_OBJECT_H

//
// Created by HKQ on 2020/12/7.
//

#ifndef OPENGL_ENGINE_CREATE_H
#define OPENGL_ENGINE_CREATE_H

#include "../define.h"
#include "../component/mesh.h"
#include "../component/filter.h"
#include "../component/material.h"
#include "../component/renderer.h"
#include "../component/transform.h"
#include "../common/object.h"

namespace Create {
    // NOLINTNEXTLINE
    static const vector<float> vertices = {
            -0.5f, -0.5f, -0.5f, +0.5f, +0.5f, -0.5f, +0.5f, -0.5f, -0.5f, +0.5f, +0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
            -0.5f, +0.5f, -0.5f, -0.5f, -0.5f, +0.5f, +0.5f, -0.5f, +0.5f, +0.5f, +0.5f, +0.5f, +0.5f, +0.5f, +0.5f,
            -0.5f, +0.5f, +0.5f, -0.5f, -0.5f, +0.5f, -0.5f, +0.5f, +0.5f, -0.5f, +0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, +0.5f, -0.5f, +0.5f, +0.5f, +0.5f, +0.5f, +0.5f, +0.5f, -0.5f, -0.5f,
            +0.5f, +0.5f, -0.5f, +0.5f, -0.5f, -0.5f, +0.5f, +0.5f, +0.5f, +0.5f, -0.5f, +0.5f, -0.5f, -0.5f, -0.5f,
            +0.5f, -0.5f, -0.5f, +0.5f, -0.5f, +0.5f, +0.5f, -0.5f, +0.5f, -0.5f, -0.5f, +0.5f, -0.5f, -0.5f, -0.5f,
            -0.5f, +0.5f, -0.5f, +0.5f, +0.5f, +0.5f, +0.5f, +0.5f, -0.5f, +0.5f, +0.5f, +0.5f, -0.5f, +0.5f, -0.5f,
            -0.5f, +0.5f, +0.5f
    };
    // NOLINTNEXTLINE
    static const vector<float> texcoords = {
            +0.0f, +0.0f, +1.0f, +1.0f, +1.0f, +0.0f, +1.0f, +1.0f, +0.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f,
            +0.0f, +1.0f, +1.0f, +1.0f, +1.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +1.0f, +1.0f, +0.0f, +1.0f,
            +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +1.0f, +1.0f, +0.0f, +1.0f, +1.0f,
            +0.0f, +0.0f, +0.0f, +0.0f, +1.0f, +1.0f, +1.0f, +1.0f, +0.0f, +1.0f, +0.0f, +0.0f, +0.0f, +0.0f, +1.0f,
            +0.0f, +1.0f, +1.0f, +0.0f, +1.0f, +1.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f
    };
    // NOLINTNEXTLINE
    static auto cubeMesh = new Mesh(vertices, texcoords);
    // NOLINTNEXTLINE
    static auto filter = new Filter(cubeMesh);

    Object createCube() {
        auto cubeRenderer = new Renderer("src/main/shader/model/model.vs",
                                         "src/main/shader/model/model.fs");
        auto material = new Material();
        auto transform = new Transform();
        Object res;
        cubeRenderer->setMaterial(material);
        res.setFilter(filter);
        res.setRenderer(cubeRenderer);
        res.setTransform(transform);
        return res;
    }

    Object createSkybox(const string &rightImage, const string &leftImage,
                        const string &topImage, const string &bottomImage,
                        const string &backImage, const string &frontImage) {
        auto skyboxRenderer = new Renderer("src/main/shader/skybox/skybox.vs",
                                           "src/main/shader/skybox/skybox.fs");
        skyboxRenderer->setRendererMode(OverlapMode);
        auto material = new Material();
        material->setFlip(false);
        material->setDiffuse(rightImage, leftImage, topImage, bottomImage, backImage, frontImage);
        auto transform = new Transform();
        Object res;
        skyboxRenderer->setMaterial(material);
        res.setFilter(filter);
        res.setRenderer(skyboxRenderer);
        res.setTransform(transform);
        return res;
    }
}


#endif //OPENGL_ENGINE_CREATE_H

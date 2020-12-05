//
// Created by HKQ on 2020/11/27.
//

#ifndef OPENGL_ENGINE_FILTER_H
#define OPENGL_ENGINE_FILTER_H

#include "../define.h"
#include "mesh.h"

class Filter {
    friend Object;
private:
    bool complete = false;
    Mesh *mesh;
    unsigned int VBO, VAO;

    void build() {
        if (complete) return;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        mesh->build();
        mesh->bind();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        complete = true;
    }

    void draw() const {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh->vertices.size());
        glBindVertexArray(0);
    }

public:
    Filter(Mesh *m) { mesh = m; }
};

#endif //OPENGL_ENGINE_FILTER_H

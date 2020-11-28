//
// Created by HKQ on 2020/11/27.
//

#ifndef OPENGL_ENGINE_FILTER_H
#define OPENGL_ENGINE_FILTER_H

#include "../define.h"

class Filter {
    friend Object;
private:
    bool complete = false;
    float *vertices{};
    int verticesLen, stride{};
    vector<int> size;
    unsigned int *indices{};
    int indicesLen{};
    unsigned int VBO{}, VAO{}, EBO{};

    void build() {
        if (complete) return;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verticesLen, vertices, GL_STATIC_DRAW);

        if (indices != nullptr) {
            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indicesLen, indices, GL_STATIC_DRAW);
        }

        int offset = 0;
        stride = 0;
        for (auto &item : size) stride += item;
        for (int i = 0; i < size.size(); ++i) {
            glVertexAttribPointer(i, size[i], GL_FLOAT, GL_FALSE, stride * sizeof(float),
                                  (void *) (offset * sizeof(float)));
            glEnableVertexAttribArray(i);
            offset += size[i];
        }

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        complete = true;
    }

    void draw() const {
        glBindVertexArray(VAO);
        if (indices == nullptr)
            glDrawArrays(GL_TRIANGLES, 0, indicesLen / stride);
        else
            glDrawElements(GL_TRIANGLES, indicesLen, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

public:
    Filter() = default;

    explicit Filter(float *v,
                    int len,
                    vector<int> s,
                    unsigned int *i = nullptr,
                    int indLen = 0)
            : vertices(v), verticesLen(len), size(s), indices(i), indicesLen(indLen) {}
};

#endif //OPENGL_ENGINE_FILTER_H

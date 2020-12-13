//
// Created by HKQ on 2020/12/2.
//

#ifndef OPENGL_ENGINE_MESH_H
#define OPENGL_ENGINE_MESH_H

#include "../define.h"

class Mesh {
    friend Filter;
    struct Vertex {
        vector3 position;
        vector2 texcoords;
    };

    bool complete = false;
    vector<Vertex> vertices;
    float *buffer{};
    string rendererParam;
    int bufferSize, bufferUnit;

    void build() {
        if (complete) return;
        int face = (int) vertices.size() / 3;
        bufferUnit = 0;
        for (auto &item : rendererParam) {
            switch (item) {
                case 'v':
                case 'n':
                    bufferUnit += 3;
                    break;
                case 't':
                    bufferUnit += 2;
                    break;
                case 'b':
                    bufferUnit += 6;
                    break;
            }
        }
        bufferSize = (int) vertices.size() * bufferUnit;
        buffer = new float[vertices.size() * bufferUnit];
        for (int i = 0; i < face; ++i) {
            vector3 deltaPos1 = vertices[i * 3 + 1].position - vertices[i * 3].position;
            vector3 deltaPos2 = vertices[i * 3 + 2].position - vertices[i * 3 + 1].position;
            vector2 deltaUV1 = vertices[i * 3 + 1].texcoords - vertices[i * 3].texcoords;
            vector2 deltaUV2 = vertices[i * 3 + 2].texcoords - vertices[i * 3 + 1].texcoords;

            float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
            vector3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
            vector3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;
            vector3 normal = glm::normalize(glm::cross(deltaPos1, deltaPos2));
            for (int j = 0; j < 3; ++j) {
                int base = i * bufferUnit * 3 + j * bufferUnit;
                bufferUnit = 0;
                auto push3 = [&](int b, const vector3 &data) {
                    buffer[b + 0] = data.x;
                    buffer[b + 1] = data.y;
                    buffer[b + 2] = data.z;
                    bufferUnit += 3;
                };
                auto push2 = [&](int b, const vector2 &data) {
                    buffer[b + 0] = data.x;
                    buffer[b + 1] = data.y;
                    bufferUnit += 2;
                };
                for (auto &item : rendererParam) {
                    switch (item) {
                        case 'v':
                            push3(base + bufferUnit, vertices[i * 3 + j].position);
                            break;
                        case 'n':
                            push3(base + 3, normal);
                            break;
                        case 't':
                            push2(base + 6, vertices[i * 3 + j].texcoords);
                            break;
                        case 'b':
                            push3(base + 8, tangent);
                            push3(base + 11, bitangent);
                            break;
                    }
                }
            }
        }
        vertices.clear();
        complete = true;
    }

    void bind() {
        glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float), buffer, GL_STATIC_DRAW);
        int count = 0;
        for (int i = 0; i < rendererParam.size(); ++i) {
            int len = rendererParam[i] == 't' ? 2 : 3;
            glVertexAttribPointer(i, len, GL_FLOAT, GL_FALSE, bufferUnit * sizeof(float), (void *) (count * sizeof(float)));
            glEnableVertexAttribArray(i);
            count += len;
        }
    }

public:
    Mesh(const vector<float> &v, const vector<float> &t, const string &rp = "vntb") : rendererParam(rp) {
        assert(v.size() % 9 == 0);
        assert(t.size() % 6 == 0);
        assert(v.size() / 9 == t.size() / 6);
        int n = (int) v.size() / 3;
        vertices.resize(n);
        for (int i = 0; i < n; ++i) {
            vertices[i].position = vector3(v[i * 3], v[i * 3 + 1], v[i * 3 + 2]);
            vertices[i].texcoords = vector2(t[i * 2], t[i * 2 + 1]);
        }
    }

    Mesh(const vector<float> &v, const vector<float> &t, const vector<int> &i, const string &rp = "vntb")
            : rendererParam(rp) {
        assert(i.size() % 3 == 0);
        assert(v.size() / 3 == t.size() / 2);
        for (int index = 0; index < i.size(); ++index) {
            vertices[index].position = vector3(v[i[index] * 3], v[i[index] * 3 + 1], v[i[index] * 3 + 2]);
            vertices[index].texcoords = vector2(t[i[index] * 2], t[i[index] * 2 + 1]);
        }
    }
};

#endif //OPENGL_ENGINE_MESH_H

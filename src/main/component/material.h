//
// Created by HKQ on 2020/11/28.
//

#ifndef OPENGL_ENGINE_MATERIAL_H
#define OPENGL_ENGINE_MATERIAL_H

#include "../define.h"

#define STB_IMAGE_IMPLEMENTATION

#include "../stb/stb_image.h"

class Material {
    friend Renderer;
    bool complete = false;
    unsigned shaderProgram;
    vector<pair<string, float>> textures;
    vector<unsigned int> textureId;

    static unsigned loadImage(const char *filePath) {
        unsigned texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, nrChannels;
        unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, nrChannels == 3 ? GL_RGB : GL_RGBA, width, height, 0,
                         nrChannels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            cerr << "Failed to load texture" << endl;
            exit(-1);
        }
        stbi_image_free(data);
        return texture;
    }

    void init(unsigned sp) {
        shaderProgram = sp;
        textureId.resize(textures.size());
        stbi_set_flip_vertically_on_load(true);
        glUseProgram(shaderProgram);
        for (int i = 0; i < textures.size(); ++i) {
            textureId[i] = loadImage(textures[i].first.data());
            glUniform1i(glGetUniformLocation(shaderProgram, ("texture" + to_string(i)).data()), i);
        }
        glUseProgram(0);
        complete = true;
    }

    void use() {
        for (int i = 0; i < textureId.size(); ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, textureId[i]);
        }
    }

public:
    void setTexture(const vector<pair<string, float>> &t) {
        textures = t;
        if (!complete) return;
        textureId.resize(t.size());
        glUseProgram(shaderProgram);
        for (int i = 0; i < textures.size(); ++i) {
            textureId[i] = loadImage(textures[i].first.data());
            glUniform1i(glGetUniformLocation(shaderProgram, ("texture" + to_string(i)).data()), i);
        }
        glUseProgram(0);
    }

    void changeTexture(const pair<string, float> &t, int id) {
        textures[id] = t;
        if (!complete) return;
        textureId[id] = loadImage(t.first.data());
    }

};

#endif //OPENGL_ENGINE_MATERIAL_H

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

    glm::vec3 color;
    string diffuse, specular;
    unsigned diffuseId, specularId;
    float shininess;

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
        if (complete) return;
        shaderProgram = sp;
        stbi_set_flip_vertically_on_load(true);
        glUseProgram(shaderProgram);
        if (!diffuse.empty()) diffuseId = loadImage(diffuse.data());
        if (!specular.empty()) specularId = loadImage(specular.data());
        glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);
        glUseProgram(0);
        complete = true;
    }

    void use() {
        int flag = 0;
        if (!diffuse.empty()) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseId);
            flag |= 1;
        }
        if (!specular.empty()) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specularId);
            flag |= 2;
        }
        glUseProgram(shaderProgram);
        glUniform3f(glGetUniformLocation(shaderProgram, "material.color"), color.r, color.g, color.b);
        glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), shininess);
        glUniform1i(glGetUniformLocation(shaderProgram, "material.flag"), flag);
        glUseProgram(0);
    }

public:
    void setDiffuse(const string &t) {
        diffuse = t;
        if (!complete) return;
        glUseProgram(shaderProgram);
        if (!diffuse.empty()) diffuseId = loadImage(diffuse.data());
        glUseProgram(0);
    }

    void setSpecular(const string &t) {
        specular = t;
        if (!complete) return;
        glUseProgram(shaderProgram);
        if (!specular.empty()) specularId = loadImage(specular.data());
        glUseProgram(0);
    }

    void setColor(float r, float g, float b) {
        color = glm::vec3(r, g, b);
    }

    void setShininess(float s) { shininess = s; }
};

#endif //OPENGL_ENGINE_MATERIAL_H

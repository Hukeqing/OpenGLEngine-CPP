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

    glm::vec3 color;
    vector<string> diffuse, specular;
    unsigned diffuseId, specularId;
    float shininess;
    bool flip;

    static GLenum imageFormat(int nrChannels) {
        if (nrChannels == 1) return GL_RED;
        else if (nrChannels == 3) return GL_RGB;
        else if (nrChannels == 4) return GL_RGBA;
        cerr << "Unknown image file type" << endl;
        exit(-1);
    }

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
            GLenum format = imageFormat(nrChannels);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            cerr << "Texture failed to load at path: " << *filePath << endl;
            stbi_image_free(data);
            exit(-1);
        }
        stbi_image_free(data);
        return texture;
    }

    static unsigned loadCubeImage(vector<string> &fileList) {
        unsigned texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        int width, height, nrChannels;
        for (unsigned int i = 0; i < fileList.size(); i++) {
            unsigned char *data = stbi_load(fileList[i].c_str(), &width, &height, &nrChannels, 0);
            if (data) {
                GLenum format = imageFormat(nrChannels);
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height,
                             0, format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            } else {
                cerr << "Cube map texture failed to load at path: " << fileList[i] << endl;
                stbi_image_free(data);
                exit(-1);
            }
        }
        return texture;
    }


    void init() {
        if (complete) return;
        stbi_set_flip_vertically_on_load(flip);
        if (diffuse.size() == 1) diffuseId = loadImage(diffuse.front().data());
        else if (diffuse.size() == 6) diffuseId = loadCubeImage(diffuse);
        if (specular.size() == 1) specularId = loadImage(specular.front().data());
        else if (specular.size() == 6) specularId = loadCubeImage(specular);
        complete = true;
    }

    void use(unsigned shaderProgram) {
        int flag = 0;
        if (!diffuse.empty()) {
            glActiveTexture(GL_TEXTURE0);
            if (diffuse.size() == 1) glBindTexture(GL_TEXTURE_2D, diffuseId);
            else if (diffuse.size() == 6) glBindTexture(GL_TEXTURE_CUBE_MAP, diffuseId);
            flag |= 1;
        }
        if (!specular.empty()) {
            glActiveTexture(GL_TEXTURE1);
            if (specular.size() == 1) glBindTexture(GL_TEXTURE_2D, specularId);
            else if (specular.size() == 6) glBindTexture(GL_TEXTURE_CUBE_MAP, specularId);
            flag |= 2;
        }
        glUniform3f(glGetUniformLocation(shaderProgram, "material.color"), color.r, color.g, color.b);
        glUniform1f(glGetUniformLocation(shaderProgram, "material.shininess"), shininess);
        glUniform1i(glGetUniformLocation(shaderProgram, "material.flag"), flag);
    }

public:
    void setDiffuse(const string &t) {
        diffuse.resize(1);
        diffuse[0] = t;
        if (!complete) return;
        diffuseId = loadImage(t.data());
    }

    void setDiffuse(const string &rightImage, const string &leftImage,
                    const string &topImage, const string &bottomImage,
                    const string &backImage, const string &frontImage) {
        diffuse.clear();
        diffuse = {rightImage, leftImage, topImage, bottomImage, backImage, frontImage};
        if (!complete) return;
        diffuseId = loadCubeImage(diffuse);
    }

    void setSpecular(const string &t) {
        specular.resize(1);
        specular[0] = t;
        if (!complete) return;
        if (!specular.empty()) specularId = loadImage(t.data());
    }

    void setSpecular(const string &rightImage, const string &leftImage,
                     const string &topImage, const string &bottomImage,
                     const string &backImage, const string &frontImage) {
        specular.clear();
        specular = {rightImage, leftImage, topImage, bottomImage, backImage, frontImage};
        if (!complete) return;
        specularId = loadCubeImage(specular);
    }

    void setColor(float r, float g, float b) {
        color = glm::vec3(r, g, b);
    }

    void setShininess(float s) { shininess = s; }

    void setFlip(bool flag = true) { flip = flag; }
};

#endif //OPENGL_ENGINE_MATERIAL_H

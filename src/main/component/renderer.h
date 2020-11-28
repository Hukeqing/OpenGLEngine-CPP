//
// Created by HKQ on 2020/11/27.
//

#ifndef OPENGL_ENGINE_RENDERER_H
#define OPENGL_ENGINE_RENDERER_H

#include "../define.h"

#define STB_IMAGE_IMPLEMENTATION

#include "../stb/stb_image.h"

class Renderer {
    friend Object;
private:
    bool complete = false;
    const char *vertexFile, *fragmentFile;
    int shaderProgram{};

    vector<pair<string, float>> textures;
    vector<unsigned int> textureId;

    void openShaderFile(const char *filePath, string &res) {
        res.clear();
        ifstream shaderFile;
        shaderFile.exceptions(ifstream::failbit | ifstream::badbit);
        try {
            shaderFile.open(filePath);
            stringstream shaderStream;
            shaderStream << shaderFile.rdbuf();
            shaderFile.close();
            res = shaderStream.str();
        } catch (ifstream::failure e) {
            cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
            exit(-1);
        }
    }

    int loadImage(const char *filePath) {
        unsigned int texture;
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

    bool init() {
        string vertexCode, fragmentCode;
        openShaderFile(vertexFile, vertexCode);
        openShaderFile(fragmentFile, fragmentCode);
        if (!compile(vertexCode.data(), fragmentCode.data())) return false;
        textureId.resize(textures.size());
        stbi_set_flip_vertically_on_load(true);
        glUseProgram(shaderProgram);
        for (int i = 0; i < textures.size(); ++i) {
            textureId[i] = loadImage(textures[i].first.data());
            glUniform1i(glGetUniformLocation(shaderProgram, ("texture" + to_string(i)).data()), i);
        }
        glUseProgram(0);
        return true;
    }

    bool compile(const char *vertexCode, const char *fragmentCode) {
        if (complete) return true;
        int success;
        char infoLog[512];
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexCode, nullptr);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
            return false;
        }
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentCode, nullptr);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
            return false;
        }
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
            cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
            return false;
        }
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        complete = true;
        return true;
    }

    void use() {
        if (complete || init()) {
            for (int i = 0; i < textureId.size(); ++i) {
                glActiveTexture(GL_TEXTURE0 + i);
                glBindTexture(GL_TEXTURE_2D, textureId[i]);
            }
            glUseProgram(shaderProgram);
        }
    }

public:
    Renderer() = default;

    Renderer(const char *v, const char *f) {
        vertexFile = v;
        fragmentFile = f;
    }

    Renderer(const string &v, const string &f) {
        vertexFile = v.data();
        fragmentFile = f.data();
    }

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

    void setUniform(const string &key, int *value, int len) {
        int location = glGetUniformLocation(shaderProgram, key.data());
        switch (len) {
            case 1:
                glUniform1i(location, value[0]);
                break;
            case 2:
                glUniform2i(location, value[0], value[1]);
                break;
            case 3:
                glUniform3i(location, value[0], value[1], value[2]);
                break;
            case 4:
                glUniform4i(location, value[0], value[1], value[2], value[3]);
                break;
        }
    }

    void setUniform(const string &key, float *value, int len) {
        int location = glGetUniformLocation(shaderProgram, key.data());
        switch (len) {
            case 1:
                glUniform1f(location, value[0]);
                break;
            case 2:
                glUniform2f(location, value[0], value[1]);
                break;
            case 3:
                glUniform3f(location, value[0], value[1], value[2]);
                break;
            case 4:
                glUniform4f(location, value[0], value[1], value[2], value[3]);
                break;
        }
    }
};

#endif //OPENGL_ENGINE_RENDERER_H

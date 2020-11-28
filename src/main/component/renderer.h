//
// Created by HKQ on 2020/11/27.
//

#ifndef OPENGL_ENGINE_RENDERER_H
#define OPENGL_ENGINE_RENDERER_H

#include "../define.h"


class Renderer {
    friend Object;
private:
    const char *vertexFile, *fragmentFile;
    bool complete = false;
    int shaderProgram{};

    void openFile(const char *filePath, string &res) {
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

    bool init() {
        string vertexCode, fragmentCode;
        openFile(vertexFile, vertexCode);
        openFile(fragmentFile, fragmentCode);
        return compile(vertexCode.data(), fragmentCode.data());
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
        if (complete || init()) glUseProgram(shaderProgram);
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

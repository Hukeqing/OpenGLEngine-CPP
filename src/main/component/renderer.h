//
// Created by HKQ on 2020/11/27.
//

#ifndef OPENGL_ENGINE_RENDERER_H
#define OPENGL_ENGINE_RENDERER_H

#include "../define.h"

#include "material.h"
#include "light.h"
#include "direction_light.h"
#include "point_light.h"
#include "spot_light.h"

enum RendererValue {
    TypeModel = 1,
    TypeView = 2,
    TypeProjection = 4,
    TypeViewPos = 8,
    TypeDirLight = 16,
    TypePointLight = 32,
    TypeSpotLight = 64,
    TypeMaterial = 128,
};

enum RendererMode {
    NormalMode = GL_LESS,
    OverlapMode = GL_LEQUAL
};

class Renderer {
    friend Object;
private:
    bool complete = false;
    const char *vertexFile, *fragmentFile;
    Material *material;
    int shaderProgram{};

    RendererMode rendererMode = NormalMode;

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

    bool init() {
        string vertexCode, fragmentCode;
        openShaderFile(vertexFile, vertexCode);
        openShaderFile(fragmentFile, fragmentCode);
        if (!compile(vertexCode.data(), fragmentCode.data())) return false;
        glUseProgram(shaderProgram);
        glUniform1i(glGetUniformLocation(shaderProgram, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram, "material.specular"), 1);
        glUseProgram(0);
        material->init();
        return true;
    }

    void use(const glm::mat4 model, const glm::mat4 &view, const glm::mat4 &projection,
             const glm::vec3 viewPos,
             const vector<DirectionLight *> &directionLights,
             const vector<PointLight *> &pointLights,
             const vector<SpotLight *> &spotLights) {
        if (!complete && !init()) return;

        glDepthFunc(rendererMode);

        glUseProgram(shaderProgram);
        material->use(shaderProgram);

        setUniform("model", model);
        setUniform("view", view);
        setUniform("projection", projection);

        setUniform("dirCount", directionLights.size());
        setUniform("pointCount", pointLights.size());
        setUniform("spotCount", spotLights.size());

        setUniform("viewPos", viewPos.x, viewPos.y, viewPos.z);

        for (int i = 0; i < directionLights.size(); ++i) {
            setUniform("dirLights[" + to_string(i) + "].direction",
                       directionLights[i]->direction.x,
                       directionLights[i]->direction.y,
                       directionLights[i]->direction.z);
            setUniform("dirLights[" + to_string(i) + "].color",
                       directionLights[i]->color.r,
                       directionLights[i]->color.g,
                       directionLights[i]->color.b);
            setUniform("dirLights[" + to_string(i) + "].ambient",
                       directionLights[i]->ambient,
                       directionLights[i]->ambient,
                       directionLights[i]->ambient);
            setUniform("dirLights[" + to_string(i) + "].diffuse",
                       directionLights[i]->diffuse,
                       directionLights[i]->diffuse,
                       directionLights[i]->diffuse);
            setUniform("dirLights[" + to_string(i) + "].specular",
                       directionLights[i]->specular,
                       directionLights[i]->specular,
                       directionLights[i]->specular);
        }
        for (int i = 0; i < pointLights.size(); ++i) {
            setUniform("pointLights[" + to_string(i) + "].position",
                       pointLights[i]->position.x,
                       pointLights[i]->position.y,
                       pointLights[i]->position.z);
            setUniform("pointLights[" + to_string(i) + "].color",
                       pointLights[i]->color.r,
                       pointLights[i]->color.g,
                       pointLights[i]->color.b);
            setUniform("pointLights[" + to_string(i) + "].constant", pointLights[i]->constant);
            setUniform("pointLights[" + to_string(i) + "].linear", pointLights[i]->linear);
            setUniform("pointLights[" + to_string(i) + "].quadratic", pointLights[i]->quadratic);
            setUniform("pointLights[" + to_string(i) + "].ambient",
                       pointLights[i]->ambient,
                       pointLights[i]->ambient,
                       pointLights[i]->ambient);
            setUniform("pointLights[" + to_string(i) + "].diffuse",
                       pointLights[i]->diffuse,
                       pointLights[i]->diffuse,
                       pointLights[i]->diffuse);
            setUniform("pointLights[" + to_string(i) + "].specular",
                       pointLights[i]->specular,
                       pointLights[i]->specular,
                       pointLights[i]->specular);
        }
        for (int i = 0; i < spotLights.size(); ++i) {
            setUniform("spotLights[" + to_string(i) + "].position",
                       spotLights[i]->position.x,
                       spotLights[i]->position.y,
                       spotLights[i]->position.z);
            setUniform("spotLights[" + to_string(i) + "].direction",
                       spotLights[i]->direction.x,
                       spotLights[i]->direction.y,
                       spotLights[i]->direction.z);
            setUniform("spotLights[" + to_string(i) + "].color",
                       spotLights[i]->color.r,
                       spotLights[i]->color.g,
                       spotLights[i]->color.b);
            setUniform("spotLights[" + to_string(i) + "].cutOff", spotLights[i]->cutOff);
            setUniform("spotLights[" + to_string(i) + "].outerCutOff", spotLights[i]->outerCutOff);
            setUniform("spotLights[" + to_string(i) + "].constant", spotLights[i]->constant);
            setUniform("spotLights[" + to_string(i) + "].linear", spotLights[i]->linear);
            setUniform("spotLights[" + to_string(i) + "].quadratic", spotLights[i]->quadratic);
            setUniform("spotLights[" + to_string(i) + "].ambient",
                       spotLights[i]->ambient,
                       spotLights[i]->ambient,
                       spotLights[i]->ambient);
            setUniform("spotLights[" + to_string(i) + "].diffuse",
                       spotLights[i]->diffuse,
                       spotLights[i]->diffuse,
                       spotLights[i]->diffuse);
            setUniform("spotLights[" + to_string(i) + "].specular",
                       spotLights[i]->specular,
                       spotLights[i]->specular,
                       spotLights[i]->specular);
        }
    }

    void unUse() {
        glUseProgram(0);
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

    void setMaterial(Material *m) { material = m; }

    void setRendererMode(RendererMode mode) { rendererMode = mode; }

    void setUniform(const string &key, const glm::mat4 &value) {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, key.data()), 1, GL_FALSE, glm::value_ptr(value));
    }

    void setUniform(const string &key, const int value) {
        glUniform1i(glGetUniformLocation(shaderProgram, key.data()), value);
    }

    void setUniform(const string &key, int a, int b) {
        glUniform2i(glGetUniformLocation(shaderProgram, key.data()), a, b);
    }

    void setUniform(const string &key, int a, int b, int c) {
        glUniform3i(glGetUniformLocation(shaderProgram, key.data()), a, b, c);
    }

    void setUniform(const string &key, float a, float b, float c) {
        glUniform3f(glGetUniformLocation(shaderProgram, key.data()), a, b, c);
    }

    void setUniform(const string &key, int a, int b, int c, int d) {
        glUniform4i(glGetUniformLocation(shaderProgram, key.data()), a, b, c, d);
    }
};

#endif //OPENGL_ENGINE_RENDERER_H

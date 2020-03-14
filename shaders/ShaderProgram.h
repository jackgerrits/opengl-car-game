#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#define _USE_MATH_DEFINES

#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#pragma warning(push, 0)
#include <glm/ext.hpp>
#pragma warning(pop)
#include <GL/glew.h>

// Abstract shader program class, holds all uniforms,
class ShaderProgram {
protected:
    GLuint shaderID;

private:
    int compileShader(const std::string& ShaderPath, GLuint ShaderID);
    GLuint loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path);
public:
    ShaderProgram(const std::string&, const std::string&);
    ShaderProgram(int);
    virtual void enable();
    virtual void disable();

    template <typename T>
    void loadLightUniform(const std::string& property, int index, const T& value);

    // Uniform loading helpers
    void loadUniformValue(GLuint uniformLocation, int value);
    void loadUniformValue(GLuint uniformLocation, float value);
    void loadUniformValue(GLuint uniformLocation, glm::vec2 value);
    void loadUniformValue(GLuint uniformLocation, glm::vec3 value);
    void loadUniformValue(GLuint uniformLocation, glm::vec4 value);
    void loadUniformValue(GLuint uniformLocation, glm::mat2 value);
    void loadUniformValue(GLuint uniformLocation, glm::mat3 value);
    void loadUniformValue(GLuint uniformLocation, glm::mat4 value);
    void loadUniformValue(GLuint uniformLocation, float* value, int count);

    GLuint getShaderID();
};


// Should generalise this to take 'lights' as a parameter
template <typename T>
void ShaderProgram::loadLightUniform(const std::string& property, int index, const T& value) {
    // Lights are passed as an array of structs. However these are essentially bound and send individually.
    // They have special uniform name syntax though. ie uniform_name[i].property -> lights[0].position
    std::ostringstream ss;
    ss << "lights[" << index << "]." << property;
    std::string uniformName = ss.str();

    GLuint uniform_location = glGetUniformLocation(shaderID, uniformName.c_str());
    loadUniformValue(uniform_location, value);
}

#endif //SHADERPROGRAM_H
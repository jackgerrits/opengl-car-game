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
#include <glm/ext.hpp>
#include <GL/glew.h>

// Abstract shader program class, holds all uniforms, 
class ShaderProgram {
protected:
    GLuint shaderID;

private:
    // Taken from previous given shader loader.
    int compileShader(const char *ShaderPath, const GLuint ShaderID);
    GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);
public:
    ShaderProgram(std::string, std::string);
    ShaderProgram(int);
    virtual void bindUniformLocations()=0;
    virtual void enable();
    virtual void disable();

    template <typename T>
    void loadLightUniform(std::string property, int index,const T& value);

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


// Should generalise this to take 'lights' as a paramter
template <typename T>
void ShaderProgram::loadLightUniform(std::string property, int index, const T& value){
    // Lights are passed as an array of structs. However these are essentially bound and send individually.
    // They have special uniform name syntax though. ie uniform_name[i].property -> lights[0].position
    std::ostringstream ss;
    ss << "lights[" << index << "]." << property;
    std::string uniformName = ss.str();

    GLuint uniform_location = glGetUniformLocation(shaderID, uniformName.c_str());
    loadUniformValue(uniform_location, value);
}

#endif //SHADERPROGRAM_H
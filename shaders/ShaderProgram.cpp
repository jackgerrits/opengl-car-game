#include "ShaderProgram.h"

#include <fstream>
#include <iostream>
#include <vector>

#include "glm_ext.h"

ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
    this->shaderID = loadShaders(vertexShader, fragmentShader);
}

ShaderProgram::ShaderProgram(int shaderID) {
    this->shaderID = shaderID;
}

void ShaderProgram::enable() {
    glUseProgram(shaderID);
}

void ShaderProgram::disable() {
    glUseProgram(0);
}

GLuint ShaderProgram::getShaderID() {
    return shaderID;
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, int value) {
    glUniform1i(uniformLocation, value);
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, float value) {
    glUniform1f(uniformLocation, value);
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, glm::vec2 value) {
    glUniform2fv(uniformLocation, 1, &value.x);
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, glm::vec3 value) {
    glUniform3fv(uniformLocation, 1, &value.x);
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, glm::vec4 value) {
    glUniform4fv(uniformLocation, 1, &value.x);
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, glm::mat2 value) {
    glUniformMatrix2fv(uniformLocation, 1, false, glm::value_ptr(value));
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, glm::mat3 value) {
    glUniformMatrix3fv(uniformLocation, 1, false, glm::value_ptr(value));
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, glm::mat4 value) {
    glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(value));
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, float* value, int count) {
    switch (count) {
        case 1:
            glUniform1f(uniformLocation, *value);
            break;
        case 2:
            glUniform2fv(uniformLocation, 1, value);
            break;
        case 3:
            glUniform3fv(uniformLocation, 1, value);
            break;
        case 4:
            glUniform4fv(uniformLocation, 1, value);
            break;
        default:
            std::cerr << "Cant load uniform with " << count << " dimensions." << std::endl;
    }
}

int ShaderProgram::compileShader(const std::string& shader_path, GLuint shader_id) {
    // Read shader code from file
    std::string shader_code;
    std::ifstream shader_file_stream(shader_path, std::ios::in);
    if (shader_file_stream.is_open()) {
        std::string line;
        while (getline(shader_file_stream, line)) {
            shader_code += "\n" + line;
        }
        shader_file_stream.close();
    } else {
        std::cerr << "Cannot open " << shader_path << ". Are you in the right directory?" << std::endl;
        return 0;
    }

    // Compile Shader
    char const* SourcePointer = shader_code.c_str();
    glShaderSource(shader_id, 1, &SourcePointer, nullptr);
    glCompileShader(shader_id);

    // Check Shader
    GLint result = GL_FALSE;
    int info_log_length;

    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
    printf("compiled shader %d %d\n", result, info_log_length);
    if (info_log_length > 1) {
        std::vector<char> shader_error_message(info_log_length + 1);
        glGetShaderInfoLog(shader_id, info_log_length, nullptr, shader_error_message.data());

        std::cerr << shader_error_message.data() << std::endl;
        return 0;
    }
    return 1;
}

GLuint ShaderProgram::loadShaders(const std::string& vertex_file_path, const std::string& fragment_file_path) {
    // Create the shaders
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile both shaders. Exit if compile errors.
    if (!compileShader(vertex_file_path, vertex_shader_id) || !compileShader(fragment_file_path, fragment_shader_id)) {
        return 0;
    }

    // Link the program
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    // Check the program
    GLint result = GL_FALSE;
    int info_log_length;

    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
    if (info_log_length > 0) {
        std::vector<char> program_error_message(info_log_length + 1);
        glGetProgramInfoLog(program_id, info_log_length, nullptr, &program_error_message[0]);
        std::cerr << &program_error_message[0] << std::endl;
    }

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}

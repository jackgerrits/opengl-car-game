#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(std::string vertexShader, std::string fragmentShader){
    this->shaderID = loadShaders(vertexShader.c_str(), fragmentShader.c_str());
}

ShaderProgram::ShaderProgram(int shaderID){
    this->shaderID = shaderID;
}

void ShaderProgram::enable(){
    glUseProgram(shaderID);
}

void ShaderProgram::disable(){
    glUseProgram(0);
}

GLuint ShaderProgram::getShaderID() {
    return shaderID;
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, int value){
    glUniform1i(uniformLocation, value);
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, float value){
    glUniform1f(uniformLocation, value);
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, glm::vec2 value){
    glUniform2fv(uniformLocation, 1, &value.x);
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, glm::vec3 value){
    glUniform3fv(uniformLocation, 1, &value.x);
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, glm::vec4 value){
    glUniform4fv(uniformLocation, 1, &value.x);
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, glm::mat2 value){
    glUniformMatrix2fv(uniformLocation, 1, false, glm::value_ptr(value));
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, glm::mat3 value){
    glUniformMatrix3fv(uniformLocation, 1, false, glm::value_ptr(value));
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, glm::mat4 value){
    glUniformMatrix4fv(uniformLocation, 1, false, glm::value_ptr(value));
}

void ShaderProgram::loadUniformValue(GLuint uniformLocation, float* value, int count){
    switch(count){
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

int ShaderProgram::compileShader(const char *ShaderPath, const GLuint ShaderID) {
    // Read shader code from file
    std::string ShaderCode;
    std::ifstream ShaderStream (ShaderPath, std::ios::in);
    if (ShaderStream.is_open()) {
        std::string Line = "";
        while (getline(ShaderStream, Line)) {
            ShaderCode += "\n" + Line;
        }
        ShaderStream.close();
    }
    else {
        std::cerr << "Cannot open " << ShaderPath << ". Are you in the right directory?" << std::endl;
        return 0;
    }

    // Compile Shader
    char const *SourcePointer = ShaderCode.c_str();
    glShaderSource(ShaderID, 1, &SourcePointer , NULL);
    glCompileShader(ShaderID);

    // Check Shader
    GLint Result = GL_FALSE;
    int InfoLogLength;

    glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    printf("compiled shader %d %d\n", Result, InfoLogLength);
    if ( InfoLogLength > 1 ) {
        char ShaderErrorMessage[InfoLogLength+1];
        glGetShaderInfoLog( ShaderID,
                            InfoLogLength,
                            NULL,
                            &ShaderErrorMessage[0]);
        std::cerr << &ShaderErrorMessage[0] << std::endl;
        return 0;
    }
    return 1;
}

GLuint ShaderProgram::loadShaders(const char * vertex_file_path, const char * fragment_file_path ) {
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile both shaders. Exit if compile errors.
    if ( !compileShader(vertex_file_path, VertexShaderID)
         || !compileShader(fragment_file_path, FragmentShaderID) ) {
        return 0;
    }

    // Link the program
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    GLint Result = GL_FALSE;
    int InfoLogLength;
    
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if ( InfoLogLength > 0 ) {
        std::vector<char> ProgramErrorMessage(InfoLogLength+1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        std::cerr << &ProgramErrorMessage[0] << std::endl;
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}
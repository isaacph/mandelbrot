#include "graphics.h"

int readShader(const std::string_view& file) {
    int shaderType;
    if (file.substr(file.length() - VERTEX_SHADER_SUFFIX.length()) == VERTEX_SHADER_SUFFIX) {
        shaderType = GL_VERTEX_SHADER;
    } else if (file.substr(file.length() - FRAGMENT_SHADER_SUFFIX.length()) == FRAGMENT_SHADER_SUFFIX) {
        shaderType = GL_FRAGMENT_SHADER;
    } else {
        throw std::runtime_error(std::string("Invalid shader pathname \"") + std::string(file) + "\": invalid suffix");
    }
    
    int shader = glCreateShader(shaderType);
    std::string shaderSource = readFile(file);
    const GLchar* shaderSourcePtr = shaderSource.c_str();
    const GLint shaderSourceLen = shaderSource.length() + 1;
    glShaderSource(shader, 1, &shaderSourcePtr, &shaderSourceLen);
    glCompileShader(shader);

    // get shader info log
    int logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::string infoLog(logLength, '\0');
    if (logLength > 0) {
        glGetShaderInfoLog(shader, logLength, &logLength, infoLog.data());
    }

    // get shader compile status
    int compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

    if (compileStatus == GL_TRUE) {
        // compilation was successful
        // print the info log anyway if there is one
        if (logLength > 0) {
            std::cout << "Shader compile warning: " << file << "\n";
            std::cout << infoLog << std::endl;
        }
    } else {
        // compilation failed, throw the log as an error
        throw std::runtime_error(std::string("Shader compile error: ") + std::string(file) + "\n" + infoLog);
    }

    return shader;
}

void checkProgram(int program) {
    int infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
    std::string infoLog(infoLogLength, '\0');
    if (infoLogLength > 0) {
        glGetProgramInfoLog(program, infoLogLength, &infoLogLength, infoLog.data());
    }

    int linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_TRUE) {
        // linking was successful
        // print info log anyway
        if (infoLogLength > 0) {
            std::cout << "Program linking warning: " << program << "\n";
            std::cout << infoLog << std::endl;
        }
    } else {
        // linking failed, throw log as an error
        throw std::runtime_error(std::string("Shader program ") + std::to_string(program) + " linking error: \n" + infoLog);
    }
}

GLuint makeNearestTexture(const std::string_view& file) {
    GLuint tex;
    int width, height;
    std::vector<GLubyte> textureData = readImage(file, width, height);
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data()); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    return tex;
}


#include "utils.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

uint32_t CreateShaderFromSource(GLenum type, const std::string& source, const std::string& source_file_name) {
    GLuint shd = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shd, 1, &src, nullptr);
    glCompileShader(shd);
    GLint testVal = 0;
    glGetShaderiv(shd, GL_COMPILE_STATUS, &testVal);
    if (testVal == GL_FALSE) {
        char infoLog[1024] = {0};
        glGetShaderInfoLog(shd, 1024, NULL, infoLog);
        std::stringstream msg;
        msg << "The shader " << source_file_name << " failed to compile with the following error:\n " << infoLog
            << "\n";
        glDeleteShader(shd);
        throw std::runtime_error(msg.str());
    }
    return shd;
}
std::string LoadFile(const std::filesystem::path& fileName) {
    std::ifstream stream(fileName.c_str());
    if (!stream.is_open()) {
        throw std::runtime_error("Can't open shader file " + fileName.string());
    }
    std::ostringstream tmp;
    tmp << stream.rdbuf();
    return tmp.str();
}

uint32_t CreateShaderFromFile(GLenum type, const std::filesystem::path& file_name) {
    std::string src = LoadFile(file_name.c_str());
    return CreateShaderFromSource(type, src, file_name.string());
}
std::string FormatDebugOutput(size_t outStrSize, GLenum source, GLenum type, GLuint id, GLenum severity,
                              const char* msg) {
    std::string srcStr = "UNDEFINED";
    switch (source)

    {
        case GL_DEBUG_SOURCE_API:
            srcStr = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            srcStr = "WINDOW_SYSTEM";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            srcStr = "SHADER_COMPILER";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            srcStr = "THIRD_PARTY";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            srcStr = "APPLICATION";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            srcStr = "OTHER";
            break;
    }

    std::string typeFmt = "UNDEFINED";
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            typeFmt = "ERROR";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typeFmt = "DEPRECATED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typeFmt = "UNDEFINED_BEHAVIOR";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            typeFmt = "PORTABILITY";
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            typeFmt = "PERFORMANCE";
            break;
        case GL_DEBUG_TYPE_OTHER:
            typeFmt = "OTHER";
            break;
    }

    std::string severityStr = "UNDEFINED";
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            severityStr = "HIGH";
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            severityStr = "MEDIUM";
            break;
        case GL_DEBUG_SEVERITY_LOW:
            severityStr = "LOW";
            break;
    }

    std::string message = "OpenGL: " + std::string(msg) + " [source= " + srcStr + " type= " + typeFmt +
                          " severity= " + severityStr + "]\n";
    return message;
}

void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                   const GLvoid* userParam) {
    std::string msg = FormatDebugOutput(256, source, type, id, severity, message);
    std::cout << msg;
}

GLuint CreateProgram(const std::vector<GLuint> shaders) {
    auto shaderProgram = glCreateProgram();
    for (auto shader : shaders) {
        glAttachShader(shaderProgram, shader);
    }

    int32_t link_status = 0;

    glLinkProgram(shaderProgram);
    // query the link status
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE) {
        char infoLog[1024] = {0};
        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        std::stringstream msg;
        msg << "The program at " << shaderProgram << " failed to link with the following error:\n " << infoLog << "\n";
        throw std::runtime_error(msg.str());
    }
    return shaderProgram;
}
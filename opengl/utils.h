#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H
#include <filesystem>
#include <vector>

#include "glad/glad.h"
uint32_t CreateShaderFromFile(GLenum type, const std::filesystem::path& file_name);
void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                   const GLvoid* userParam);
GLuint CreateProgram(const std::vector<GLuint> shaders);
#endif
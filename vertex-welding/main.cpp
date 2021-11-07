#include <chrono>
#include <iostream>

#include "glad/glad.h"
#define GLM_FORCE_RADIANS
#include "GLFW/glfw3.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "opengl/utils.h"
#include "scene.h"
const int width = 1024;
const int height = 768;

GLFWwindow* InitWindowAndOpenGL();
std::unique_ptr<Scene> scene;

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        scene->ToggleWireframe();
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        scene->TogglePause();
    }
}

int main(int argc, char** argv) {
    auto window = InitWindowAndOpenGL();
    if (!window) {
        return -1;
    }
    glfwSetKeyCallback(window, KeyCallback);
    scene.reset(new Scene);
    scene->Init(width, height);

    auto currentTime = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        auto newTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> frameTime = newTime - currentTime;
        currentTime = newTime;
        scene->Render(frameTime.count());
        glfwSwapBuffers(window);
    }
    scene.reset();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

GLFWwindow* InitWindowAndOpenGL() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    auto window = glfwCreateWindow(width, height, "vertex-welding", nullptr, nullptr);
    if (!window) {
        const char* errorDesc;
        glfwGetError(&errorDesc);
        std::cerr << "Can't create a GLFW window." << std::endl;
        std::cerr << "Reason: " << errorDesc << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }
    if (GLAD_GL_KHR_debug) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        // use only the medium > severity
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, NULL, GL_TRUE);
        glDebugMessageCallback(DebugCallback, nullptr);
    }
    std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << " successfully created" << std::endl;
    return window;
}
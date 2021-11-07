#ifndef SCENE_H
#define SCENE_H
#include "camera.h"
#include "glad/glad.h"
#include "glm/glm.hpp"

class Scene {
   public:
    ~Scene();
    void Init(int width, int height);
    void Render(float dt);
    void ToggleWireframe();
    void TogglePause() {
        pause = !pause;
    }

   private:
    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;
    GLuint shaderProgram = 0;
    GLuint vertexBuffer = 0;
    GLuint indexBuffer = 0;
    GLuint vertexArrayObject = 0;

    Camera camera;
    float time = 0;
    float maxTime = 10;

    bool wireFrame = false;
    uint32_t numIndices = 0;
    bool pause = false;

    glm::vec3 GetCameraPath(float t) const;
    void Update(float dt);
};
#endif

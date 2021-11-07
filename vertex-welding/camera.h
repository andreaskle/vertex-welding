#ifndef CAMERA_H_
#define CAMERA_H_
#include "glm/glm.hpp"
class Camera {
   public:
    Camera();
    void SetTarget(const glm::vec3& tg) {
        target = tg;
    }
    void SetPosition(const glm::vec3& pos) {
        position = pos;
    }
    glm::mat4 GetViewProjMatrix() const;
    glm::vec3 GetPosition() const {
        return position;
    }
    void perspective(int width, int height);

   private:
    glm::vec3 position, target, up;
    glm::mat4 proj;
};
#endif
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Target;
    glm::vec3 Up;
    
    // Camera options
    float Distance;
    float Height;
    float Pitch;

    Camera(glm::vec3 startPos = glm::vec3(0.0f, 0.0f, 0.0f));

    // Update camera position based on car position and rotation
    void update(glm::vec3 carPosition, float carYaw);

    // Get view matrix
    glm::mat4 GetViewMatrix();
};

#endif

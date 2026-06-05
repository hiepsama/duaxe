#include "Camera.h"
#include <cmath>

Camera::Camera(glm::vec3 startPos) {
    Position = startPos;
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    Distance = 8.0f;  // Further back to see more of the road
    Height = 3.5f;    // Higher for better overview
    Pitch = 5.0f;
}

void Camera::update(glm::vec3 carPosition, float carYaw) {
    float yawRad = glm::radians(carYaw);

    // Camera sits behind and above the car
    float offsetX = sin(yawRad) * Distance;
    float offsetZ = cos(yawRad) * Distance;

    Position.x = carPosition.x - offsetX;
    Position.y = carPosition.y + Height;
    Position.z = carPosition.z - offsetZ;

    // Look slightly ahead of the car (12 units) and at hood level
    Target = carPosition;
    Target.y += 0.8f;
    Target.x += sin(yawRad) * 12.0f;
    Target.z += cos(yawRad) * 12.0f;
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Target, Up);
}

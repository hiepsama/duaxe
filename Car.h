#ifndef CAR_H
#define CAR_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Car {
public:
    glm::vec3 Position;
    float Yaw; // Rotation angle around Y axis (in degrees)
    float Speed;
    
    // Properties
    float MaxSpeed;
    float Acceleration;
    float Deceleration;
    float TurnSpeed;

    // Bounding box for simple collision 
    // Assumes Car is a cube from -0.5 to 0.5 in local space
    glm::vec3 Size; 

    Car(glm::vec3 startPos = glm::vec3(0.0f, 0.5f, 0.0f));

    // Handle movement logic
    // forward/backward controls throttle (-1, 0, 1)
    // left/right controls steering (-1, 0, 1)
    void update(float deltaTime, int forwardBackward, int leftRight);

    // Get basic transformation matrix (without scale)
    glm::mat4 GetModelMatrix();
    
    // Draw the multi-part car model
    void Draw(class Shader& shader, unsigned int cubeVAO);
    
    // Simple AABB vs AABB collision check (ignoring rotation for simplicity in basic version)
    bool checkCollision(glm::vec3 obstaclePos, glm::vec3 obstacleSize);
};

#endif

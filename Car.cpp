#include "Car.h"
#include "Shader.h"
#include <cmath>
#include <algorithm>

namespace {
    constexpr float kRoadHalfWidth = 7.8f;
}

Car::Car(glm::vec3 startPos) {
    Position = startPos;
    Yaw = 0.0f;
    Speed = 0.0f;
    
    MaxSpeed = 40.0f;
    Acceleration = 20.0f;
    Deceleration = 10.0f;
    TurnSpeed = 120.0f; // degrees per second
    Size = glm::vec3(1.0f, 1.0f, 2.0f); // Width 1.0, Height 1.0, Length 2.0
}

void Car::update(float deltaTime, int forwardBackward, int leftRight) {
    // 1. Handle Speed (Acceleration & Braking/Friction)
    if (forwardBackward == 1) { // W key: Accelerate Forward
        Speed += Acceleration * deltaTime;
    } else if (forwardBackward == -1) { // S key: Brake / Reverse
        Speed -= Acceleration * deltaTime;
    } else { // No key: Natural deceleration
        if (Speed > 0) {
            Speed -= Deceleration * deltaTime;
            if (Speed < 0) Speed = 0;
        } else if (Speed < 0) {
            Speed += Deceleration * deltaTime;
            if (Speed > 0) Speed = 0;
        }
    }

    // Clamp speed to max limits
    if (Speed > MaxSpeed) Speed = MaxSpeed;
    if (Speed < -MaxSpeed / 2.0f) Speed = -MaxSpeed / 2.0f; // Max reverse speed is slower

    // 2. Handle Steering
    // Car only steers linearly when moving (realistic constraint)
    if (abs(Speed) > 0.1f) {
        float turnMultiplier = (Speed > 0) ? 1.0f : -1.0f; // Reverse steering when reversing
        if (leftRight == 1) { // A key: Turn Left
            Yaw += TurnSpeed * deltaTime * turnMultiplier;
        } else if (leftRight == -1) { // D key: Turn Right
            Yaw -= TurnSpeed * deltaTime * turnMultiplier;
        }
    }

    // 3. Update Position based on Yaw and Speed
    float yawRad = glm::radians(Yaw);
    
    // Direction vector points along the Z-axis local to the car
    glm::vec3 direction(sin(yawRad), 0.0f, cos(yawRad));

    float moveStep = Speed * deltaTime;
    glm::vec3 nextPosition = Position + direction * moveStep;

    // Keep the whole car inside the road bounds before committing the move.
    float carHalfWidth = Size.x * 0.5f + 0.7f;
    float minX = -kRoadHalfWidth + carHalfWidth;
    float maxX =  kRoadHalfWidth - carHalfWidth;

    if (nextPosition.x < minX) nextPosition.x = minX;
    if (nextPosition.x > maxX) nextPosition.x = maxX;

    Position = nextPosition;
}

glm::mat4 Car::GetModelMatrix() {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, Position);
    model = glm::rotate(model, glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
    return model;
}

void Car::Draw(Shader& shader, unsigned int cubeVAO) {
    glBindVertexArray(cubeVAO);
    glm::mat4 baseTransform = GetModelMatrix();
    glm::mat4 model;

    // --- CAR BODY ---
    // 1. Main Body (deep metallic blue)
    model = baseTransform;
    model = glm::translate(model, glm::vec3(0.0f, Size.y / 2.0f, 0.0f));
    model = glm::scale(model, Size);
    shader.setMat4("model", model);
    shader.setVec3("objectColor", 0.05f, 0.25f, 0.75f); // Deep blue body
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 2. Hood (front half, slightly darker)
    model = baseTransform;
    model = glm::translate(model, glm::vec3(0.0f, Size.y + 0.01f, Size.z * 0.22f));
    model = glm::scale(model, glm::vec3(Size.x, 0.05f, Size.z * 0.5f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", 0.04f, 0.18f, 0.55f); // Darker blue hood
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 3. Cabin / Roof (compact coupe style)
    model = baseTransform;
    model = glm::translate(model, glm::vec3(0.0f, Size.y + 0.38f, -0.15f));
    model = glm::scale(model, glm::vec3(Size.x * 0.82f, 0.55f, Size.z * 0.48f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", 0.04f, 0.18f, 0.55f); // Dark blue cabin
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 4. Windshield (light teal/glass color)
    model = baseTransform;
    model = glm::translate(model, glm::vec3(0.0f, Size.y + 0.42f, Size.z * 0.08f));
    model = glm::scale(model, glm::vec3(Size.x * 0.75f, 0.42f, 0.12f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", 0.4f, 0.85f, 0.95f); // Glass color
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 5. Rear window
    model = baseTransform;
    model = glm::translate(model, glm::vec3(0.0f, Size.y + 0.42f, -Size.z * 0.30f));
    model = glm::scale(model, glm::vec3(Size.x * 0.70f, 0.38f, 0.10f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", 0.3f, 0.75f, 0.85f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 6. Racing stripe on roof (white)
    model = baseTransform;
    model = glm::translate(model, glm::vec3(0.0f, Size.y + 0.67f, -0.15f));
    model = glm::scale(model, glm::vec3(0.18f, 0.03f, Size.z * 0.47f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", 1.0f, 1.0f, 1.0f); // White stripe
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 7. Front bumper (light gray)
    model = baseTransform;
    model = glm::translate(model, glm::vec3(0.0f, Size.y * 0.3f, Size.z * 0.52f));
    model = glm::scale(model, glm::vec3(Size.x * 1.0f, Size.y * 0.35f, 0.12f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", 0.6f, 0.6f, 0.65f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 8. Rear bumper (light gray)
    model = baseTransform;
    model = glm::translate(model, glm::vec3(0.0f, Size.y * 0.3f, -Size.z * 0.52f));
    model = glm::scale(model, glm::vec3(Size.x * 1.0f, Size.y * 0.35f, 0.12f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", 0.55f, 0.55f, 0.60f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 9. Left Headlight (bright yellow)
    model = baseTransform;
    model = glm::translate(model, glm::vec3(-Size.x * 0.3f, Size.y * 0.65f, Size.z * 0.51f));
    model = glm::scale(model, glm::vec3(0.25f, 0.18f, 0.08f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", 1.0f, 0.95f, 0.3f); // Yellow headlight
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 10. Right Headlight
    model = baseTransform;
    model = glm::translate(model, glm::vec3( Size.x * 0.3f, Size.y * 0.65f, Size.z * 0.51f));
    model = glm::scale(model, glm::vec3(0.25f, 0.18f, 0.08f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", 1.0f, 0.95f, 0.3f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 11. Left Tail Light (red)
    model = baseTransform;
    model = glm::translate(model, glm::vec3(-Size.x * 0.3f, Size.y * 0.65f, -Size.z * 0.51f));
    model = glm::scale(model, glm::vec3(0.25f, 0.18f, 0.08f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", 1.0f, 0.1f, 0.1f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 12. Right Tail Light (red)
    model = baseTransform;
    model = glm::translate(model, glm::vec3( Size.x * 0.3f, Size.y * 0.65f, -Size.z * 0.51f));
    model = glm::scale(model, glm::vec3(0.25f, 0.18f, 0.08f));
    shader.setMat4("model", model);
    shader.setVec3("objectColor", 1.0f, 0.1f, 0.1f);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // --- WHEELS ---
    float wRadius = 0.42f;
    float wThick  = 0.22f;
    float wX = Size.x / 2.0f + wThick / 2.0f;
    float wY = 0.0f; // wheels at ground level relative to body base
    float wZf = Size.z * 0.34f;  // front axle
    float wZr = -Size.z * 0.34f; // rear axle

    glm::vec3 wheelOffsets[4] = {
        glm::vec3(-wX, wY, wZf),  // front left
        glm::vec3( wX, wY, wZf),  // front right
        glm::vec3(-wX, wY, wZr),  // rear left
        glm::vec3( wX, wY, wZr)   // rear right
    };

    for(int i = 0; i < 4; i++) {
        // Tire (dark gray)
        model = baseTransform;
        model = glm::translate(model, wheelOffsets[i]);
        model = glm::scale(model, glm::vec3(wThick, wRadius, wRadius));
        shader.setMat4("model", model);
        shader.setVec3("objectColor", 0.12f, 0.12f, 0.12f); // Dark tire
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Hubcap (silver)
        glm::vec3 hubOffset = wheelOffsets[i];
        float hubSide = (i % 2 == 0) ? -(wThick * 0.55f) : (wThick * 0.55f);
        hubOffset.x += hubSide;
        model = baseTransform;
        model = glm::translate(model, hubOffset);
        model = glm::scale(model, glm::vec3(0.06f, wRadius * 0.65f, wRadius * 0.65f));
        shader.setMat4("model", model);
        shader.setVec3("objectColor", 0.75f, 0.75f, 0.78f); // Silver hubcap
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


bool Car::checkCollision(glm::vec3 obstaclePos, glm::vec3 obstacleSize) {
    // AABB (Axis-Aligned Bounding Box) collision detection
    // For a basic game, we treat the car bounding box as axis-aligned despite rotation
    // In a real game, this would use OBB (Oriented Bounding Box) or cylinders
    
    // Calculate boundaries with some margin
    bool collisionX = Position.x + Size.x/2 >= obstaclePos.x - obstacleSize.x/2 &&
                      obstaclePos.x + obstacleSize.x/2 >= Position.x - Size.x/2;
                      
    bool collisionZ = Position.z + Size.z/2 >= obstaclePos.z - obstacleSize.z/2 &&
                      obstaclePos.z + obstacleSize.z/2 >= Position.z - Size.z/2;
                      
    return collisionX && collisionZ; // If both X and Z overlap, we have a collision
}

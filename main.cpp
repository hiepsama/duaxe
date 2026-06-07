#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Car.h"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <stdio.h>

extern "C" {
    void __mingw_free(void *ptr) { free(ptr); }
    void *__mingw_realloc(void *ptr, size_t size) { return realloc(ptr, size); }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, float deltaTime, Car &car);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Cube vertices with normals
float vertices[] = {
    // positions          // normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

struct Obstacle {
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 color;
};

enum class GameState { START, PLAYING, PAUSED, GAME_OVER };

int loadHighScore()
{
    std::ifstream file("highscore.txt");
    int highScore = 0;
    if (file.is_open()) {
        file >> highScore;
    }
    return highScore;
}

void saveHighScore(int highScore)
{
    std::ofstream file("highscore.txt", std::ios::trunc);
    if (file.is_open()) {
        file << highScore;
    }
}

void resetRun(Car& car, std::vector<Obstacle>& obstacles, float& spawnZ, float& playTime, int& score)
{
    car = Car(glm::vec3(0.0f, 0.5f, 0.0f));
    obstacles.clear();
    spawnZ = 20.0f;
    playTime = 0.0f;
    score = 0;
}

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Racing Game 3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    Shader lightingShader("shader.vs", "shader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Init Game Objects
    Car myCar(glm::vec3(0.0f, 0.5f, 0.0f));
    Camera myCamera(myCar.Position);

    // Light position properties
    glm::vec3 lightPos(10.0f, 20.0f, 10.0f);
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

    // Obstacles
    std::vector<Obstacle> obstacles;
    Obstacle o1; o1.position = glm::vec3(5.0f, 1.0f, 10.0f); o1.size = glm::vec3(2.0f, 2.0f, 2.0f); o1.color = glm::vec3(1.0f, 0.0f, 0.0f);
    obstacles.push_back(o1);
    Obstacle o2; o2.position = glm::vec3(-6.0f, 1.0f, 20.0f); o2.size = glm::vec3(3.0f, 2.0f, 2.0f); o2.color = glm::vec3(0.0f, 0.0f, 1.0f);
    obstacles.push_back(o2);
    Obstacle o3; o3.position = glm::vec3(0.0f, 1.0f, 40.0f); o3.size = glm::vec3(5.0f, 2.0f, 1.0f); o3.color = glm::vec3(1.0f, 1.0f, 0.0f);
    obstacles.push_back(o3);
    Obstacle o4; o4.position = glm::vec3(-10.0f, 1.0f, 50.0f); o4.size = glm::vec3(2.0f, 2.0f, 20.0f); o4.color = glm::vec3(1.0f, 0.5f, 0.0f);
    obstacles.push_back(o4);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    
    GameState gameState = GameState::START;
    int score = 0;
    int highScore = loadHighScore();
    float spawnZ = 20.0f;
    float playTime = 0.0f;
    bool enterWasDown = false;
    bool pWasDown = false;
    bool rWasDown = false;

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        bool enterDown = glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS;
        bool pDown = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS;
        bool rDown = glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS;

        // state transitions
        if (gameState == GameState::START && enterDown && !enterWasDown) {
            resetRun(myCar, obstacles, spawnZ, playTime, score);
            gameState = GameState::PLAYING;
        }

        if ((gameState == GameState::GAME_OVER || gameState == GameState::PAUSED) && rDown && !rWasDown) {
            resetRun(myCar, obstacles, spawnZ, playTime, score);
            gameState = GameState::PLAYING;
        }

        if (gameState == GameState::PLAYING && pDown && !pWasDown) {
            gameState = GameState::PAUSED;
        } else if (gameState == GameState::PAUSED && pDown && !pWasDown) {
            gameState = GameState::PLAYING;
        }

        enterWasDown = enterDown;
        pWasDown = pDown;
        rWasDown = rDown;

        if (gameState == GameState::PLAYING) {
            processInput(window, deltaTime, myCar);
            playTime += deltaTime;

            // Score based on Z position
            score = (int)(myCar.Position.z);
            if (score < 0) score = 0;

            if (score > highScore) {
                highScore = score;
                saveHighScore(highScore);
            }

            float progress = std::min(1.0f, score / 1500.0f);
            myCar.MaxSpeed = 40.0f + progress * 18.0f;
            myCar.Acceleration = 20.0f + progress * 6.0f;

            // Spawning logic (Endless running)
            float spawnInterval = 28.0f - progress * 12.0f;
            if (spawnInterval < 14.0f) spawnInterval = 14.0f;

            if (myCar.Position.z + 200.0f > spawnZ) {
                Obstacle o;
                int lane = rand() % 3;
                float laneX[3] = { -6.0f, 0.0f, 6.0f };
                o.position = glm::vec3(laneX[lane], 1.25f, spawnZ);
                // Curated obstacle type colors
                int obstType = rand() % 4;
                glm::vec3 colors[4] = {
                    glm::vec3(1.0f, 0.45f, 0.0f),  // Traffic cone orange
                    glm::vec3(0.85f, 0.08f, 0.08f), // Red barrel
                    glm::vec3(0.95f, 0.85f, 0.05f), // Yellow barricade
                    glm::vec3(0.9f,  0.3f,  0.0f)   // Warning orange
                };
                o.size = glm::vec3(1.6f + (rand() % 3) * 0.4f, 2.2f + (rand() % 2) * 0.4f, 1.4f + (rand() % 2) * 0.3f);
                o.size += glm::vec3(progress * 0.4f, progress * 0.2f, progress * 0.2f);
                o.color = colors[obstType];
                obstacles.push_back(o);
                spawnZ += spawnInterval + (rand() % 10);
            }


            // Memory management: Cleanup obstacles that are far behind
            while (!obstacles.empty() && obstacles.front().position.z < myCar.Position.z - 30.0f) {
                obstacles.erase(obstacles.begin());
            }

            // collision detection
            for(size_t i = 0; i < obstacles.size(); i++) {
                if(myCar.checkCollision(obstacles[i].position, obstacles[i].size)) {
                        gameState = GameState::GAME_OVER;
                        if (score > highScore) {
                            highScore = score;
                            saveHighScore(highScore);
                        }
                }
            }
        }

            // update camera relative to car
            myCamera.update(myCar.Position, myCar.Yaw);
        
        // Update Window Title
        char title[100];
            if (gameState == GameState::START)
                sprintf(title, "Racing Game 3D - Press ENTER to Start | High Score: %d", highScore);
            else if (gameState == GameState::PLAYING)
                sprintf(title, "Racing Game 3D - Score: %d | High Score: %d | P to Pause", score, highScore);
            else if (gameState == GameState::PAUSED)
                sprintf(title, "PAUSED - Score: %d | High Score: %d | Press P or R", score, highScore);
            else
                sprintf(title, "GAME OVER! Score: %d | High Score: %d | Press R to Restart", score, highScore);
        glfwSetWindowTitle(window, title);

        // render
            float skyFactor = std::min(1.0f, score / 2000.0f);
            glClearColor(0.53f - skyFactor * 0.18f, 0.72f - skyFactor * 0.28f, 0.85f - skyFactor * 0.40f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", myCamera.Position);
        glm::vec3 dynamicLight = lightColor;
        dynamicLight *= (1.0f - skyFactor * 0.25f);
        lightingShader.setVec3("lightColor", dynamicLight);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = myCamera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        glBindVertexArray(VAO);

        glm::mat4 model;

        // 1. Draw Environment (Road + Grass) centered around car's Z
        // Road is dark gray
        model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, myCar.Position.z + 50.0f));
        model = glm::scale(model, glm::vec3(20.0f, 1.0f, 400.0f));
        lightingShader.setMat4("model", model);
        lightingShader.setVec3("objectColor", 0.2f, 0.2f, 0.2f); // Dark road
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Grass left
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-30.0f, -0.5f, myCar.Position.z + 50.0f));
        model = glm::scale(model, glm::vec3(40.0f, 1.0f, 400.0f));
        lightingShader.setMat4("model", model);
        lightingShader.setVec3("objectColor", 0.2f, 0.5f, 0.2f); // Grass
        glDrawArrays(GL_TRIANGLES, 0, 36);
        
        // Grass right
        model = glm::translate(glm::mat4(1.0f), glm::vec3(30.0f, -0.5f, myCar.Position.z + 50.0f));
        model = glm::scale(model, glm::vec3(40.0f, 1.0f, 400.0f));
        lightingShader.setMat4("model", model);
        lightingShader.setVec3("objectColor", 0.2f, 0.5f, 0.2f); // Grass
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Lane markings (dashed white lines) 3 lanes -> 2 div lines
        float startZ = (int)(myCar.Position.z / 10.0f) * 10.0f - 20.0f;
        for (float z = startZ; z < startZ + 200.0f; z += 10.0f) {
            model = glm::translate(glm::mat4(1.0f), glm::vec3(-3.3f, -0.4f, z));
            model = glm::scale(model, glm::vec3(0.3f, 1.0f, 4.0f));
            lightingShader.setMat4("model", model);
            lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
            model = glm::translate(glm::mat4(1.0f), glm::vec3(3.3f, -0.4f, z));
            model = glm::scale(model, glm::vec3(0.3f, 1.0f, 4.0f));
            lightingShader.setMat4("model", model);
            lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // 2. Draw the car
        myCar.Draw(lightingShader, VAO);

        // 3. Draw obstacles (with a dark base under each for depth)
        for(size_t i = 0; i < obstacles.size(); i++) {
            // Main obstacle block
            model = glm::mat4(1.0f);
            model = glm::translate(model, obstacles[i].position);
            model = glm::scale(model, obstacles[i].size);
            lightingShader.setMat4("model", model);
            lightingShader.setVec3("objectColor", obstacles[i].color);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            // Dark base/shadow strip under obstacle
            glm::vec3 basePos = obstacles[i].position;
            basePos.y = -0.3f;
            model = glm::mat4(1.0f);
            model = glm::translate(model, basePos);
            model = glm::scale(model, glm::vec3(obstacles[i].size.x * 1.1f, 0.25f, obstacles[i].size.z * 1.1f));
            lightingShader.setMat4("model", model);
            lightingShader.setVec3("objectColor", 0.1f, 0.1f, 0.1f);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // glfw: swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window, float deltaTime, Car &car)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    int forwardBackward = 0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        forwardBackward = 1;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        forwardBackward = -1;
        
    int leftRight = 0;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        leftRight = 1;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        leftRight = -1;
        
    car.update(deltaTime, forwardBackward, leftRight);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

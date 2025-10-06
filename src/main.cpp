

#include <glad/glad.h>
#include <iomanip>

#include <GLFW/glfw3.h>
#include <iostream>
#include <oneapi/tbb/detail/_utils.h>

#include "shader.h"
#include "types.h"

#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// settings
const uint SRC_WIDTH  = 800;
const uint SRC_HEIGTH = 600;

// camera
Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
f32    lastX{SRC_WIDTH / 2.0f}, lastY{SRC_HEIGTH / 2.0f};
f32    deltaTime{0.0f};
f32    lastFrame{0.0f};

bool firstMouse{true};

int main()
{
    // note: Initialization of glfw and glad, and create window.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGTH, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set up callback for resizing
    glViewport(0, 0, SRC_WIDTH, SRC_HEIGTH);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // mouse callback
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // note: shader program;
    Shader ourShader("shaders/3dProj.vert", "shaders/ourShader.frag");

    ourShader.use();

    // clang-format off
    float vertices[] = {
    // region vertices

    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

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
    //endregion
    };
    // uint indeces[]{
        // 0, 1, 3, // triangle 1
        // 1, 2, 3,
    // };
    // clang-format on

    // region oldvaostuff
    // note: VAO: Vertex array object
    uint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    // note: EBO: Element Buffer Object
    uint EBO{};
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);
    // note: VBO: Vertex Buffer Object

    uint VBO{};
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // note: Tell opengl how to interprete vertex data
    // xyz coords
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void *) (3 * sizeof(f32)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    // texture coords
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(f32), (void *) (3 * sizeof(float)));
    // glEnableVertexAttribArray(1);


    // unbind so later vbos dont accidentally use this VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // endregion

    // TODO: use new dsa api
    // region note: texture options
    // uint texture1{};
    // glGenTextures(1, &texture1);
    // glBindTexture(GL_TEXTURE_2D, texture1);
    //
    // // Mirrored repeat of texture
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // // only applies if we have GL_CLAMP_TO_BORDER
    // // f32 borderColor[]{1.0f, 1.0f, 0.0f, 1.0f};
    // // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //
    // // Don't smooth when scaled down, but smooth when scaled up
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // mipmaps
    // // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // {
    //     int width{}, height{}, nrChannels{};
    //     stbi_set_flip_vertically_on_load(true);
    //     uchar *data{stbi_load(
    //             "textures/container.jpg", &width, &height, &nrChannels, 0)};
    //
    //     if (data)
    //     {
    //         // Generate texture and mipmaps
    //         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //         glGenerateMipmap(GL_TEXTURE_2D);
    //     } else
    //     {
    //         std::cout << "Failed to load texture" << std::endl;
    //     }
    //     stbi_image_free(data);
    // }
    // uint texture2{};
    // glGenTextures(1, &texture2);
    // glBindTexture(GL_TEXTURE_2D, texture2);
    //
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //
    // {
    //     int    width{}, height{}, nrChannels{};
    //     uchar *data{stbi_load(
    //             "textures/awesomeface.png", &width, &height, &nrChannels, 0)};
    //
    //     if (data)
    //     {
    //         // Generate texture and mipmaps
    //         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //         glGenerateMipmap(GL_TEXTURE_2D);
    //     } else
    //     {
    //         std::cout << "Failed to load texture" << std::endl;
    //     }
    //     stbi_image_free(data);
    // }
    // endregion

    // note: set uniforms to use correct textures
    ourShader.use();

    // glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
    // ourShader.setInt("texture1", 0);
    // ourShader.setInt("texture2", 1);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // wireframe :)

    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
    };
    // note: light VAO
    uint lightVAO{};
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void *) (3 * sizeof(f32)));
    glEnableVertexAttribArray(0);


    // note: colours

    Shader lightingShader{"shaders/3dProj.vert", "shaders/lighting.frag"};
    Shader lightCubeShader{"shaders/3dProj.vert", "shaders/lightSource.frag"};
    lightingShader.use();
    lightingShader.setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
    lightingShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    // note: depth testing
    glEnable(GL_DEPTH_TEST);
    // note: Main loop
    while (!glfwWindowShouldClose(window))
    {
        static f32 factor        = 0.5;
        static f32 lastPrintTime = 0.0f;
        static f32 deltaSum      = 0.0f;
        static i32 frames        = 0;

        f32 currentFrame = (f32) glfwGetTime();
        deltaTime        = currentFrame - lastFrame;
        lastFrame        = currentFrame;

        deltaSum += deltaTime;
        frames++;

        if (currentFrame - lastPrintTime >= 3.0f)
        {
            f32 avgDelta = deltaSum / frames;
            f32 avgFps   = frames / (currentFrame - lastPrintTime); // more precise average FPS

            std::cout << std::fixed << std::setprecision(2);
            std::cout << "second: " << (int) glfwGetTime() << " avg FPS: " << avgFps
                      << "  avg Δt: " << avgDelta * 1000.0f << " ms\n";

            frames        = 0;
            deltaSum      = 0.0f;
            lastPrintTime = currentFrame;
        }


        // input
        processInput(window);

        // rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture1);
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, texture2);


        // note: projections
        //  V_clip = M_projection * M_view * M_model * V_local

        glm::vec3 lightPos(1.2f, 0.0f, -2.0f);

        lightingShader.use();

        auto      projection = glm::perspective(glm::radians(camera.Zoom), (float) SRC_WIDTH / (float) SRC_HEIGTH, 0.1f, 100.0f);
        glm::mat4 view{camera.GetViewMatrix()};
        {
            glm::mat4 model = glm::mat4(1.0f);
            model           = glm::translate(model, cubePositions[0]);

            lightingShader.setMat4("projection", projection);
            lightingShader.setMat4("view", view);
            lightingShader.setFloat("factor", factor);
            lightingShader.setMat4("model", model);
            lightingShader.setVec3("lightPos", lightPos);

            // glBindVertexArray(VAO);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        lightCubeShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model           = glm::translate(model, lightPos);
        model           = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.setMat4("model", model);
        lightCubeShader.setMat4("view", view);
        lightCubeShader.setMat4("projection", projection);

        glDrawArrays(GL_TRIANGLES, 0, 36);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}
void processInput(GLFWwindow *window)
{


    static bool pausePressedLastFrame = false;

    bool pausePressedNow = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

    if (pausePressedNow && !pausePressedLastFrame)
    {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
        {
            camera.Frozen = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else
        {
            camera.Frozen = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    pausePressedLastFrame = pausePressedNow;

    const float cameraSpeed = 5.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
}
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{


    if (firstMouse)
    {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX         = xpos;
    lastY         = ypos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

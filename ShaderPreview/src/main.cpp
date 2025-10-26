#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#include <Logger.h>

#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "SphereModel.h"
#include "Types.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, f64 xpos, f64 ypos);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow *window, f64 xoffset, f64 yoffset);
// settings
constexpr uint SRC_WIDTH  = 800;
constexpr uint SRC_HEIGTH = 600;

// camera
Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
f32    lastX{SRC_WIDTH / 2.0f}, lastY{SRC_HEIGTH / 2.0f};
f32    deltaTime{0.0f};
f32    lastFrame{0.0f};

bool firstMouse{true};


int main()
{
    // NOTE: Turn on file logging

    Logger::LogToFile(true);
    // note: Initialization of glfw and glad, and create window.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    GLFWwindow *window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGTH, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        Logger::Get().Error("Failed to create window!");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        Logger::Get().Error("Failed to initialize GLAD!");
        return -1;
    }
    Logger::ClearLog();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, SRC_WIDTH, SRC_HEIGTH);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // NOTE: Outline setup
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);


    int stencilBits;
    glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencilBits);
    Logger::Get().Info("Stencil buffer bits: " + std::to_string(stencilBits));

    const Shader ourShader{"shaders/3dProj.vert", "shaders/Lighting.frag"};
    const Shader shaderSingleColor{"shaders/3dProj.vert", "shaders/shaderSingleColor.frag"};
    const Model  ourModel{"models/backpack/backpack.obj"};
    // const Model ourModel{"models/suzanne/suzanne.obj"};

    [[maybe_unused]] const Shader pointLightshader{"shaders/3dProj.vert", "shaders/PointLight.frag"};
    [[maybe_unused]] SphereModel  lightSphere{};

    camera.set_movement_speed(camera.movement_speed() * 2);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // note: Main loop

    Logger::Get().InfoS("Started application.");


    while (!glfwWindowShouldClose(window))
    {
        f32 currentFrame = (f32) glfwGetTime();
        deltaTime        = currentFrame - lastFrame;
        lastFrame        = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glm::mat4 view       = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom()), (f32) SRC_WIDTH / (f32) SRC_HEIGTH, 0.1f, 100.0f);

        shaderSingleColor.use();
        shaderSingleColor.setMat4("view", view);
        shaderSingleColor.setMat4("projection", projection);

        ourShader.use();
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);
        ourShader.setVec3("viewPos", camera.position());
        ourShader.setVec3("dirLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
        ourShader.setVec3("dirLight.diffuse", glm::vec3(1.0f));
        ourShader.setVec3("dirLight.ambient", glm::vec3(0.2f));
        ourShader.setVec3("dirLight.specular", glm::vec3(2.0f));
        ourShader.setFloat("material.shininess", 32.0f);


        auto backpackModel = glm::mat4(1.0f);
        backpackModel      = glm::translate(backpackModel, glm::vec3(0.0f, 0.0f, 0.0f));
        backpackModel      = glm::scale(backpackModel, glm::vec3(1.0f, 1.0f, 1.0f));
        ourShader.setMat4("model", backpackModel);

        shaderSingleColor.use();
        f32 scale = 1.015f;

        auto backpackModelOutline = glm::mat4(1.0f);
        backpackModelOutline      = glm::translate(backpackModelOutline, glm::vec3(0.0f, 0.0f, 0.0f));
        backpackModelOutline      = glm::scale(backpackModelOutline, glm::vec3(scale, scale, scale));
        shaderSingleColor.setMat4("model", backpackModelOutline);

        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        ourModel.Draw(ourShader);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        ourModel.Draw(shaderSingleColor);

        glEnable(GL_DEPTH_TEST);
        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    Logger::Get().InfoS("Application terminated.");
    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow *window, f64 xpos, f64 ypos)
{
    if (firstMouse)
    {
        lastX      = (f32) xpos;
        lastY      = (f32) ypos;
        firstMouse = false;
    }

    const f32 xOffset = (f32) (xpos - lastX);
    const f32 yOffset = (f32) (lastY - ypos);
    lastX             = (f32) xpos;
    lastY             = (f32) ypos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void processInput(GLFWwindow *window)
{


    static bool pausePressedLastFrame = false;

    bool pausePressedNow = glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

    if (pausePressedNow && !pausePressedLastFrame)
    {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
        {
            camera.set_frozen(false);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else
        {
            camera.set_frozen(true);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }

    pausePressedLastFrame = pausePressedNow;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
}
void scroll_callback(GLFWwindow *window, f64 xoffset, f64 yoffset)
{
    camera.ProcessMouseScroll((f32) yoffset);
}

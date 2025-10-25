#include <glad/glad.h>

#include <iomanip>

#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Types.h"

#include <stb/stb_image.h>

#include <Logger.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"


#include "Model.h"


void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void processInput(GLFWwindow *window);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
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

    const Shader ourShader{"shaders/3dProj.vert", "shaders/Lighting.frag"};
    const Model  ourModel{"models/backpack/backpack.obj"};

    camera.MovementSpeed *= 2;
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // note: Main loop

    Logger::Get().InfoS("Started application.");

    while (!glfwWindowShouldClose(window))
    {
        f32 currentFrame = (f32) glfwGetTime();
        deltaTime        = currentFrame - lastFrame;
        lastFrame        = currentFrame;

        processInput(window);

        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SRC_WIDTH / (float) SRC_HEIGTH, 0.1f, 100.0f);
        glm::mat4 view       = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        auto model = glm::mat4(1.0f);
        model      = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model      = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourShader.setVec3("dirLight.direction", glm::vec3(0.0f, -1.0f, 0.0f));
        ourShader.setVec3("dirLight.diffuse", glm::vec3(1.0f));
        ourShader.setVec3("dirLight.ambient", glm::vec3(0.2f));
        ourShader.setVec3("dirLight.specular", glm::vec3(1.0f));

        ourModel.Draw(ourShader);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    Logger::Get().InfoS("Application terminated.");
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

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((f32) yoffset);
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

#pragma once
#include <glad/glad.h>

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"

#include "Types.h"

enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

constexpr f32 YAW         = -90.0f;
constexpr f32 PITCH       = 0.0f;
constexpr f32 SPEED       = 2.5f;
constexpr f32 SENSITIVITY = 0.1f;
constexpr f32 ZOOM        = 45.0f;


class Camera
{
public:
    explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    [[nodiscard]] glm::mat4 GetViewMatrix() const;
    void                    ProcessKeyboard(Camera_Movement direction, float deltaTime);
    void                    ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
    void                    ProcessMouseScroll(float yoffset);

    [[nodiscard]] glm::vec3 position() const;
    [[nodiscard]] glm::vec3 front() const;
    [[nodiscard]] glm::vec3 up() const;
    [[nodiscard]] glm::vec3 right() const;
    [[nodiscard]] glm::vec3 world_up() const;
    [[nodiscard]] float     yaw() const;
    [[nodiscard]] float     pitch() const;
    [[nodiscard]] float     movement_speed() const;
    void                    set_movement_speed(float movement_speed);
    [[nodiscard]] float     mouse_sensitivity() const;
    [[nodiscard]] float     zoom() const;
    [[nodiscard]] bool      frozen() const;
    void                    set_frozen(bool frozen);

private:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    bool Frozen;

    void updateCameraVectors();
};

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

enum Camera_Movement_ {
  FORWARD_,
  BACKWAR_,
  LEFT_,
  RIGHT_,
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Camera
{
 public:
  // camera Attributes
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // euler Angles
  float Yaw;
  float Pitch;
  // camera options
  float MovementSpeed;
  float MouseSensitivity;
  float Zoom;

  // Projection parameters
  float fov_;        // Field of View
  float nearPlane_;  // Near clipping plane
  float farPlane_;   // Far clipping plane
  glm::mat4 projectionMatrix_;  // Projection matrix

  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM),
        fov_(ZOOM), nearPlane_(0.1f), farPlane_(100.0f)
  {
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
    UpdateProjectionMatrix(45.0f);  // Initial projection matrix (assumes 45 degrees fov, aspect ratio will be updated later)
  }

  // returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 GetViewMatrix()
  {
    return glm::lookAt(Position, Position + Front, Up);
  }

  glm::mat4 GetProjectionMatrix() const
  {
    return projectionMatrix_;
  }

  // Update projection matrix on resize
  void UpdateProjectionMatrix(float aspectRatio)
  {
    // Create a perspective projection matrix based on the field of view and aspect ratio
    projectionMatrix_ = glm::perspective(glm::radians(Zoom), aspectRatio, nearPlane_, farPlane_);
  }

  void ProcessKeyboard(Camera_Movement_ direction, float deltaTime)
  {
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD_)
      Position += Front * velocity;
    if (direction == BACKWAR_)
      Position -= Front * velocity;
    if (direction == LEFT_)
      Position -= Right * velocity;
    if (direction == RIGHT_)
      Position += Right * velocity;
  }

  void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
  {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw   += xoffset;
    Pitch += yoffset;

    if (constrainPitch)
    {
      if (Pitch > 89.0f)
        Pitch = 89.0f;
      if (Pitch < -89.0f)
        Pitch = -89.0f;
    }

    updateCameraVectors();
  }

  void ProcessMouseScroll(float yoffset)
  {
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
      Zoom = 1.0f;
    if (Zoom > 45.0f)
      Zoom = 45.0f;
  }

 private:

  void updateCameraVectors()
  {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
  }

};
#endif

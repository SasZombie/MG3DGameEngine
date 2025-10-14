#include "camera.hpp"
#include <ostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera::Camera() noexcept
    : cameraPosition({0.f, 0.f, 100.f}), cameraViewDirection({0.f, 0.f, -1.f}), cameraUp({0.f, 1.f, 0.f}),
      cameraRight(glm::cross(cameraViewDirection, cameraUp))
{
}

Camera::Camera(const glm::vec3 &newCameraPosition) noexcept
    : cameraPosition(newCameraPosition), cameraViewDirection({0.f, 0.f, -1.f}), cameraUp({0.f, 1.f, 0.f}),
      cameraRight(glm::cross(cameraViewDirection, cameraUp))
{
}

Camera::Camera(const glm::vec3& newCameraPosition, const glm::vec3& newCameraViewDirection, const glm::vec3& newCameraUp) noexcept
    : cameraPosition(newCameraPosition), cameraViewDirection(newCameraViewDirection), cameraUp(newCameraUp),
      cameraRight(glm::cross(cameraViewDirection, cameraUp))
{
}

void Camera::keyboardMoveFront(float cameraSpeed)
{
    cameraPosition.x += cameraViewDirection.x * cameraSpeed;
    cameraPosition.z += cameraViewDirection.z * cameraSpeed;
}

void Camera::keyboardMoveBack(float cameraSpeed)
{
    cameraPosition.x -= cameraViewDirection.x * cameraSpeed;
    cameraPosition.z -= cameraViewDirection.z * cameraSpeed;
}

void Camera::keyboardMoveLeft(float cameraSpeed)
{
    cameraPosition.x -= cameraRight.x * cameraSpeed;
    cameraPosition.z -= cameraRight.z * cameraSpeed;
}

void Camera::keyboardMoveRight(float cameraSpeed)
{
    cameraPosition.x += cameraRight.x * cameraSpeed;
    cameraPosition.z += cameraRight.z * cameraSpeed;
}

glm::vec3 Camera::getCameraRight() const noexcept
{
    return cameraRight;
}

void Camera::move(const glm::vec3 &newPos) noexcept
{
    cameraPosition += newPos;
    // hitbox.sphereHitbox.center = cameraPosition;
    // hitbox.sphereHitbox.radius = 100.f;
}

void Camera::keyboardMoveUp(float cameraSpeed)
{
    cameraPosition += cameraUp * cameraSpeed;
}

void Camera::keyboardMoveDown(float cameraSpeed)
{
    cameraPosition -= cameraUp * cameraSpeed;
}

void Camera::setCameraViewDirection(const glm::vec3 &newDir) noexcept
{
    this->cameraViewDirection = newDir;
    this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

void Camera::uppdate(const Camera *camera) noexcept
{   
    //Dont need the pointer to self
    (void)camera;

    frust.update(getProjectionMatrix(), getViewMatrix());
}

float Camera::getCameraHeight() const noexcept
{
    return cameraHeight;
}

void Camera::setCameraPosition(const glm::vec3 &newDir) noexcept
{
    this->cameraPosition = newDir;
}

void Camera::setCameraX(const float poz) noexcept
{
    this->cameraPosition.x = poz;
}

void Camera::setCameraY(const float poz) noexcept
{
    this->cameraPosition.y = poz;
}

void Camera::setCameraZ(const float poz) noexcept
{
    this->cameraPosition.z = poz;
}

glm::mat4 Camera::getViewMatrix() const noexcept
{
    return glm::lookAt(cameraPosition, cameraPosition + cameraViewDirection, cameraUp);
}

glm::mat4 Camera::getProjectionMatrix() const noexcept
{
    //Hardcoded for now, maybe I will adapt it
    return glm::perspective(glm::radians(90.f), 16.f / 9.f, 1.f, 100.f);
}

glm::vec3 Camera::getCameraPosition() const noexcept
{
    return cameraPosition;
}

glm::vec3 Camera::getCameraViewDirection() const noexcept
{
    return cameraViewDirection;
}

glm::vec3 Camera::getCameraUp() const noexcept
{
    return cameraUp;
}

std::ostream &operator<<(std::ostream &os, const Camera &obj)
{
    os << "Camera Position(x = " << obj.cameraPosition.x << ", y = " << obj.cameraPosition.y << ", z = " << obj.cameraPosition.z << ")";
    return os;
}
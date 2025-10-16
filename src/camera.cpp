#include "camera.hpp"
#include <ostream>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Asset.hpp"

Camera::Camera() noexcept
    : cameraViewDirection({0.f, 0.f, -1.f}), cameraUp({0.f, 1.f, 0.f}),
      cameraRight(glm::cross(cameraViewDirection, cameraUp))
{
    localTransform.position = {0.f, 0.f, 0.f};
}

Camera::Camera(const glm::vec3 &newCameraPosition) noexcept
    : cameraViewDirection({0.f, 0.f, -1.f}), cameraUp({0.f, 1.f, 0.f}),
      cameraRight(glm::cross(cameraViewDirection, cameraUp))
{
    localTransform.position = newCameraPosition;
}

Camera::Camera(const glm::vec3 &newCameraPosition, const glm::vec3 &newCameraViewDirection, const glm::vec3 &newCameraUp) noexcept
    : cameraViewDirection(newCameraViewDirection), cameraUp(newCameraUp),
      cameraRight(glm::cross(cameraViewDirection, cameraUp))
{
    localTransform.position = newCameraPosition;
}

void Camera::keyboardMoveFront(float cameraSpeed)
{
    localTransform.position.x += cameraViewDirection.x * cameraSpeed;
    localTransform.position.z += cameraViewDirection.z * cameraSpeed;
}

void Camera::keyboardMoveBack(float cameraSpeed)
{
    localTransform.position.x -= cameraViewDirection.x * cameraSpeed;
    localTransform.position.z -= cameraViewDirection.z * cameraSpeed;
}

void Camera::keyboardMoveLeft(float cameraSpeed)
{
    localTransform.position.x -= cameraRight.x * cameraSpeed;
    localTransform.position.z -= cameraRight.z * cameraSpeed;
}

void Camera::keyboardMoveRight(float cameraSpeed)
{
    localTransform.position.x += cameraRight.x * cameraSpeed;
    localTransform.position.z += cameraRight.z * cameraSpeed;
}

glm::vec3 Camera::getCameraRight() const noexcept
{
    return cameraRight;
}

void Camera::move(const glm::vec3 &newPos) noexcept
{
    localTransform.position += newPos;
}

void Camera::keyboardMoveUp(float cameraSpeed)
{
    localTransform.position += cameraUp * cameraSpeed;
}

void Camera::keyboardMoveDown(float cameraSpeed)
{
    localTransform.position -= cameraUp * cameraSpeed;
}

void Camera::setCameraViewDirection(const glm::vec3 &newDir) noexcept
{
    this->cameraViewDirection = newDir;
    this->cameraRight = glm::cross(cameraViewDirection, cameraUp);
}

void Camera::uppdate(const Camera *camera) noexcept
{
    (void)camera;

     if (auto p = parent.lock()) 
        uppdateWorldTransformCamera(p->worldTransform);
    else 
        uppdateWorldTransformCamera(sas::Transform{});  // Root camera

    // uppdateWorldTransformCamera(parent.lock()->worldTransform);
    SceneNode::uppdateAttachedToCamera(camera);
}

float Camera::getCameraHeight() const noexcept
{
    return cameraHeight;
}

void Camera::setCameraPosition(const glm::vec3 &newDir) noexcept
{
    this->localTransform.position = newDir;
}

void Camera::setCameraX(const float poz) noexcept
{
    this->localTransform.position.x = poz;
}

void Camera::setCameraY(const float poz) noexcept
{
    this->localTransform.position.y = poz;
}

void Camera::setCameraZ(const float poz) noexcept
{
    this->localTransform.position.z = poz;
}

glm::mat4 Camera::getViewMatrix() const noexcept
{
    return glm::lookAt(localTransform.position, localTransform.position + cameraViewDirection, cameraUp);
}

glm::mat4 Camera::getProjectionMatrix() const noexcept
{
    // Hardcoded for now, maybe I will adapt it
    return glm::perspective(glm::radians(90.f), 16.f / 9.f, 1.f, 100.f);
}

glm::vec3 Camera::getCameraPosition() const noexcept
{
    return localTransform.position;
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
    os << "Camera Position(x = " << obj.localTransform.position.x << ", y = " << obj.localTransform.position.y << ", z = " << obj.localTransform.position.z << ")";
    return os;
}
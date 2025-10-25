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

sas::CameraSettings Camera::getCameraSettings() const noexcept
{
    return cameraSettings;
}

void Camera::uppdate(const Camera *camera) noexcept
{
    (void)camera;
    SceneNode::uppdateAttachedToCamera(camera);
}

void Camera::save(std::ofstream &out) noexcept
{
    out << sas::SerializeCodes::CAMERA << '\n' << cameraSettings << *this;

    SceneNode::save(out);

    out << sas::SerializeCodes::END << '\n';

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

// std::ostream &operator<<(std::ostream &os, const Camera &obj)
// {
//     os << "Camera Position(x = " << obj.localTransform.position.x << ", y = " << obj.localTransform.position.y << ", z = " << obj.localTransform.position.z << ")\n"
//         << "Camera Up(x = " << obj.cameraUp.x << ", y = " << obj.cameraUp.y << ", z = " << obj.cameraUp.z << ")\n"
//         << "Camera Right(x = " << obj.cameraRight.x << ", y = " << obj.cameraRight.y << ", z = " << obj.cameraRight.z << ")\n"
//         << "Camera View Direction " <<  obj.cameraViewDirection.x << ", y = " << obj.cameraViewDirection.y << ", z = " << obj.cameraViewDirection.z << ")\n";
//     return os;
// }

std::ostream &operator<<(std::ostream &os, const Camera &obj)
{
    os << obj.cameraUp.x << ' ' << obj.cameraUp.y << ' ' << obj.cameraUp.z << '\n'
       << obj.cameraRight.x << ' ' << obj.cameraRight.y << ' ' << obj.cameraRight.z << '\n'
       << obj.cameraViewDirection.x << ' ' << obj.cameraViewDirection.y << ' ' << obj.cameraViewDirection.z << '\n';
    return os;
}


std::istream &operator>>(std::istream &is, Camera &obj)
{
    is >> obj.cameraSettings >> obj.cameraUp.x >> obj.cameraUp.y >> obj.cameraUp.z 
       >> obj.cameraRight.x >> obj.cameraRight.y >> obj.cameraRight.z 
       >> obj.cameraViewDirection.x >> obj.cameraViewDirection.y >> obj.cameraViewDirection.z 
       >> obj.localTransform >> obj.worldTransform;
    return is;
}
#include "Asset.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <GL/freeglut.h>
#include <SFML/Audio.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma GCC diagnostic pop

#include "Frustum.hpp"

sas::Asset::Asset(Window *nwindow) noexcept
    : window(nwindow)
{
}

sas::Asset::Asset(const Mesh &nmesh, Window *nwindow) noexcept
    : mesh(nmesh), window(nwindow)

{
}

sas::Asset::Asset(const Shader &nshader, const Mesh &nmesh, Window *nwindow) noexcept
    : shader(nshader), mesh(nmesh), window(nwindow)
{
    shader.use();
    setShaderUniformID(glGetUniformLocation(shader.getId(), "MVP"));
}

void sas::Asset::addMesh(const Mesh &nmesh) noexcept
{
    this->mesh = nmesh;
}

void sas::Asset::basicPVM(const Camera *camera) noexcept
{
    ProjectionMatrix = glm::perspective(90.f, window->getWidth() * 1.f / window->getHeight(), 0.1f, 10000.f);
    ViewMatrix = glm::lookAt(camera->getCameraPosition(), camera->getCameraPosition() + camera->getCameraViewDirection(), camera->getCameraUp());
}

void sas::Asset::draw(const Camera *camera) noexcept
{
    shader.use();

    basicPVM(camera);

    MVP = ProjectionMatrix * ViewMatrix * worldTransform.getModelMatrix();
    glUniformMatrix4fv(uniformShaderID, 1, GL_FALSE, &MVP[0][0]);
    // Dont bake if statements in release
#ifdef debugMode
    if (!mesh)
    {
        std::cerr << "Warning! Drawing without a mesh!\n";
        return;
    }
#endif

    mesh->draw(shader);
}

void sas::Asset::drawAttachedToCamera(const Camera *camera) noexcept
{
    shader.use();

    MVP = camera->getProjectionMatrix() * localTransform.getModelMatrix();

    glUniformMatrix4fv(uniformShaderID, 1, GL_FALSE, &MVP[0][0]);

    // Dont bake if statements in release
#ifdef debugMode
    if (!mesh)
    {
        std::cerr << "Warning! Drawing without a mesh!\n";
        return;
    }
#endif

    mesh->draw(shader);
}

void sas::Asset::uppdateAttachedToCamera(const Camera *camera) noexcept
{
    drawAttachedToCamera(camera);

    if (collisionObject)
    {
        collisionObject->uppdateBounds(worldTransform);
    }

    SceneNode::uppdateAttachedToCamera(camera);
}

void sas::Asset::addCollisionObject(CollisionObject *colObj) noexcept
{
    collisionObject = colObj;
}

sas::CollisionObject *sas::Asset::getCollisionObject() const noexcept
{
#ifdef debugMode

    if (!collisionObject)
    {
        std::cerr << "Warning! Calling getCollisionObject without an collision object\n";
        return {};
    }
#endif
    return collisionObject;
}

void sas::Asset::uppdate(const Camera *camera) noexcept
{
    for (auto &cb : callbacks)
    {
        cb();
    }
    draw(camera);

    // This is necesarry to not dereference nullptr
    if (collisionObject)
    {
        collisionObject->uppdateBounds(worldTransform);
    }

    SceneNode::uppdate(camera);
}

void sas::Asset::translate(const glm::vec3 &newPosition) noexcept
{
    localTransform.position = newPosition;
}

void sas::Asset::scale(const glm::vec3 &newScaleVector) noexcept
{
    localTransform.scale = newScaleVector;
}

void sas::Asset::rotate(const glm::vec3 &axisVector) noexcept
{
    localTransform.rotation = axisVector;
}

void sas::Asset::setShader(const Shader &nshader) noexcept
{
    this->shader = nshader;
}

void sas::Asset::setShaderUniformID(int id) noexcept
{
    this->uniformShaderID = id;
}

void sas::Asset::addCallback(Callback cb) noexcept
{
    callbacks.push_back(std::move(cb));
}

void sas::Asset::addCollisionCallback(Callback cb) noexcept
{
    collisionCallbacks.push_back(std::move(cb));
}


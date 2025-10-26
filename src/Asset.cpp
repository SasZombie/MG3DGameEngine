#include "Asset.hpp"
#include "ScriptTransform.hpp"
#include "ScriptInstance.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"

#include <GL/freeglut.h>
#include <SFML/Audio.hpp>
#include <glm/gtc/matrix_transform.hpp>

#pragma GCC diagnostic pop

sas::Asset::Asset(Window *nwindow) noexcept
    : window(nwindow)
{
}

sas::Asset::Asset(std::shared_ptr<Mesh> nmesh, Window *nwindow) noexcept
    : mesh(nmesh), window(nwindow)

{
}

sas::Asset::Asset(std::shared_ptr<Shader> nshader, std::shared_ptr<Mesh> nmesh, Window *nwindow) noexcept
    : shader(nshader), mesh(nmesh), window(nwindow)
{
    shader->use();
    setShaderUniformID(glGetUniformLocation(shader->getId(), "MVP"));
}

void sas::Asset::addMesh(std::shared_ptr<Mesh> nmesh) noexcept
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
    shader->use();

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

    mesh->draw(*shader);
}

void sas::Asset::drawAttachedToCamera(const Camera *camera) noexcept
{
    shader->use();

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

    mesh->draw(*shader);
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

void sas::Asset::save(std::ofstream &out) noexcept
{
    out << SerializeCodes::ASSET << '\n'
        << *shader << *mesh;

    SceneNode::save(out);

    out << SerializeCodes::END << '\n';
}

void sas::Asset::addCollisionObject(CollisionObject *colObj) noexcept
{
    collisionObject = colObj;
}

bool sas::Asset::hasCollisionObject() const noexcept
{
    return !(collisionObject == nullptr);
}

sas::CollisionObject *sas::Asset::getCollisionObject() const noexcept
{
#ifdef debugMode

    // if (!collisionObject)
    // {
    //     std::cerr << "Warning! Calling getCollisionObject without an collision object\n";
    //     return {};
    // }
#endif
    return collisionObject;
}

void sas::Asset::uppdate(const Camera *camera) noexcept
{
    for (auto &cb : callbacks)
    {
        cb(*this);
    }
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

void sas::Asset::setShader(std::shared_ptr<Shader> nshader) noexcept
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

void sas::Asset::addCallback(const std::filesystem::path &scriptPath) noexcept
{
    const auto &tokens = getAllTokens(scriptPath);

    const std::string fileNoExt = scriptPath.stem().string();

    const std::string filePath = "ScriptsGen/" + fileNoExt + ".cpp";
    generateCPPClass(tokens, filePath, fileNoExt);
    
    const std::string dllFile = script::createDLL(filePath, fileNoExt);

    auto scriptInstance = std::make_shared<script::ScriptInstance>();

    if (!scriptInstance->load(dllFile))
    {
        std::cerr << "Failed to load script " << dllFile << '\n';
        return;
    }

    callbacks.emplace_back([scriptInstance](sas::Asset& self){
        scriptInstance->callUpdate(self);
    });
}

void sas::Asset::onCollision(const Signal<Asset &, Asset &>::collsionCallback &cb) noexcept
{
    this->signals.connect(cb);
}

void sas::Asset::emit(Asset &other) noexcept
{
    signals.emit(*this, other);
}

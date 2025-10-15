///@brief
/// Read me [1]

#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
#pragma GCC diagnostic ignored "-Wshadow"


#include <glm/glm.hpp>

#pragma GCC diagnostic pop

#include "SceneNode.hpp"
#include "Frustum.hpp"

namespace sas
{
	class Asset;
}

class Camera : public sas::SceneNode
{

	private:
		float cameraHeight = -10.f;

		glm::vec3 cameraPosition;
		glm::vec3 cameraViewDirection;
		glm::vec3 cameraUp;
		glm::vec3 cameraRight;

	public:

		Frustum frust;
		Camera() noexcept;
		Camera(const glm::vec3& cameraPosition) noexcept;
		Camera(const glm::vec3& cameraPosition, const glm::vec3& cameraViewDirection, const glm::vec3& cameraUp) noexcept;
		~Camera() = default;

		glm::vec3 getCameraUp() const noexcept;
		glm::mat4 getViewMatrix() const noexcept;
		glm::mat4 getProjectionMatrix() const noexcept;
		glm::vec3 getCameraPosition() const noexcept;
		glm::vec3 getCameraViewDirection() const noexcept;
		glm::vec3 getCameraRight() const noexcept;

		void move(const glm::vec3& newPos) noexcept;
		
		void keyboardMoveUp(float cameraSpeed);
		void keyboardMoveDown(float cameraSpeed);

		//Seemed like a good idea until I realized that I have speed up on diagonal movement and I dont really have
		//as much controll as I want
		[[deprecated("Use move for better controll")]]
		void keyboardMoveBack(float cameraSpeed);
		
		[[deprecated("Use move for better controll")]]
		void keyboardMoveLeft(float cameraSpeed);
		
		[[deprecated("Use move for better controll")]]
		void keyboardMoveFront(float cameraSpeed);
		
		[[deprecated("Use move for better controll")]]
		void keyboardMoveRight(float cameraSpeed);

		void setCameraX(const float poz) noexcept;
		void setCameraY(const float poz) noexcept;
		void setCameraZ(const float poz) noexcept;
		void setCameraPosition(const glm::vec3 &newDir) noexcept;
		void setCameraViewDirection(const glm::vec3 &newDir) noexcept;

		void uppdate(const Camera* camera) noexcept override;
		float getCameraHeight() const noexcept;

		
		friend std::ostream& operator<<(std::ostream& os, const Camera& obj);
};


#pragma once
/*!
\file CameraComponent.h
*/

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "Component.h"

#define GLM_FORCE_RADIANS
#define ROTATION_VALUE 0.001


/*!
\class CameraComponent
\brief Manages and generates all camera related data.
*/
class CameraComponent : public Component
{
protected:
	//
	glm::vec3 m_position; //!< glm::vec3 position, stores position.
	glm::quat m_orientation; //!< glm::quat Variable orientation, stores orientation.	
	float m_fov; //!< Float field of view variable.
public:

	
	//! Quaternion calculation interface.
	/*!
	\param axis the axis to rotate about.
	\param angle the angle to rotate around.
	*/
	virtual glm::quat quaternion(glm::vec3 axis, float angle) = 0;

	//! QuatCameraRotation interface.
	/*!
	This function uses reverse order of the multiplications to imitate the constraints of one axis
	i.e. having control of one axis does not make the camera convoluted after pitching and yawing.
	\param pitch left and right rotations.
	\param yaw up and down rotations.
	*/
	virtual void cameraQuatRotate(float pitch, float yaw) = 0;

	//! Virtual Update Function.
	/*!
	\param dt, float for delta time.
	*/
	virtual void OnUpdate(float dt){}

	//! Virtual OnMessage Function Override.
	/*!
	\param m const string which takes a message from components.
	*/
	virtual void OnMessage(const std::string m){}

	//! Default Constructor.
	/*!
	*/
	CameraComponent() : m_position(0), m_orientation(1, 0, 0, 0), m_fov(45) {}
	//! Constructor.
	/*!
	\param pos the position.
	*/
	CameraComponent(glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0), m_fov(45) {}

	//!Copy Constructor.
	/*!
	\param pos the position.
	\param orient the orientation.
	*/
	CameraComponent(glm::vec3& pos,glm::quat& orient) : m_position(pos), m_orientation(orient), m_fov(45) {}

	//! lookAt Function.
	/*!
	\param target target to look at.
	*/
	void lookAt(glm::vec3& target)  { m_orientation = (glm::toQuat(glm::lookAt(m_position, target, glm::vec3(0, 1, 0)))); }

	//! Position getter.
	/*!
	\returns the position.
	*/
	inline glm::vec3 getPosition(){ return m_position; }

	//! Orientation getter.
	/*!
	\returns m_orientation.
	*/
	inline glm::quat getOrientation(){ return m_orientation; }

	//! View matrix Getter Function.
	/*!
	Gets returns viewmatrix.
	*/
	inline glm::mat4 getViewMatrix(){ return glm::translate(glm::mat4_cast(glm::inverse(m_orientation)), -m_position); }

	//! Setter for glm::vec3 m_position.
	/*!
	\param newPosition the new position
	*/
	inline void setPosition(glm::vec3 newPosition){m_position = newPosition;}

	//! Setter for glm::vec3 m_orientation.
	/*!
	\param newOrientation the new orientation
	*/
	inline void setOrientation(glm::quat newOrientation){m_orientation = newOrientation;}

	//! FOV setter Function.
	/*!
	\param angle The new angle as a float
	*/
	inline void setFOV(float fov) { m_fov = fov; }

	//! FOV geetter Function.
	/*!
	\returns float m_fov
	*/
	inline float getFOV() { return m_fov;  }

};
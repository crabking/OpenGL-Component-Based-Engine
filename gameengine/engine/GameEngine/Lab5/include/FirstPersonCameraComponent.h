#pragma once
/*!
\file FirstPersonCameraComponent.h
*/
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "CameraComponent.h"
#include "DebugLog.h"


/*!
\class FirstPersonCameraComponent
\brief Manages camera inputs, rotations and translation.
*/
class FirstPersonCameraComponent : public CameraComponent
{
public:

	//! Update Function override.
	/*!
	\param dt, float for delta time.
	*/
	void OnUpdate(float dt) override {}

	//! OnMessage Function Override.
	/*!
	\param m const string which takes a message from components.
	*/
	void OnMessage(const std::string m) override{}

	//! Default Constructor.
	/*!
	*/
	FirstPersonCameraComponent()  { m_position = glm::vec3(0); m_orientation = glm::quat(1, 0, 0, 0); }

	//! Overloaded Constructor.
	/*!
	\param pos the position.
	*/
	FirstPersonCameraComponent(glm::vec3& pos)  { m_position = pos; }

	//! Overloaded Constructor.
	/*!
	\param pos the position.
	\param orient the orientation.
	*/
	FirstPersonCameraComponent(glm::vec3& pos,glm::quat& orient) { m_position = pos; m_orientation = orient; }

	//! Quaternion calculation.
	/*!
	\param axis the axis to rotate about.
	\param angle the angle to rotate around.
	*/
	glm::quat quaternion(glm::vec3 axis, float angle) override
	{
		glm::quat l_rotation;

		l_rotation.w = cos(angle / 2);
		l_rotation.x = sin(angle / 2) * axis.x;
		l_rotation.y = sin(angle/ 2) * axis.y;
		l_rotation.z = 0; //! constraining z to remove unwanted roll

		return l_rotation;
	}

	//! QuatCameraRotation.
	/*!
	This function uses reverse order of the multiplications to imitate the constraints of one axis
	i.e. having control of one axis does not make the camera convoluted after pitching and yawing.
	\param pitch left and right rotations.
	\param yaw up and down rotations.
	*/
	void cameraQuatRotate(float pitch, float yaw) override
	{
		m_orientation = (glm::normalize(quaternion(glm::vec3(0.0f,1.0f,0.0f),  yaw) * m_orientation ));
		m_orientation = (m_orientation * (glm::normalize(quaternion(glm::vec3(1.0f, 0.0f, 0.0f), pitch)  )));
	}

};
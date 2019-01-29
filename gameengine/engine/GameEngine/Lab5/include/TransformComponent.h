#pragma once
/*!
\file TransformComponent.h
*/
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>

/*!
\class TransformComponent
\brief Manages and handles transformations that the component is attatched to.
*/



class TransformComponent : public Component
{
protected:
	float m_fov; //!< Float field of view variable.

	glm::vec3 m_position; //!< glm::vec3 for setting position.

	glm::quat m_orientation; //!< glm::quat Variable for setting orientation.

	glm::vec3 m_scale; //!< glm::vec3 Variable for setting scale.

public:

	//! Update Function override.
	/*!
	\param dt, float for delta time.
	*/
	void OnUpdate(float dt) override{}

	//! OnMessage Function Override.
	/*!
	\param m const string which takes a message from components.
	*/
	void OnMessage(const std::string m) override{}

	//! Default Constructor.
	/*!
	*/
	TransformComponent()  { m_position = glm::vec3(0); m_orientation = glm::quat(1, 0, 0, 0); m_scale = glm::vec3(1.0f); }

	//! Overloaded Constructor.
	/*!
	\param pos the position.
	*/
	TransformComponent(glm::vec3& pos)  { m_position = pos; }

	//! Overloaded Constructor.
	/*!
	\param pos the position.
	\param orient the orientation.
	*/
	TransformComponent(glm::vec3& pos, glm::quat& orient)  { m_position = pos; m_orientation = orient; }

	//! Overloaded Constructor.
	/*!
	\param pos the position.
	\param orient the orientation.
	\param scale
	*/
	TransformComponent(glm::vec3& pos, glm::quat& orient, glm::vec3& scale)  { m_position = pos; m_orientation = orient; m_scale = scale; }

	//! lookAt Function.
	/*!
	\param target target to look at.
	*/
	inline void lookAt(const glm::vec3& target) { m_orientation = (glm::toQuat(glm::lookAt(m_position, target, glm::vec3(0, 1, 0)))); }

	//! position Getter .
	/*!
	\returns m_position
	*/
	inline glm::vec3 getPosition(){return m_position;}


	//! Orientation getter.
	/*!
	\returns m_orientation.
	*/
	inline glm::quat getOrientation(){return m_orientation;}

	//! Scale getter.
	/*!
	\returns m_scale.
	*/
	inline glm::vec3 getScale() { return m_scale; }


	//! View matrix Getter Function.
	/*!
	Gets returns viewmatrix.
	*/
	inline glm::mat4 getViewMatrix() { return glm::translate(glm::mat4_cast(glm::inverse(m_orientation)), -m_position); }

	//! Gets the modelmatrix.
	/*!
	\returns transMatrix * rotMatrix * scaleMatrix
	*/
	glm::mat4 getModelMatrix()
	{
		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_position);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
		glm::mat4 rotMatrix = glm::mat4_cast(m_orientation);
		return transMatrix * rotMatrix * scaleMatrix;
	}


	//! Quaternion calculation.
	/*!
	\param axis the axis to rotate about.
	\param angle the angle to rotate around.
	*/
	glm::quat quaternion(glm::vec3 axis, float angle)
	{
		glm::quat l_rotation;

		l_rotation.w = cos(angle / 2);
		l_rotation.x = sin(angle / 2) * axis.x;
		l_rotation.y = sin(angle / 2) * axis.y;
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
	void cameraQuatRotate(float pitch, float yaw) 
	{
		m_orientation = (glm::normalize(quaternion(glm::vec3(0.0f, 1.0f, 0.0f), yaw) * m_orientation));
		m_orientation = (m_orientation * (glm::normalize(quaternion(glm::vec3(1.0f, 0.0f, 0.0f), pitch))));

	}
	//! Translate Function.
	/*!
	translation in a vec3 axis, x,y,z
	\param v
	*/
	inline void translate(const glm::vec3 &v) { m_position +=  v; }
	inline void translate(float x, float y, float z) { m_position += glm::vec3(x, y, z) * m_orientation; }

	//! Rotation Function.
	/*!
	rotates around an angle and about an axis
	\param angle
	\param axis
	*/
	inline void rotate(float angle, const glm::vec3 &axis) { m_orientation = m_orientation * (glm::angleAxis(angle, axis * m_orientation)); }
	inline void rotate(float angle, float x, float y, float z) { m_orientation = m_orientation * glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation); }

	//! Scale Function.
	/*!
	\param v
	*/
	inline void scaleUp(const glm::vec3 &v) { m_scale += v; }
	inline void scaleUp(float x, float y, float z) { m_scale += glm::vec3(x, y, z); }

	//! yaw Function. 
	/*!
	\param angle The new angle as a float
	*/
	inline void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	//! pitch Function.
	/*!
	\param angle The new angle as a float
	*/
	inline void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	//! roll Function.
	/*!
	\param angle The new angle as a float
	*/
	inline void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }

	//! FOV setter Function.
	/*!
	\param angle The new angle as a float
	*/
	inline void setFOV(float fov) { m_fov = fov; }



};

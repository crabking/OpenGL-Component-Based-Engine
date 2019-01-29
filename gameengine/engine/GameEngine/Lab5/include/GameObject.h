#pragma once
/*!
\file GameObject.h
*/
#include "Component.h"
#include "CameraComponent.h"
#include "FirstPersonCameraComponent.h"
#include "ThirdPersonCameraComponent.h"
#include "TransformComponent.h"
#include <unordered_map>
#include <typeindex>


/*!
\class GameObject
\brief handles game objects and adding components to them.
*/
class GameObject : public Component
{

private:

	std::unordered_map<std::type_index, Component*> m_components; //!< map of components.
	glm::vec3 relativePosition;

public:
	bool firstPerson = true; //!< Boolean for camera.

	//! GetComponent Template.
	/*!
	Gets components from game object.
	*/
	template <typename T>
	T* getComponent()
	{
		auto iter = m_components.find(typeid(T));

		if(iter != std::end(m_components))
		{
			return dynamic_cast<T*>(iter->second); //! if found dynamic cast the component pointer and return it
		}

		return nullptr; //! return null if we don't have a component of that type	
	}

	//! Add component Template.
	/*!
	sets components to game object.
	*/
	template <typename T>
	void addComponent(T* comp)
	{	
		m_components[typeid(T)] = comp; //! add the component to unoreder map with hash of its typeid
	}

	
	//! Update Function override.
	/*!
	\param dt, float for delta time.
	*/
	void OnUpdate(float dt) override{}
	

	//! OnMessage Function Override.
	/*!
	\param m const string which takes a message from components.
	*/
	void OnMessage(const std::string m) override 
	{
		TransformComponent* tc = getComponent<TransformComponent>();
		CameraComponent* cc = getComponent<CameraComponent>();
		FirstPersonCameraComponent* fpc = getComponent<FirstPersonCameraComponent>();
		ThirdPersonCameraComponent* tpc = getComponent<ThirdPersonCameraComponent>();
		
	
		if (m == "rotateLeft")
		{
			tc->yaw(45 * ROTATION_VALUE);
			
			std::cout << "Input: FP Rotate LEFT " << std::endl;
		}
		else if (m == "rotateRight")
		{
			tc->yaw(45 * -ROTATION_VALUE);
			std::cout << "Input: FP Rotate RIGHT " << std::endl;
		}
		else if (m == "rotateUp")
		{
			tc->pitch(45 * ROTATION_VALUE);
			std::cout << "Input: FP Rotate UP " << std::endl;
		}
		else if (m == "rotateDown")
		{
			tc->pitch(45 * -ROTATION_VALUE);
			std::cout << "Input: FP Rotate DOWN " << std::endl;
		}

		else if (m == "moveLeft")
		{
			tc->translate(tc->getOrientation() * glm::vec3(-1.0f, 0.0f, 0.0f));
			std::cout << "Input: FP Translate Left " << std::endl;
		}

		else if (m == "moveRight")
		{
			tc->translate(tc->getOrientation() * glm::vec3(1.0f, 0.0f, 0.0f));
			std::cout << "Input: FP Translate Right " << std::endl;
		}

		else if (m == "moveForward")
		{
			tc->translate(tc->getOrientation() * glm::vec3(0.0f, 0.0f, -1.0f));
			std::cout << "Input: FP Translate Forwards " << std::endl;
		}

		else if (m == "moveBackward")
		{
			tc->translate(tc->getOrientation() * glm::vec3(0.0f, 0.0f, 1.0f));
			std::cout << "Input: FP Translate Backwards " << std::endl;
		}

		else if (m == "setFPcam")
		{
			firstPerson = true;
		}

		else if (m == "setTPcam")
		{
			firstPerson = false;	
		}

		else if (m == "setNextLevel")
		{
			
			
		}

		else if (m == "setPreviousLevel")
		{
			
		}

		glm::quat orientation = tc->getOrientation();
		glm::vec3 position = tc->getPosition();
	
		if (firstPerson)
		{
			relativePosition = glm::vec3(0,0,0);  // in front
			position += (orientation * relativePosition);
			fpc->setPosition(position);
			fpc->setOrientation(orientation);
		}
		if (!firstPerson)
		{
			relativePosition = glm::vec3(0,2,4);      // behind and above
			position += (orientation * relativePosition);
			tpc->setPosition(position);
			tpc->setOrientation(orientation);
		
		}

	}

	//! Getter for Relative Position.
	/*!
	\brief might be needed in other functions.
	*/
	inline glm::vec3 getRelativePos() {return relativePosition;}

};
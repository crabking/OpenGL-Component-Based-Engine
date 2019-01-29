#pragma once
/*!
\file Factory.h
*/

#include "TransformComponent.h"
#include "FirstPersonCameraComponent.h"
#include "ThirdPersonCameraComponent.h"
#include "ModelComponent.h"


/*!
\class Factory
\brief, This design is responsible for defering people from creating components without my permission.
*/
class Factory 
{

	
public:
	
	inline TransformComponent * createTransformComponent(glm::vec3& (pos), glm::quat &(ori), glm::vec3&(scale)) { return new TransformComponent(pos, ori, scale); }
	inline FirstPersonCameraComponent * createFPCameraComponent(glm::vec3& (pos), glm::quat &(ori)) { return new FirstPersonCameraComponent(pos, ori); }
	inline ThirdPersonCameraComponent * createTPCameraComponent(glm::vec3& (pos), glm::quat &(ori)) { return new ThirdPersonCameraComponent(pos, ori); }
	inline ModelComponent * createModelComponent(Model *model) { return new ModelComponent(model); }

};
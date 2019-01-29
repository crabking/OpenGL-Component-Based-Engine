#pragma once
/*!
\file Factory.h
*/


/*!
\class Factory
\brief, Factory design, handles creation of all components here.
*/
class ComponentFactory
{
private:


public:

	enum ComponentType {
		T_TransformComponent, T_FirstPersonCameraComponent, T_ThirdPersonCameraComponent, T_ModelComponent, T_SceneHandlerComponent
	};

	
	static ComponentFactory* Create(ComponentType type);



	/*inline TransformComponent * createTransformComponent(glm::vec3& (pos), glm::quat &(ori), glm::vec3&(scale)) { return new TransformComponent(pos, ori, scale); }
	inline FirstPersonCameraComponent * createFPCameraComponent(glm::vec3& (pos), glm::quat &(ori)) { return new FirstPersonCameraComponent(pos, ori); }
	inline ThirdPersonCameraComponent * createTPCameraComponent(glm::vec3& (pos), glm::quat &(ori)) { return new ThirdPersonCameraComponent(pos, ori); }
	inline ModelComponent * createModelComponent(Model *model) { return new ModelComponent(model); }
	inline SceneHandlerComponent* createSceneHandlerComponent() { return new SceneHandlerComponent; }*/

};

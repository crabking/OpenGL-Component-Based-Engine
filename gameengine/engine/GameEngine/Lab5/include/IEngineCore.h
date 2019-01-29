#pragma once
/*!
\file IEngineCore.h
*/
#include <string>
#include <iostream>
#include <glm/mat4x4.hpp>
#include "Model.h"
#include "ThirdPersonCameraComponent.h"
#include "FirstPersonCameraComponent.h"


class Game; //!< forward declaration of game.

/*!
\class IEngineCore
\brief Interface for OpenGL engine.
*/
class IEngineCore
{
public:

	//! Destructor Interface.
	/*!
	\brief its the destructor interface.
	*/
	virtual ~IEngineCore() {}

	//! Initialize GLFW window Interface.
	/*!
	\brief for initializing window.
	\param int width
	\param int height
	\param string windowName
	*/
	virtual bool initWindow(int width, int height, std::string windowName) = 0;

	//! Core Engine Interface.
	/*!
	\brief for core engine.
	\param Game& game
	*/
	virtual bool runEngine(Game& game) = 0;

	//! Render coloured background function Interface.
	/*!
	\brief for color background..
	\param float r
	\param float g
	\param float b
	*/
	virtual void renderColouredBackground(float r, float g, float b) = 0;

	//! Render text function Interface.
	/*!
	\brief for color background..
	\param string text
	\param float x
	\param float y
	\param float scale
	\param glm::vec3 colour
	*/
	virtual void renderText(std::string text, float x, float y, float scale, glm::vec3 colour) = 0;

	//! Sets third person camera Interface.
	/*!
	\param tpcam ThirdPersonCameraComponent pointer.
	*/
	virtual	void setThirdPersonCamera(ThirdPersonCameraComponent* tpcam) = 0;

	//! Sets first person camera Interface.
	/*!
	\param fpcam FirstPersonCameraComponent pointer.
	*/
	virtual	void setFirstPersonCamera(FirstPersonCameraComponent* fpcam) = 0;

	//! Draws model to screen Interface.
	/*!
	\param model model pointer.
	\param modelMatrix the model matrix.
	*/
	virtual void drawModel(Model* model, glm::mat4& modelMatrix) = 0;
};

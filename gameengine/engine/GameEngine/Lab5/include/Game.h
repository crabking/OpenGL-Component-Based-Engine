#pragma once
/*!
\file Game.h
*/
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "json/json.h"
#include "IEngineCore.h"
#include "InputHandler.h"
#include "GLFW_EngineCore.h"
#include "SceneHandler.h"
#include "Factory.h"


/*!
\class Game
\brief Manages and generates all game related data.
*/
class Game
{

private:

	Factory* factory; //!< Factory pointer.

	std::vector<string> v_jsonLevels; //!< vector of strings.

	SceneHandler* m_sceneHandler; //!< scene handler pointer.
	

public:
	

	//! Game default constructor.
    /*!
    */
	Game();

	//! Game Destructor.
    /*!
    */
	~Game();

	IEngineCore *m_engineInterfacePtr; //!< IEngineCore object pointer.

	InputHandler *m_inputHandler; //!< InputHandler object pointer.
	

	//InputHandler *m_ginputHandler; //!< InputHandler object pointer.

	//! Update function.
	/*!
	*/
	void update();

	//! Render function.
    /*!
    */
	void render();

	//! Get scene function.
	/*!
	*/
	Scene* getCurrentScene() {
		return m_sceneHandler->getCurrentScene();
	}

	//! Get Scene handler function.
	/*!
	*/
	SceneHandler* getSceneHandler() {
		return m_sceneHandler;
	}


};
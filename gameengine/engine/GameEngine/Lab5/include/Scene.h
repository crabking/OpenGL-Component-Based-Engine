#pragma once
/*!
\file Scene.h
*/

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "IEngineCore.h"
#include "InputHandler.h"
#include "GameObject.h"

#include "ModelManager.h"
#include "Factory.h"

/*!
\class Scene
\brief Handles scene logic.
*/
class Scene
{

private:

	GameObject m_playerBackground; //!< Player Background GameObject.

	ModelManager m_modelmanager;  //!< ModelManager Object.

	GameObject m_playerObject; //!< ModelManager Object

	std::vector<GameObject> v_gameObjects; //!< Vector of gameObjects.

	std::vector<GameObject> v_playerObjects; //!< Vector of gameObjects.
public:

	//! Scene default constructor.
	/*!
	*/
	Scene() {}
	//! Scene constructor.
	/*!
	*/
	Scene(std::string sceneName);

	//! Scene Destructor.
    /*!
    */
	~Scene();
	
	//! Game Object Pointer Getter Function.
    /*!
	\returns game object pointer.
    */
	GameObject *getControlledGameObject();

	//! Game Object Pointer Getter Function.
	/*!
	\returns game object pointer.
	*/
	GameObject* getControlledPlayerObject();
	
	//! Vector of GameObjects Getter Function.
    /*!
	\returns multiple game objects pointers.
    */
	std::vector<GameObject> *GetGameObjects();

	//! Render Function.
    /*!
	\param engineInterfacePtr handles rendering.
    */
	void render(IEngineCore *engineInterfacePtr); 

	//! Bool Json Loading Function.
    /*!
	\param levelJSONfile string of jsonfile name
    */
	bool loadLevelJSON(std::string levelJSONFile);

	//! Bool Json Loading Function.
    /*!
	\param levelJSONfile string of jsonfile name
    */
	bool loadplayerJSON(std::string playerJSONFile);


};


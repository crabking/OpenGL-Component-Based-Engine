#pragma once
/*!
\file SceneHandler.h
*/
#include <string>

#include "Scene.h"

/*!
\class SceneHandler
\brief Handles scenes.
*/
class SceneHandler : public Component
{

	
public:
	Scene* m_level1;
	Scene* m_level2;
	Scene* m_currentScene;

	InputHandler* m_currentInputHandler;
	InputHandler* m_InputHandler1;
	InputHandler* m_InputHandler2;


	void swapScene() {

		if (m_currentScene == m_level1)
		{

			m_currentScene = m_level2;

			m_InputHandler1 = new InputHandler(m_currentScene->getControlledPlayerObject());
			m_currentInputHandler = m_InputHandler1;
		}
		if(m_currentScene == m_level2)
		{
			m_currentScene = m_level1;

			m_InputHandler2 = new InputHandler(m_level2->getControlledPlayerObject());
			m_currentInputHandler = m_InputHandler2;
		}

	}

	SceneHandler() {
		
		m_currentScene = new Scene("0");
		m_level1 = new Scene("0");
		m_level2 = new Scene("1");
		

		m_currentInputHandler = new InputHandler(m_currentScene->getControlledPlayerObject());
	
	}

	Scene* getCurrentScene() {
		return m_currentScene;
	}

	InputHandler* getcurrentInputHandler() {
		return m_currentInputHandler;
	}
	

	//! Update Function override.
	/*!
	\param dt, float for delta time.
	*/
	void OnUpdate(float dt) override {}

	//! OnMessage Function Override.
	/*!
	\param m const string which takes a message from components.
	*/
	void OnMessage(const std::string m) override {}

	

};


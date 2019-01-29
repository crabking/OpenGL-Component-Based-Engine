#include "Game.h"

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>


#include "FirstPersonCameraComponent.h"
#include "ThirdPersonCameraComponent.h"


using namespace std;

Game::Game()
{
	DebugLog& log = log.getInstance(); //!< gets Debug instance.
	factory = new Factory();

	//m_currentScene = new Scene("0");

	m_engineInterfacePtr = nullptr; //!< initialize engineinterfaceptr.
	m_sceneHandler = new SceneHandler();
	
	m_inputHandler = m_sceneHandler->getcurrentInputHandler();
	//m_inputHandler = new InputHandler(getCurrentScene()->getControlledPlayerObject()); //! handles input on the 1 game object		

}

Game::~Game()
{
	delete m_inputHandler;
	delete m_engineInterfacePtr;
	delete factory;
	delete m_sceneHandler;
	
}

void Game::update()
{

	
}

void Game::render()
{

	FirstPersonCameraComponent* cam = getCurrentScene()->getControlledPlayerObject()->getComponent<FirstPersonCameraComponent>();
	ThirdPersonCameraComponent* tpcam = getCurrentScene()->getControlledPlayerObject()->getComponent<ThirdPersonCameraComponent>();

	if (getCurrentScene()->getControlledPlayerObject()->firstPerson)
	{
		m_engineInterfacePtr->setFirstPersonCamera(cam); //! gets players camera component from player object
	}
	if(!getCurrentScene()->getControlledPlayerObject()->firstPerson)
	{
		m_engineInterfacePtr->setThirdPersonCamera(tpcam); //! gets players camera component from player object
	}
	
	m_engineInterfacePtr->renderColouredBackground(0.2f,0.2f,0.2f); //! draw background


	getCurrentScene()->render(m_engineInterfacePtr); //! render current scene

}






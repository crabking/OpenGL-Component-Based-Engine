#pragma once
/*!
\file InputHandler.h
*/
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

#include "GameObject.h"

#include "json/json.h"

#define TRANSLATE_ADJUSTMENT_VALUE 0.01f;
/*!
\class InputCommand
\brief Input interface which handles execution of inputs.
*/
class InputCommand
{
public:
	virtual ~InputCommand() {}
	virtual void execute(GameObject& object ) = 0;
	
};


//---------- COLOR COMMANDS -------------------------------------------//

/*!
\class RotateLeftCommand
\brief Handles Roations to the left.
*/
class EditBlueColorCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override { obj.OnMessage("increase"); }
};

/*!
\class RotateLeftCommand
\brief Handles Roations to the left.
*/
class EditGreenColorCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override {obj.OnMessage("increase"); }
};

/*!
\class RotateLeftCommand
\brief Handles Roations to the left.
*/
class EditRedColorCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override {obj.OnMessage("increase"); }
};


//------------------ ROTATION COMMANDS -----------------------------------//

/*!
\class RotateLeftCommand
\brief Handles Roations to the left.
*/
class RotateLeftCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override{ obj.OnMessage("rotateLeft"); }
};

/*!
\class RotateRightCommand
\brief Handles Roations to the Right.
*/
class RotateRightCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override { obj.OnMessage("rotateRight"); }
};

/*!
\class RotateUpCommand
\brief Handles Roations upwards.
*/
class RotateUpCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override { obj.OnMessage("rotateUp"); }
};

/*!
\class RotateDownCommand
\brief Handles Roations downwards.
*/
class RotateDownCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override{ obj.OnMessage("rotateDown"); }
};


//--------------------- TRANSLATION COMMANDS ---------------------------//

/*!
\class TranslateLeftCommand
\brief Handles translation to the left.
*/
class TranslateLeftCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override{ obj.OnMessage("moveLeft"); }
};

/*!
\class TranslateRightCommand
\brief Handles translation to the right.
*/
class TranslateRightCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override { obj.OnMessage("moveRight"); }
};

/*!
\class TranslateForwardsCommand
\brief Handles translation forwards.
*/
class TranslateForwardsCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override { obj.OnMessage("moveForward"); }
};

/*!
\class TranslateBackwardsCommand
\brief Handles translation backwards.
*/
class TranslateBackwardsCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override { obj.OnMessage("moveBackward"); }
};


//----------------------- SWAP CAMREA COMMANDS -------------------------//

/*!
\class FirstPersonCamCommand
\brief Sets and Handles first person camera and its inputs.
*/
class FirstPersonCamCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override {obj.OnMessage("setFPcam"); }
};

/*!
\class FirstPersonCamCommand
\brief Sets and Handles first person camera and its inputs.
*/
class ThirdPersonCamCommand : public InputCommand
{
public:
	inline void execute(GameObject& obj) override{	obj.OnMessage("setTPcam");	}
};

//
///*!
//\class NextLevelCommand
//\brief Handles level swapping.
//*/
//class NextLevelCommand : public InputCommand
//{
//public:
//	inline void execute(GameObject& obj) override { 
//		if (obj.getComponent<SceneHandler>())
//		{
//			obj.getComponent<SceneHandler>()->OnMessage("setNextLevel");
//		}
//		
//	}	
//};
//
///*!
//\class PreviousLevelCommand
//\brief  Handles level swapping.
//*/
//class PreviousLevelCommand : public InputCommand
//{
//public:
//	inline void execute(GameObject& obj) override {
//		if (obj.getComponent<SceneHandler>())
//		{
//			obj.getComponent<SceneHandler>()->OnMessage("setNextLevel");
//		}
//	}
//	
//};

/*!
\class InputHandler
\brief communicates with game objects to execute tasks.
*/
struct InputHandler
{	

	int moveForwardKey, moveBackwardKey, moveLeftKey, moveRightkey, rotateUpKey, rotateDownKey, rotateLeftKey, rotateRightKey,
		setFirstPersonCameraKey, setThirdPersonCameraKey, increaseRedColorKey, increaseBlueColorKey, increaseGreenColorKey,
		nextLevelKey, previousLevelkey;
	bool setupInputs(std::string levelJSONFile)
	{

		std::fstream jsonData;
		Json::Value root;
		Json::Reader reader;
		jsonData.open(levelJSONFile.c_str());
		// check for errors
		if (!reader.parse(jsonData, root))
		{
			std::cout << "Failed to parse data from: "
				<< levelJSONFile
				<< reader.getFormattedErrorMessages();
			return false;
		}

		const Json::Value keyboardInput = root["KeyboardInputs"];
	
		int moveForward = keyboardInput["MoveForward"].asInt();
		moveForwardKey = moveForward;
		
		int moveBackward = keyboardInput["MoveBackward"].asInt();
		moveBackwardKey = moveBackward;
		
		int moveLeft = keyboardInput["MoveLeft"].asInt();
		moveLeftKey = moveLeft;
		
		int moveRight = keyboardInput["MoveRight"].asInt();
		moveRightkey = moveRight;

		int rotateUp = keyboardInput["RotateUp"].asInt();
		rotateUpKey = rotateUp;

		int rotateDown = keyboardInput["RotateDown"].asInt();
		rotateDownKey = rotateDown;

		int rotateRight = keyboardInput["RotateRight"].asInt();
		rotateRightKey = rotateRight;

		int rotateLeft = keyboardInput["RotateLeft"].asInt();
		rotateLeftKey = rotateLeft;

		int setFirstPersonCam = keyboardInput["SetFirstPersonCamera"].asInt();
		setFirstPersonCameraKey = setFirstPersonCam;

		int setThirdPersonCam = keyboardInput["SetThirdPersonCamera"].asInt();
		setThirdPersonCameraKey = setThirdPersonCam;

		int increaseRedColor = keyboardInput["IncreaseRedColor"].asInt();
		increaseRedColorKey = increaseRedColor;

		int increaseBlueColor = keyboardInput["IncreaseBlueColor"].asInt();
		increaseBlueColorKey = increaseBlueColor;

		int increaseGreenColor = keyboardInput["IncreaseGreenColor"].asInt();
		increaseGreenColorKey = increaseGreenColor;

		int nextLevel = keyboardInput["NextLevel"].asInt();
		nextLevelKey = nextLevel;

		int previousLevel = keyboardInput["PreviousLevel"].asInt();
		previousLevelkey = previousLevel;
				
		return true;
	}

	GameObject* m_playerObject; //!< game object pointer.

	std::map<int, InputCommand*> m_controlMapping; //!< map of controls.

	InputHandler(GameObject* obj) : m_playerObject(obj)
	{
		setupInputs("assets/Levels/keys.json");
		
		m_controlMapping[moveForwardKey] = new TranslateForwardsCommand;
		m_controlMapping[moveBackwardKey] = new TranslateBackwardsCommand;
		m_controlMapping[moveLeftKey] = new TranslateLeftCommand;
		m_controlMapping[moveRightkey] = new TranslateRightCommand;

		m_controlMapping[rotateLeftKey] = new RotateLeftCommand;
		m_controlMapping[rotateRightKey] = new RotateRightCommand;
		m_controlMapping[rotateUpKey] = new RotateUpCommand;
		m_controlMapping[rotateDownKey] = new RotateDownCommand;

		m_controlMapping[setFirstPersonCameraKey] = new FirstPersonCamCommand;
		m_controlMapping[setThirdPersonCameraKey] = new ThirdPersonCamCommand;
		
		m_controlMapping[increaseRedColorKey] = new EditRedColorCommand;
		m_controlMapping[increaseBlueColorKey] = new EditBlueColorCommand;
		m_controlMapping[increaseGreenColorKey] = new EditGreenColorCommand;

		/*m_controlMapping[nextLevelKey] = new NextLevelCommand;
		m_controlMapping[previousLevelkey] = new PreviousLevelCommand;*/
	}


	void handleInputs(const std::vector<bool>& keyBuffer)
	{
		for(const auto& mapEntry : m_controlMapping)
		{
			if(keyBuffer[mapEntry.first])
			{
				mapEntry.second->execute(*m_playerObject);
			}
		}

	}

	~InputHandler() {
	
		delete m_controlMapping[moveForwardKey];
		delete m_controlMapping[moveBackwardKey];
		delete m_controlMapping[moveLeftKey];
		delete m_controlMapping[moveRightkey];
		delete m_controlMapping[rotateLeftKey];
		delete m_controlMapping[rotateRightKey];
		delete m_controlMapping[rotateUpKey];
		delete m_controlMapping[rotateDownKey];
		delete m_controlMapping[setFirstPersonCameraKey];
		delete m_controlMapping[setThirdPersonCameraKey];
		delete m_controlMapping[nextLevelKey];
		delete m_controlMapping[previousLevelkey];
	}

};
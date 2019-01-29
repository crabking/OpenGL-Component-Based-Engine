#include "Scene.h"

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "json/json.h"

using namespace std;

Factory* factory;



Scene::Scene(std::string sceneName)
{
	factory = new Factory();

	loadplayerJSON("assets/Levels/Player" + sceneName + ".json");
	loadLevelJSON("assets/Levels/Level" + sceneName + ".json");
}




bool Scene::loadplayerJSON (std::string playerJSONFile) //! Loads player from file, choose from different players based on model.
{
	

	std::fstream jsonData;
	Json::Value root;
	Json::Reader reader;
	jsonData.open(playerJSONFile.c_str());
	// check for errors
	if (!reader.parse(jsonData, root))
	{
		std::cout << "Failed to parse data from: "
			<< playerJSONFile
			<< reader.getFormattedErrorMessages();
		return false;
	}

	const Json::Value playerModels = root["GameObjects"];


	v_playerObjects.resize(playerModels.size());

	// size() tells us how large the array is
	for (int i = 0; i < playerModels.size(); i++)
	{
		//----> name in json file <----//
		std::cout << playerModels[i]["name"].asString() << " loaded\n";

		//----> the ACTUAL modelname in json <------//
		std::string modelName = playerModels[i]["model"].asString();

		//----> the values pos or scale in json <------//
		float x, y, z, w;

		// get the position node
		const Json::Value playerposNode = playerModels[i]["playerposition"];
		x = playerposNode[0].asFloat(); // get float
		y = playerposNode[1].asFloat();
		z = playerposNode[2].asFloat();
		glm::vec3 playerpos(x, y, z);
		cout << "playerposition :" << x << y << z << endl;

		// get the position node
		const Json::Value firstpersoncamposNode = playerModels[i]["firstpersoncamposition"];
		x = firstpersoncamposNode[0].asFloat(); // get float
		y = firstpersoncamposNode[1].asFloat();
		z = firstpersoncamposNode[2].asFloat();
		glm::vec3 firstpersonCamPos(x, y, z);
		cout << "first person cam position: " << x << y << z << endl;

		// get the position node
		const Json::Value thirdpersoncamposNode = playerModels[i]["thirdpersoncamposition"];
		x = thirdpersoncamposNode[0].asFloat(); // get float
		y = thirdpersoncamposNode[1].asFloat();
		z = thirdpersoncamposNode[2].asFloat();
		glm::vec3 thirdpersonCamPos(x, y, z);
		cout << "third person cam position: " << x << y << z << endl;


		const Json::Value oriNode = playerModels[i]["playerorientation"];
		x = oriNode[0].asFloat(); // get float
		y = oriNode[1].asFloat();
		z = oriNode[2].asFloat();
		w = oriNode[3].asFloat();
		glm::quat playerori(x, y, z, w);
		cout << "player orientation : " << x << y << z << w << endl;

		const Json::Value camoriNode = playerModels[i]["camorientation"];
		x = camoriNode[0].asFloat(); // get float
		y = camoriNode[1].asFloat();
		z = camoriNode[2].asFloat();
		w = camoriNode[3].asFloat();
		glm::quat camori(x, y, z, w);
		cout << "cam orientation: " << x << y << z << w << endl;


		const Json::Value scaNode = playerModels[i]["scale"];
		x = scaNode[0].asFloat(); // get float
		y = scaNode[1].asFloat();
		z = scaNode[2].asFloat();
		glm::vec3 sca(x, y, z);
		cout << "scale: " << x << y << z << endl;


		//--------------- ADD COMPONENTS TO PLAYER HERE --------------------------------------//
		

		v_playerObjects[i].addComponent(factory->createModelComponent(m_modelmanager.getModel(modelName)));
		v_playerObjects[i].addComponent(factory->createTransformComponent(playerpos, playerori, sca));

		v_playerObjects[i].addComponent(factory->createFPCameraComponent(firstpersonCamPos, camori));
		v_playerObjects[i].addComponent(factory->createTPCameraComponent(thirdpersonCamPos, camori));
		
	
	}

	return true;
}

bool Scene::loadLevelJSON(std::string levelJSONFile)
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

	const Json::Value gameObjects = root["GameObjects"];


	v_gameObjects.resize(gameObjects.size());

	// size() tells us how large the array is
	for (int i = 0; i < gameObjects.size(); i++)
	{
		//----> name in json file <----//
		std::cout << gameObjects[i]["name"].asString() << " loaded\n";

		//----> the ACTUAL modelname in json <------//
		std::string modelName = gameObjects[i]["model"].asString();


		//----> the values pos or scale in json <------//
		float x, y, z, w;
		// get the position node
		const Json::Value posNode = gameObjects[i]["position"];
		x = posNode[0].asFloat(); // get float
		y = posNode[1].asFloat();
		z = posNode[2].asFloat();
		glm::vec3 pos(x, y, z);

		const Json::Value oriNode = gameObjects[i]["orientation"];
		x = oriNode[0].asFloat(); // get float
		y = oriNode[1].asFloat();
		z = oriNode[2].asFloat();
		w = oriNode[3].asFloat();
		glm::quat ori(x, y, z, w);

		const Json::Value scaNode = gameObjects[i]["scale"];
		x = scaNode[0].asFloat(); // get float
		y = scaNode[1].asFloat();
		z = scaNode[2].asFloat();
		glm::vec3 sca(x, y, z);


		//--------------- ADD COMPONENTS TO LEVEL GAME OBJECTS HERE --------------------------------------//
		v_gameObjects[i].addComponent(factory->createTransformComponent(pos, ori, sca));
		v_gameObjects[i].addComponent(factory->createModelComponent(m_modelmanager.getModel(modelName)));


	}

	return true;
}




GameObject* Scene::getControlledGameObject()
{
	return &v_gameObjects[0];
}

GameObject* Scene::getControlledPlayerObject()
{
	return &v_playerObjects[0];
}

std::vector<GameObject>* Scene::GetGameObjects()
{
	return &v_gameObjects;
}

void Scene::render(IEngineCore *engineInterfacePtr)
{
	for (int i = 0; i < v_gameObjects.size(); i++)
	{
		Model* model = v_gameObjects[i].getComponent<ModelComponent>()->getModel();
		glm::mat4 gameobjects = v_gameObjects[i].getComponent<TransformComponent>()->getModelMatrix(); // ?

		engineInterfacePtr->drawModel(model, gameobjects);
	}

	for (int i = 0; i < v_playerObjects.size(); i++) 
	{
		Model* playerModel = v_playerObjects[i].getComponent<ModelComponent>()->getModel();
		glm::mat4 playerobjects = v_playerObjects[i].getComponent<TransformComponent>()->getModelMatrix(); // ?

		engineInterfacePtr->drawModel(playerModel, playerobjects);
	
	}
}

Scene::~Scene()
{
	for (GameObject gameObject : v_gameObjects)
	{
		if (gameObject.getComponent<TransformComponent>())
		{
			delete gameObject.getComponent<TransformComponent>();
		}
		if (gameObject.getComponent<ModelComponent>())
		{
			delete gameObject.getComponent<ModelComponent>();
		}
		if (gameObject.getComponent<FirstPersonCameraComponent>())
		{
			delete gameObject.getComponent<FirstPersonCameraComponent>();
		}
		if (gameObject.getComponent<ThirdPersonCameraComponent>())
		{
			delete gameObject.getComponent<ThirdPersonCameraComponent>();
		}
	}

	for (GameObject playerObject : v_playerObjects)
	{
		if (playerObject.getComponent<TransformComponent>())
		{
			delete playerObject.getComponent<TransformComponent>();
		}
		if (playerObject.getComponent<ModelComponent>())
		{
			delete playerObject.getComponent<ModelComponent>();
		}
		if (playerObject.getComponent<FirstPersonCameraComponent>())
		{
			delete playerObject.getComponent<FirstPersonCameraComponent>();
		}
		if (playerObject.getComponent<ThirdPersonCameraComponent>())
		{
			delete playerObject.getComponent<ThirdPersonCameraComponent>();
		}
	}
}





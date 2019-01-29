#include "SceneObject.h"

#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "json/json.h"
using namespace std;

SceneObject::SceneObject()
{

	loadLevel(filePath + "config" + fileName);

	/*for (int i = 0; i < v_Levels.size(); i++)
	{
		cout << v_Levels[i] << endl;
	}*/

	// game loads the level list file
	//cout << v_Levels << endl;
	// know we know the aanmes of each level and how many there are

	// game starts at first level
	// allows you to step the next level (if itr exists)

	
}

std::vector<string>* SceneObject::GetSceneLevels()
{
	return &v_Levels;
}
void SceneObject::loadLevel(string levelConfigFile)
{
	string level;
	string s;
	ifstream inputFile;
	stringstream ss;
	int numObj;
	

	inputFile.open(levelConfigFile, ios_base::in);

	if (inputFile.is_open())
	{
		getline(inputFile, s);
		ss.clear();
		ss.str(s);
		ss.ignore(17);
		ss >> numObj;

		

		for (int i = 0; i < numObj; i++)
		{
	
			getline(inputFile, s);
			
			v_Levels.push_back(s);
		
		}
	}

}

SceneObject::~SceneObject()
{
	// destroy stuff
}

//
//bool SceneManager::levelLoader(std::string levelListFile)
//{
//
//	std::fstream jsonData;
//	Json::Value root;
//	Json::Reader reader;
//	jsonData.open( (filePath+ levelListFile +fileName).c_str() );
//	// check for errors
//	if (!reader.parse(jsonData, root))
//	{
//		std::cout << "Failed to parse data from: "
//			<< levelListFile
//			<< reader.getFormattedErrorMessages();
//		return false;
//	}
//
//	const Json::Value json_levels = root["Levels"];
//
//	
//	for (int i = 0; i < json_levels.size(); i++)
//	{
//		//----> name in json file <----//
//		std::cout << json_levels[i]["level"].asString() << " loaded\n";
//		//----> the ACTUAL modelname in json <------//
//		std::string levelname = json_levels[i]["level"].asString();
//	}
//
//	return true;
//}




//bool Game::readConfigLevelJSON(std::string levelConfig, std::string levelChoice)
//{
//
//	std::fstream jsonData;
//	Json::Value root;
//	Json::Reader reader;
//	jsonData.open(levelConfig.c_str());
//	// check for errors
//	if (!reader.parse(jsonData, root))
//	{
//		std::cout << "Failed to parse data from: "
//			<< levelConfig
//			<< reader.getFormattedErrorMessages();
//		return false;
//	}
//
//	const Json::Value jsonlevels = root["Levels"];
//
//
//	v_jsonLevels.resize(jsonlevels.size());
//
//	// size() tells us how large the array is
//	for (int i = 0; i < jsonlevels.size(); i++)
//	{
//		//----> the ACTUAL modelname in json <------//
//		std::string levelsInJsonFile = jsonlevels[i]["name"].asString();
//		std::cout << levelsInJsonFile << " read\n"; // prints out the loaded levels
//		std::cout << endl;
//		
//		if (jsonlevels[i]["name"].asString() == levelChoice)
//		{
//
//			//std::cout << levelsInJsonFile << " loaded\n"; // prints out the loaded levels
//
//			std::cout << endl;
//
//			std::cout << v_jsonLevels[i]  << endl;
//
//			
//		}
//
//	}
//
//	
//	return true;
//}

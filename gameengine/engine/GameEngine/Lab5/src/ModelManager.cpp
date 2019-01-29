#include "ModelManager.h"
#include <iostream>


ModelManager::ModelManager()
{
}


ModelManager::~ModelManager()
{
}


Model* ModelManager::getModel(string name)
{ 
		//myMapOfModels.find(name);
		if (myMapOfModels.find(name) != myMapOfModels.end()) // already exists so return
		{
			return myMapOfModels[name]; // returns model
		}
		else // doesnt exist so make new then return
		{
			Model* myTestModel = new Model(filePath + name + fileName);
			//myMapOfModels.insert(pair <std::string, Model*>(name, myTestModel));
			myMapOfModels[name] = myTestModel;
			return myMapOfModels[name]; // returns model, 
		}
}
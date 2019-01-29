#pragma once
/*!
\file ModelManager.h
*/
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Model.h"

/*!
\class Scene
\brief Handles Models, creating and returning them.
*/
class ModelManager
{

private:

	std::string filePath = "assets/models/"; //!< filepath name.
	std::string fileName = ".obj"; //!< filename.

	std::map<std::string, Model* > myMapOfModels; //!<  map of model pointers.

public:
	//! ModelManager default constructor.
	/*!
	*/
	ModelManager();
	//! ModelManager default destructor.
	/*!
	*/
	~ModelManager();

	//! Render Function.
	/*!
	\param name gets model name from map.
	*/
	Model* getModel(string name); //!< 

};


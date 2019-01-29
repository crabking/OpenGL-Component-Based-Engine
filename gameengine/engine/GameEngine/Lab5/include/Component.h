#pragma once
/*!
\file Component.h
*/
#include <string>

/*!
\class Component
\Interface for components, all components have an update and messaging function.
*/
class Component
{
public:

	//! Update Function Interface.
	/*!
	\param dt 
	*/
	virtual void OnUpdate(float dt) = 0;

	//! OnMessage Function Interface.
	/*!
	\param m 
	*/
	virtual void OnMessage(const std::string m) = 0;

};


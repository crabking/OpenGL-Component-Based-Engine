#pragma once
/*!
\file ColourComponent.h
*/
#include "Component.h"

#define COLOUR_ADJUSTMENT_VALUE 0.1f

/*!
\class ColourComponent
\brief Adjust color values of a component.
*/
class ColourComponent : public Component
{

public:

	ColourComponent() : m_colourValue(0) {};

	void OnUpdate(float dt) override{}

	void OnMessage(const std::string m) override
	{
		if (m == "increase")
		{
			m_colourValue += COLOUR_ADJUSTMENT_VALUE;
		}
		else if (m == "decrease")
		{
			m_colourValue -= COLOUR_ADJUSTMENT_VALUE;
		}
	}

	float m_colourValue;

};

class RedComponent : public ColourComponent {};
class GreenComponent : public ColourComponent {};
class BlueComponent : public ColourComponent {};
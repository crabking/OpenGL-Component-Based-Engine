#pragma once
/*!
\file DebugLog.h
*/

#include <vector>
#include <iostream>
#include <string>

// TODO this entire class since its fucked
using namespace std;

/*!
\class DebugLog
\brief DebugLog is a Singleton design pattern class implementation that can be accessed in all classes for writign debug messages to the screen.
*/
class DebugLog
{
private:

	//! Default Constructor.
	/*!
	*/
	DebugLog() {}

	//! Destructor.
	/*!
	*/
	~DebugLog() {}

	//! Copy Constructor.
	/*!
	*/
	DebugLog(DebugLog const&);

	//! Operator Overloader.
	/*!
	\brief Blocks accidentally making new instances of this object.
	*/
	void operator=(DebugLog const&);

public:

	//! Constructor.
	/*!
	\brief Returns only one instance of this object.
	*/
	static DebugLog& getInstance()
	{
		static DebugLog instance; //!< Guaranteed to be destroyed.
							      //!< Instantiated on first use.
		return instance;
	}

	vector<string> v_messages; //!< Vector holding messages;

	//! WwriteLog function.
	/*!
	\brief puts messages into a vector which it later can print to screen through another function.
	*/
	void writeLog(string message) {
		v_messages.push_back(message);
	}


	
};

/**************************************************************************
|	File:	DestroyEntityMessage.h
|	Author: Jordan Ness
|	Purpose: The DestroyEntityMessage class is used by the message
|			callback function to allow entities to be destroyed by others
**************************************************************************/
#pragma once

#include "..\SGD Wrappers\SGD_Message.h"

//Forward Declarations
class Entity;

class DestroyEntityMessage :
	public SGD::Message
{
public:
	DestroyEntityMessage(Entity* destroy);
	~DestroyEntityMessage();

	///////////////////////////////////////////////////
	/////////// Destroyed Entity Accessor ////////////
	Entity*		GetEntity(void) const		{ return m_pDestroy; }


private:
	Entity* m_pDestroy;
};


/**************************************************************************
|	File:	ChangeLevelMessage.h
|	Author: Michael Sciortino
|	Purpose: Changes the current level to the given level
**************************************************************************/

#pragma once

#include "..\SGD Wrappers\SGD_Message.h"

//Forward Declarations
class Entity;

class ChangeLevelMessage :
	public SGD::Message
{
public:
	ChangeLevelMessage(Entity* owner);
	~ChangeLevelMessage();

	Entity* GetOwner(void) const { return m_pOwner; }

private:

	Entity* m_pOwner;
};


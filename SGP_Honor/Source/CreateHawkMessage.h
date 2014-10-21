/**************************************************************************
|	File:	CreateHawkMessage.h
|	Author: Conor Maloney
|	Purpose: CreateProjectileMessage class is used in the message callback
|			function to allow entities to create projectiles
**************************************************************************/

#pragma once

#include "..\SGD Wrappers\SGD_Message.h"

//Forward Declarations
class Entity;

class CreateHawkMessage :
	public SGD::Message
{
public:
	CreateHawkMessage(Entity* owner);
	~CreateHawkMessage();

	Entity* GetOwner(void) const { return m_pOwner; }

private:

	Entity* m_pOwner;
};


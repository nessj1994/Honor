/**************************************************************************
|	File:	CreateSprayMessage.h
|	Author: Conor Maloney
|	Purpose: CreateProjectileMessage class is used in the message callback
|			function to allow entities to create projectiles
**************************************************************************/

#pragma once

#include "..\SGD Wrappers\SGD_Message.h"

//Forward Declarations
class Entity;

class CreateSprayMessage :
	public SGD::Message
{
public:
	CreateSprayMessage(Entity* owner);
	~CreateSprayMessage();

	Entity* GetOwner(void) const { return m_pOwner; }

private:

	Entity* m_pOwner;
};


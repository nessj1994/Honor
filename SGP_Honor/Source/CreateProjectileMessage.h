/**************************************************************************
|	File:	CreateProjectileMessage.h
|	Author: Jordan Ness
|	Purpose: CreateProjectileMessage class is used in the message callback
|			function to allow entities to create projectiles
**************************************************************************/

#pragma once

#include "..\SGD Wrappers\SGD_Message.h"

//Forward Declarations
class Entity;

class CreateProjectileMessage :
	public SGD::Message
{
public:
	CreateProjectileMessage(Entity* owner);
	~CreateProjectileMessage();

	Entity* GetOwner(void) const { return m_pOwner; }

private:

	Entity* m_pOwner;
};


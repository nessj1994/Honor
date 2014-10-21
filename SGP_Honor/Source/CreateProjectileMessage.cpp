/**************************************************************************
|	File:	CreateProjectileMessage.cpp
|	Author: Jordan Ness
|	Purpose: CreateProjectileMessage class is used in the message callback 
|			function to allow entities to create projectiles
**************************************************************************/

#include "CreateProjectileMessage.h"
#include "Entity.h"
#include "MessageID.h"

CreateProjectileMessage::CreateProjectileMessage(Entity* owner)
	: SGD::Message(MessageID::MSG_CREATE_PROJECTILE)
{
	m_pOwner = owner;
	owner->AddRef();

}


CreateProjectileMessage::~CreateProjectileMessage()
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr;
	}
}

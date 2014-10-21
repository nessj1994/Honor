/**************************************************************************
|	File:	CreateSprayMessage.cpp
|	Author: Conor
|	Purpose: CreateProjectileMessage class is used in the message callback
|			function to allow entities to create projectiles
**************************************************************************/

#include "CreateSprayMessage.h"
#include "Entity.h"
#include "MessageID.h"

CreateSprayMessage::CreateSprayMessage(Entity* owner)
: SGD::Message(MessageID::MSG_CREATE_SPRAY)
{
	m_pOwner = owner;
	owner->AddRef();

}


CreateSprayMessage::~CreateSprayMessage()
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr;
	}
}

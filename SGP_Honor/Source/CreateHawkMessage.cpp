/**************************************************************************
|	File:	CreateHawkMessage.cpp
|	Author: Conor
|	Purpose: CreateProjectileMessage class is used in the message callback
|			function to allow entities to create projectiles
**************************************************************************/

#include "CreateHawkMessage.h"
#include "Entity.h"
#include "MessageID.h"

CreateHawkMessage::CreateHawkMessage(Entity* owner)
: SGD::Message(MessageID::MSG_CREATE_HAWK)
{
	m_pOwner = owner;
	owner->AddRef();

}


CreateHawkMessage::~CreateHawkMessage()
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr;
	}
}

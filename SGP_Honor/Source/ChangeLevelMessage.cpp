/**************************************************************************
|	File:	ChangeLevelMessage.h
|	Author: Michael Sciortino
|	Purpose: Changes the current level to the given level
**************************************************************************/

#include "ChangeLevelMessage.h"
#include "Entity.h"
#include "MessageID.h"

ChangeLevelMessage::ChangeLevelMessage(Entity* owner)
	: SGD::Message(MessageID::MSG_CHANGE_LEVEL)
{
	m_pOwner = owner;
	owner->AddRef();

}


ChangeLevelMessage::~ChangeLevelMessage()
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr;
	}
}

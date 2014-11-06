#include "CreateStalactite.h"
#include "Entity.h"
#include "MessageID.h"

CreateStalactiteMessage::CreateStalactiteMessage(Entity* Owner)
: SGD::Message(MessageID::MSG_CREATE_STALACTITE)
{
	m_pOwner = Owner;
	Owner->AddRef();
}


CreateStalactiteMessage::~CreateStalactiteMessage()
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr;
	}
}
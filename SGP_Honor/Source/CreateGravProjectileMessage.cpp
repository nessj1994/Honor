#include "CreateGravProjectileMessage.h"
#include "Entity.h"
#include "MessageID.h"

CreateGravProjectileMessage::CreateGravProjectileMessage(Entity* owner) : SGD::Message(MessageID::MSG_CREATE_GRAVPROJECTILE)
{
	m_pOwner = owner;
	owner->AddRef();
}


CreateGravProjectileMessage::~CreateGravProjectileMessage()
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr;
	}
}

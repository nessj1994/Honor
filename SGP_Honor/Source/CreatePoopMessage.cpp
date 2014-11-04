#include "CreatePoopMessage.h"
#include "Entity.h"
#include "MessageID.h"

CreatePoopMessage::CreatePoopMessage(Entity* Owner)
: SGD::Message(MessageID::MSG_CREATE_POOP)
{
	m_pOwner = Owner;
	Owner->AddRef();
}


CreatePoopMessage::~CreatePoopMessage()
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr;
	}
}

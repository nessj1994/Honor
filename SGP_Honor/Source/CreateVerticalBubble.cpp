#include "CreateVerticalBubble.h"
#include "Entity.h"
#include "MessageID.h"

CreateVerticalBubble::CreateVerticalBubble(Entity* owner) : SGD::Message(MessageID::MSG_CREATE_VERT_BUBBLE)
{
	m_pOwner = owner;
	owner->AddRef();
}


CreateVerticalBubble::~CreateVerticalBubble()
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr;
	}
}

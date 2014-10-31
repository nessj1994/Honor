#include "CreateHorizontalBubble.h"
#include "Entity.h"
#include "MessageID.h"

CreateHorizontalBubble::CreateHorizontalBubble(Entity* owner) : SGD::Message(MessageID::MSG_CREATE_HORIZ_BUBBLE)
{
	m_pOwner = owner;
	owner->AddRef();
}


CreateHorizontalBubble::~CreateHorizontalBubble()
{
	if (m_pOwner != nullptr)
	{
		m_pOwner->Release();
		m_pOwner = nullptr;
	}
}

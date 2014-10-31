#pragma once
#include "..\SGD Wrappers\SGD_Message.h"

class Entity;

class CreateHorizontalBubble : public SGD::Message
{
public:
	CreateHorizontalBubble(Entity* owner);
	~CreateHorizontalBubble();

	Entity* GetOwner(void) const { return m_pOwner; }

private:
	Entity* m_pOwner = nullptr;
};


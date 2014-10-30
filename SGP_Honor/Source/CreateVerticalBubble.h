#pragma once
#include "..\SGD Wrappers\SGD_Message.h"

class Entity;

class CreateVerticalBubble : public SGD::Message
{
public:
	CreateVerticalBubble(Entity* owner);
	~CreateVerticalBubble();

	Entity* GetOwner(void) const { return m_pOwner; }

private:
	Entity* m_pOwner = nullptr;
};


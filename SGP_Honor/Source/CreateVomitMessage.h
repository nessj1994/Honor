#pragma once

#include "../SGD Wrappers/SGD_Message.h"

class Entity;

class CreateVomitMessage : public SGD::Message
{

	Entity* m_pOwner;

public:
	Entity* GetOwner(void) const { return m_pOwner; }
	CreateVomitMessage(Entity* Owner);
	~CreateVomitMessage();
};


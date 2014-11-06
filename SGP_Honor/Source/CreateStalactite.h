#pragma once

#include"../SGD Wrappers/SGD_Message.h"

class Entity;

class CreateStalactiteMessage : public SGD::Message
{

	Entity* m_pOwner;

public:
	Entity* GetOwner(void) const { return m_pOwner; }
	CreateStalactiteMessage(Entity* _Owner);
	~CreateStalactiteMessage();
};



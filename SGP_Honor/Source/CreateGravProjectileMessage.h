#pragma once
#include "..\SGD Wrappers\SGD_Message.h"

class Entity;

class CreateGravProjectileMessage : public SGD::Message
{
public:
	CreateGravProjectileMessage(Entity* owner);
	~CreateGravProjectileMessage();

	Entity* GetOwner(void) const { return m_pOwner; }

private:
	Entity* m_pOwner = nullptr;
};


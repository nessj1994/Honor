#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Listener.h"

class Player;

class Armor : public Entity, SGD::Listener
{
public:
	Armor();
	virtual ~Armor();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return Entity::ENT_ARMOR; };
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;
	bool GetIsCollected() const { return isCollected; }
	void HandleEvent(const SGD::Event* pEvent);

private:
	bool isCollected = false;
};


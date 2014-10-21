#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class Player;

class Armor :
	public Entity
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

private:
	bool isCollected = false;
};


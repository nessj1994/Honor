#pragma once
#include "Entity.h"
class Stalactite :
	public Entity
{
public:
	Stalactite();
	virtual ~Stalactite();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;
};


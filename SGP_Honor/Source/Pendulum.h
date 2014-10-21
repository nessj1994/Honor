#pragma once
#include "Entity.h"
class Pendulum :
	public Entity
{
public:
	Pendulum();
	virtual ~Pendulum();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;
};


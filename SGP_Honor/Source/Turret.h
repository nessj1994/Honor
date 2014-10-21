#pragma once
#include "Entity.h"
class Turret :
	public Entity
{
public:
	Turret();
	virtual ~Turret();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

private:

	float m_fFireTimer = 1.0f;
};



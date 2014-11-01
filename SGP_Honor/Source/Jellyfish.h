#pragma once
#include "Enemy.h"
#include "../SGD Wrappers/SGD_Listener.h"

class Jellyfish : public Enemy, SGD::Listener
{
public:
	Jellyfish();
	virtual ~Jellyfish();

	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_JELLYFISH; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;
	void HandleEvent(const SGD::Event* pEvent);
	int GetBounceCount() const { return numOfBounces; }
	void SetPatrol();

private:
	int numOfBounces = 0;
	float bouncetimer = 0.0f;
	bool bouncecounting = false;
	SGD::Vector patrolDistance = SGD::Vector(100.0f, 0.0f);
	SGD::Vector patrolTotal;
};


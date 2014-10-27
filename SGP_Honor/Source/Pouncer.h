#pragma once
#include "Enemy.h"
#include "../SGD Wrappers/SGD_Listener.h"

class Player;

class Pouncer : public Enemy, SGD::Listener
{
public:
	Pouncer();
	virtual ~Pouncer();

	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_ENEMY; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	void HandleEvent(const SGD::Event* pEvent);
	void SetTarget(Player* plr);

private:
	Player* target = nullptr;
};


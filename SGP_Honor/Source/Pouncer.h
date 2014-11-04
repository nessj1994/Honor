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

	int GetType(void) const override { return ENT_POUNCER; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;
	bool GetInAir() const { return inAir; }

	void HandleEvent(const SGD::Event* pEvent);
	void SetTarget(Player* plr);
	SGD::HAudio GetDeathSound() const { return m_aDeath; }

private:
	Player* target = nullptr;
	bool isPouncing = false;
	float apex = 0.0f;
	bool inAir = false;
	SGD::HAudio m_aPounce = SGD::INVALID_HANDLE;
	SGD::HAudio m_aDeath = SGD::INVALID_HANDLE;
};


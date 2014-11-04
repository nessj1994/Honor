#pragma once
#include "Enemy.h"
#include "../SGD Wrappers/SGD_Listener.h"

class Player;

class Squid : public Enemy, SGD::Listener
{
public:
	Squid();
	virtual ~Squid();

	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_SQUID; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	void HandleEvent(const SGD::Event* pEvent);
	void SetTarget(Player* plr);
	SGD::HAudio GetDeathSound() const { return m_aDeath; }

private:
	float shotTimer = 0.0f;
	Player* target = nullptr;
	SGD::HAudio spit = SGD::INVALID_HANDLE;
	SGD::HAudio m_aDeath = SGD::INVALID_HANDLE;
};


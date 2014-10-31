#pragma once
#include "Enemy.h"
#include "../SGD Wrappers/SGD_Listener.h"

class Player;

class MutantMan : public Enemy, SGD::Listener
{
public:
	MutantMan();
	~MutantMan();
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_ENEMY; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	void HandleEvent(const SGD::Event* pEvent);
	void SetTarget(Player* plr);
	void Begin(SGD::Point _Pos){ m_pPatrolPoint = _Pos; }

private:
	Player* m_pptarget = nullptr;
	SGD::Point m_pPatrolPoint;
};


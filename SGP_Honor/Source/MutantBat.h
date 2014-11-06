#pragma once
#include "Enemy.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include <vector>

class MutantBat : public Enemy
{
public:
	MutantBat();
	~MutantBat();
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_MUTANT_BIRD; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	void Begin(SGD::Point _Pos){ m_pPatrolPoint = _Pos; }

private:
	SGD::Point m_pPatrolPoint;
	//Pooping Timer
	float m_fPoopTimer;
	//Sound Effects
	SGD::HAudio m_hHurt = SGD::INVALID_HANDLE;
	SGD::HAudio m_hPoop = SGD::INVALID_HANDLE;
	//Dieing Timer
	float m_fDieing;
}; 


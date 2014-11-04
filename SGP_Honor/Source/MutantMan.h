#pragma once
#include "Enemy.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include <vector>

class Vomit;
class MutantMan : public Enemy, SGD::Listener
{
public:
	MutantMan();
	~MutantMan();
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_MUTANT_MAN; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	void HandleEvent(const SGD::Event* pEvent);
	void Begin(SGD::Point _Pos){ m_pPatrolPoint = _Pos; }

private:
	SGD::Point m_pPatrolPoint;
	//Vomiting Timer
	float m_fVomitTimer;
	bool m_bPlayerAttacked;
	//Sound Effects
	SGD::HAudio m_hHurt = SGD::INVALID_HANDLE;
	SGD::HAudio m_hGag = SGD::INVALID_HANDLE;
	SGD::HAudio m_hPunch = SGD::INVALID_HANDLE;
};


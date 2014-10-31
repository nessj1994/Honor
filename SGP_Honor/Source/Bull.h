#pragma once
#include "Boss.h"

#include "../SGD Wrappers/SGD_Listener.h"

class Bull :
	public Boss, SGD::Listener
{
public:
	Bull();
	virtual ~Bull();

	enum BullState { BS_WALKING, BS_CHARGING, BS_RUNNING, BS_RETURNING, BS_DEATH, BS_SLOWING, BS_STUNNED };

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual int GetType(void) const override { return ENT_BOSS_BULL; }
	void HandleCollision(const IEntity* pOther) override;

	////////////////////////////////////////////////
	////////////// Listener Interface //////////////
	void HandleEvent(const SGD::Event* pEvent);

	bool GetRunning() const { return m_bsCurrState == BS_RUNNING; }
	void ResetBull();

private:
	////////////////////////////////////////////////
	////////////// Member fields //////////////
	bool m_bCanCharge = false;
	bool m_bDead = false;

	float m_fStunSpeed = 0.0f;

	float m_fTurnTimer = 0.0f;
	float m_fChargeTimer = 0.0f;
	float m_fRunTimer = 0.0f;
	float m_fWalkTimer = 0.0f;
	float m_fSlowTimer = 0.0f;
	float m_fStunTimer = 0.0f;
	float m_fDeathTimer = 0.0f;

	BullState m_bsCurrState;
	
};


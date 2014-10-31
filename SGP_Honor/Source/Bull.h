#pragma once
#include "Boss.h"

#include "../SGD Wrappers/SGD_Listener.h"

class Bull :
	public Boss, SGD::Listener
{
public:
	Bull();
	virtual ~Bull();

	enum BullState { BS_WALKING, BS_CHARGING, BS_RUNNING, BS_RETURNING, BS_DEATH };

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual int GetType(void) const override { return ENT_BOSS_BULL; }
	void HandleCollision(const IEntity* pOther) override;

	////////////////////////////////////////////////
	////////////// Listener Interface //////////////
	void HandleEvent(const SGD::Event* pEvent);

private:
	////////////////////////////////////////////////
	////////////// Member fields //////////////
	float m_fTurnTimer = 0.0f;
	BullState m_bsCurrState;
	
};


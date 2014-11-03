#pragma once
#include "Boss.h"

#include "../SGD Wrappers/SGD_Listener.h"


class Wizard :
	public Boss, SGD::Listener
{
public:
	Wizard();
	virtual ~Wizard();

	enum WizardState { WZ_FLOATING, WZ_BULL, WZ_HAWK, WZ_BUBBLE, WZ_ICE, WZ_VUNERABLE, WZ_DEATH, };

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual int GetType(void) const override { return ENT_BOSS_WIZARD; }
	void HandleCollision(const IEntity* pOther) override;

	//void HandleEvent(const SGD::Event* pEvent);


private:

	float m_fFloatTimer = 0.0f;
	float m_fXTimerLoop = 0.0f;
	float m_fYTimerLoop = 0.0f;

	bool floatingLeft = true;

	WizardState m_bsCurrState;
	SGD::HAudio m_hVictory = SGD::INVALID_HANDLE;

};


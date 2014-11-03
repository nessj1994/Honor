#pragma once
#include "Boss.h"

#include "../SGD Wrappers/SGD_Listener.h"

class WizardDash;
class WizardHawk;

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

	void CastClones();
	void ResetClones();

	void CastHawks();
	void ResetHawks();



	//DashPtrs
	void SetDash1(WizardDash* _ptr) { dashPtr1 = _ptr; }
	void SetDash2(WizardDash* _ptr) { dashPtr2 = _ptr; }
	void SetDash3(WizardDash* _ptr) { dashPtr3 = _ptr; }
	void SetDash4(WizardDash* _ptr) { dashPtr4 = _ptr; }

	//HawkPtrs
	void SetHawk1(WizardHawk* _ptr) { hawkPtr1 = _ptr; }
	void SetHawk2(WizardHawk* _ptr) { hawkPtr2 = _ptr; }
	void SetHawk3(WizardHawk* _ptr) { hawkPtr3 = _ptr; }
	void SetHawk4(WizardHawk* _ptr) { hawkPtr4 = _ptr; }

private:

	float m_fFloatTimer = 0.0f;
	float m_fXTimerLoop = 0.0f;
	float m_fYTimerLoop = 0.0f;
	float m_fDashStateTimer = 3.5f;
	float m_fHawkStateTimer = 13.5f;

	bool floatingLeft = true;
	bool clonesCasted = false;
	bool hawksCasted = false;


	WizardState m_bsCurrState;
	SGD::HAudio m_hVictory = SGD::INVALID_HANDLE;

	WizardDash* dashPtr1 = nullptr;
	WizardDash* dashPtr2 = nullptr;
	WizardDash* dashPtr3 = nullptr;
	WizardDash* dashPtr4 = nullptr;

	WizardHawk* hawkPtr1 = nullptr;
	WizardHawk* hawkPtr2 = nullptr;
	WizardHawk* hawkPtr3 = nullptr;
	WizardHawk* hawkPtr4 = nullptr;
};


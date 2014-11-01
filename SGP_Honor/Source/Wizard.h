#pragma once
#include "Boss.h"
class Wizard :
	public Boss
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


private:

	WizardState m_bsCurrState;

};


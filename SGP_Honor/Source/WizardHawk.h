#pragma once
#include "Unit.h"
class WizardHawk :
	public Unit
{
public:
	WizardHawk();
	virtual ~WizardHawk();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_WIZARD_DASH; }
	virtual SGD::Rectangle GetRect(void) const override { return SGD::Rectangle{ m_ptPosition, m_szSize }; }
	virtual void HandleCollision(const IEntity* pOther) override;


	void SetTimer(float _timer) { m_fTimer = _timer; }


private:

	//particle effect 
	//SGD::Vector m_vtDirection = { 0, 0 };
	float m_fTimer = 0.0f;
};


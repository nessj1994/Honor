#pragma once
#include "Unit.h"

class Wizard;


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

	SGD::Point GetDestPos(void) { return m_ptDestPos; }
	void SetDestPos(SGD::Point _pt) { m_ptDestPos = _pt; }

	SGD::Size GetDestSize(void) { return m_szDestSize; }
	void SetDestSize(SGD::Size _sz) { m_szDestSize = _sz; }

private:

	//particle effect 
	//SGD::Vector m_vtDirection = { 0, 0 };
	float m_fTimer = 0.0f;

	Wizard* m_pWizard = nullptr;
	SGD::Size m_szDestSize = { 0, 0 };
	SGD::Point m_ptDestPos  = { 0, 0 };
};


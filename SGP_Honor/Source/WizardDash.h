#pragma once
#include "Unit.h"
class WizardDash :
	public Unit
{
public:
	WizardDash();
	virtual ~WizardDash();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_WIZARD_DASH; }
	virtual SGD::Rectangle GetRect(void) const override {return SGD::Rectangle{ m_ptPosition, m_szSize };}
	virtual void HandleCollision(const IEntity* pOther) override;

	////////////////////////////////////////////////
	/////////////////Accessors/////////////////////
	//SGD::Vector GetDirection(void) const { return m_vtDirection; }
	//Entity* GetOwner(void) const { return m_pOwner; }


	///////////////////////////////////////////////
	////////////////Mutators//////////////////////
	//void SetDirection(SGD::Vector Direction) { m_vtDirection = Direction; }
	//void SetOwner(Entity* owner) { if (m_pOwner) m_pOwner->Release(); m_pOwner = owner; if(m_pOwner)m_pOwner->AddRef(); }
	void SetTimer(float _timer) { m_fTimer = _timer; }



private:

	//particle effect 
	//SGD::Vector m_vtDirection = { 0, 0 };
	//Entity* m_pOwner = nullptr;
	float m_fTimer = 0.0f;
	
};


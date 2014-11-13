#pragma once
#include "Entity.h"
#include "Activator.h"
#include "../SGD Wrappers/SGD_Listener.h"

class Emitter;


class Laser :
	public Entity, public SGD::Listener
{
public:
	Laser();
	virtual ~Laser();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;


	SGD::Point	GetOrigPosition(void) const  { return m_ptOrigPos; }
	void		SetOrigPosition(SGD::Point pos)		{ m_ptOrigPos = pos; }

	Activator* GetActivator(void) const { return m_pActivator; }
	void SetActivator(Activator* activator) { if (m_pActivator)m_pActivator->Release(); m_pActivator = activator; if (m_pActivator)m_pActivator->AddRef(); }
	
	unsigned int GetFreq(void)const { return m_nFreq; }
	void SetFreq(unsigned int _freq) { m_nFreq = _freq; }

	void HandleEvent(const SGD::Event* pEvent);
	void SetOn(bool on) { m_bOn = on; }


	//SGD::Point	GetDirection(void) const	{ return m_ptDirection; }
	//void		SetDirection(SGD::Point dir)	   { m_ptDirection = dir; }


private:

	SGD::Point m_ptOrigPos = { 0, 0 };
	SGD::Size m_szOrigSize = { 0, 0 };

	bool m_bFull = false;
	
	bool m_bOn = true;

	Activator* m_pActivator = nullptr;

	unsigned int m_nFreq = 0;


	//SGD::Point m_ptDirection = { 0, 0 };

	SGD::HAudio m_hEffect = SGD::INVALID_HANDLE;

	Emitter* m_LaserEffect;
};


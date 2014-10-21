#pragma once
#include "Entity.h"
#include "Activator.h"


class Laser :
	public Entity
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


	//SGD::Point	GetDirection(void) const	{ return m_ptDirection; }
	//void		SetDirection(SGD::Point dir)	   { m_ptDirection = dir; }


private:

	SGD::Point m_ptOrigPos = { 0, 0 };
	SGD::Size m_szOrigSize = { 0, 0 };

	bool m_bFull = false;
	Activator* m_pActivator = nullptr;


	//SGD::Point m_ptDirection = { 0, 0 };

};


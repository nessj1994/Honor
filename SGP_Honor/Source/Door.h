#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "Activator.h"


class Door :
	public Entity
{
public:
	Door();
	virtual ~Door();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;

	///////////////////////////////////////////////
	///////////////// Accessors //////////////////
	bool GetOpen(void) const { return m_bOpen; }
	bool GetHorizontal(void) const { return m_bHorizontal; }
	Activator* GetActivator(void) const { return m_pActivator; }

	/////////////////////////////////////////////
	///////////////// Mutators /////////////////
	void SetOpen(bool isOpen) { m_bOpen = isOpen; }
	void SetHorizontal(bool isHorizontal) { m_bHorizontal = isHorizontal; }
	void SetActivator(Activator* activator) { if(m_pActivator)m_pActivator->Release(); m_pActivator = activator; if(m_pActivator)m_pActivator->AddRef(); }

private:

	bool m_bOpen = false;
	bool m_bHorizontal = false;
	Activator* m_pActivator = nullptr;
	

};


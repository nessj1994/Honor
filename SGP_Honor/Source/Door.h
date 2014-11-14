#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "Activator.h"


class SGD::Event;

class Door :
	public Entity, public SGD::Listener
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

	////////////////////////////////////////////////
	////////////// Listener Interface //////////////
	void HandleEvent(const SGD::Event* pEvent);



	///////////////////////////////////////////////
	///////////////// Accessors //////////////////
	bool GetOpen(void) const { return m_bOpen; }
	bool GetHorizontal(void) const { return m_bHorizontal; }
	Activator* GetActivator(void) const { return m_pActivator; }
	unsigned int GetKeyID(void) const { return m_unKeyID; }

	/////////////////////////////////////////////
	///////////////// Mutators /////////////////
	void SetOpen(bool isOpen) { m_bOpen = isOpen; }
	void SetHorizontal(bool isHorizontal) { m_bHorizontal = isHorizontal; }
	void SetStartOpen(bool _start) { m_bStartOpen = _start; }
	void SetActivator(Activator* activator) { if (m_pActivator)m_pActivator->Release(); m_pActivator = activator; if (m_pActivator)m_pActivator->AddRef(); }
	void SetKeyID(unsigned int ID) { m_unKeyID = ID; }


private:
	/////////////////////////////////////////////
	///////////////// Member fields /////////////////
	bool m_bOpen = false;
	bool m_bHorizontal = false;
	bool m_bStartOpen = false;
	Activator* m_pActivator = nullptr;
	unsigned int m_unKeyID = 0;

	SGD::HTexture m_hDoorH = SGD::INVALID_HANDLE;
	SGD::HTexture m_hDoorV = SGD::INVALID_HANDLE;
};


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
	int GetLength(void) const { return m_nLength; }

	/////////////////////////////////////////////
	///////////////// Mutators /////////////////
	void SetOpen(bool isOpen) { m_bOpen = isOpen; }
	void SetHorizontal(bool isHorizontal) { m_bHorizontal = isHorizontal; }
	void SetActivator(Activator* activator) { if (m_pActivator)m_pActivator->Release(); m_pActivator = activator; if (m_pActivator)m_pActivator->AddRef(); }
	void SetKeyID(unsigned int ID) { m_unKeyID = ID; }
	void SetLength(int _length) { m_nLength = _length; }


private:

	bool m_bOpen = false;
	bool m_bHorizontal = false;
	int m_nLength = 1;
	Activator* m_pActivator = nullptr;
	unsigned int m_unKeyID = 0;

};

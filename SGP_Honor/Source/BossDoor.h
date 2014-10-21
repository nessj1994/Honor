#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Listener.h"

class BossDoor :
	public Entity, public SGD::Listener
{
public:
	BossDoor();
	virtual ~BossDoor();

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
	////////////////// Accessors /////////////////
	bool GetOpen(void) const { return m_bOpen; }
	unsigned int GetRequiredHonor(void) const { return m_unRequiredHonor; }



	/////////////////////////////////////////////
	//////////////// Mutators //////////////////
	void SetOpen(bool open) { m_bOpen = open; }
	void SetRequiredHonor(unsigned int honor) { m_unRequiredHonor = honor; }


private:

	bool m_bOpen = false;
	unsigned int m_unRequiredHonor = 50;

};


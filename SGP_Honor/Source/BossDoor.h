#pragma once
#include "Teleporter.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include <string>

class BossDoor :
	public Teleporter
{
public:
	BossDoor();
	virtual ~BossDoor();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Render(void) override;
	virtual int GetType(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	///////////////////////////////////////////////
	////////////////// Accessors /////////////////
	unsigned int GetRequiredHonor(void) const { return m_unRequiredHonor; }



	/////////////////////////////////////////////
	//////////////// Mutators //////////////////
	void SetRequiredHonor(unsigned int honor) { m_unRequiredHonor = honor; }


private:

	unsigned int m_unRequiredHonor = 50;

};


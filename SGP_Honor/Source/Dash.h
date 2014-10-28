#pragma once
#include "Entity.h"
#include "Unit.h"
#include "AnimTimeStamp.h"
#include "../SGD Wrappers/SGD_Handle.h"

//#include "an"

class Dash :
	public Entity
{
public:
	Dash();
	virtual ~Dash();

	void CastDash(Unit* pOther);

	//void SetOwner(Entity* ptr) { m_pOwner = ptr; }
	//Entity* GetOwner(void) { return m_pOwner; }
	AnimTimeStamp m_tDashTimeStamp;



private:
	//Entity* m_pOwner = nullptr;
	SGD::HAudio m_hEffect = SGD::INVALID_HANDLE;

};


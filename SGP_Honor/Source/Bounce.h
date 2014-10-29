#pragma once
#include "Entity.h"
//#include "Unit.h"


class Unit;
class Emitter;

class Bounce :
	public Entity
{
	//Emitter 
	Emitter* m_emBubbles;
public:
	Bounce();
	virtual ~Bounce();

	void CastBounce(Unit* pOther);
	//Get Emitter
	Emitter* GetEMBubbles(){ return m_emBubbles; }
	

};


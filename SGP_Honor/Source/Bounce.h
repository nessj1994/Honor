#pragma once
#include "Entity.h"
//#include "Unit.h"


class Unit;

class Bounce :
	public Entity
{
public:
	Bounce();
	virtual ~Bounce();

	void CastBounce(Unit* pOther);


};


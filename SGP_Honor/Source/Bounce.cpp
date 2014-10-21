#include "Bounce.h"
#include "Unit.h"


Bounce::Bounce()
{
}


Bounce::~Bounce()
{
}


void Bounce::CastBounce(Unit* pOther)
{

	if (pOther->GetType() == ENT_PLAYER)
	{
		pOther->SetIsBouncing(true);
	}

}
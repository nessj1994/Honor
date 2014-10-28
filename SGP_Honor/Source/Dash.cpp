#include "Dash.h"


Dash::Dash()
{
}


Dash::~Dash()
{
}


void Dash:: CastDash(Unit* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		pOther->SetIsDashing(true);

		if (pOther->IsFacingRight() == true)
			pOther->SetVelocity({ 2000, pOther->GetVelocity().y });
		else
			pOther->SetVelocity({ - 2000, pOther->GetVelocity().y });



	}
}

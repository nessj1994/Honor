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
			pOther->SetVelocity({ pOther->GetVelocity().x + 1000, pOther->GetVelocity().y });
		else
			pOther->SetVelocity({ pOther->GetVelocity().x - 1000, pOther->GetVelocity().y });



	}
}

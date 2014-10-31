#include "Unit.h"


Unit::Unit()
{
}


Unit::~Unit()
{
}


void Unit::Update(float elapsedTime)
{
	if (m_bDashing == true)
		m_fDashTimer -= elapsedTime;


	if (m_fDashTimer <= 0)
	{
		m_bDashing = false;
		m_fDashTimer = 0.15f;
		if (IsFacingRight() == true)
			SetVelocity({/* GetVelocity().x - 30*/0, GetVelocity().y });
		else
			SetVelocity({ /*GetVelocity().x + 30*/0, GetVelocity().y });
	}

	Entity::Update(elapsedTime);
}



void Unit::Render(void)
{

	Entity::Render();
}

int Unit::GetType(void) const
{

	return 1;
}


SGD::Rectangle Unit::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };
}


void Unit::HandleCollision(const IEntity* pOther)
{

	if (pOther->GetType() == ENT_FROZEN)
	{
		SetFriction(0.1f);
	}
	else
	{
		SetFriction(1.0f);
	}
}

void Unit::BasicCollision(const IEntity* pOther)
{

}

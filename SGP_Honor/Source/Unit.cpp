#include "Unit.h"
#include <Windows.h>


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
		SetFriction(25.0f);
	}
}

void Unit::BasicCollision(const IEntity* pOther)
{
	SetGravity(-3000);

	RECT rBoss;
	rBoss.left = (LONG)GetRect().left;
	rBoss.top = (LONG)GetRect().top;
	rBoss.right = (LONG)GetRect().right;
	rBoss.bottom = (LONG)GetRect().bottom;

	//Create a rectangle for the other object
	RECT rObject;
	rObject.left = (LONG)pOther->GetRect().left;
	rObject.top = (LONG)pOther->GetRect().top;
	rObject.right = (LONG)pOther->GetRect().right;
	rObject.bottom = (LONG)pOther->GetRect().bottom;

	//Create a rectangle for the intersection
	RECT rIntersection = {};

	RECT rBossWall;
	rBossWall.left = (LONG)GetRect().left - 1;
	rBossWall.top = (LONG)GetRect().top;
	rBossWall.right = (LONG)GetRect().right + 1;
	rBossWall.bottom = (LONG)GetRect().bottom;

	IntersectRect(&rIntersection, &rObject, &rBossWall);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;

	//if (nIntersectHeight > nIntersectWidth)
	//{
	//	if (GetIsFalling() == true
	//		|| GetIsJumping() == true)
	//		SetIsInputStuck(true);

	//}

	IntersectRect(&rIntersection, &rBoss, &rObject);

	nIntersectWidth = rIntersection.right - rIntersection.left;
	nIntersectHeight = rIntersection.bottom - rIntersection.top;

	//Colliding with the side of the object
	if (nIntersectHeight > nIntersectWidth)
	{
		if (rBoss.right == rIntersection.right)
		{

			SetPosition({ (float)rObject.left - GetSize().width + 1, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
			//SetDashTimer(0);

			//is_Right_Coll = true;
		}
		if (rBoss.left == rIntersection.left)
		{
			SetPosition({ (float)rObject.right, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
			//SetDashTimer(0);

			//is_Left_Coll = true;

		}
	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (rBoss.bottom == rIntersection.bottom)
		{
			if (IsBouncing() == true)
			{
				SetVelocity({ GetVelocity().x, GetVelocity().y * -1 });
				SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height  /*- nIntersectHeight*/ });
			}

			else
			{
				SetVelocity({ GetVelocity().x, 0 });
				SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 /*- nIntersectHeight*/ });
			}


			//SetJumpVelCur(0);
			//SetIsJumping(false);
			//SetIsInputStuck(false);

			//is_Left_Coll = false;
			//is_Right_Coll = false;
		}
		if (rBoss.top == rIntersection.top)
		{
			SetPosition({ GetPosition().x, (float)rObject.bottom });
			SetVelocity({ GetVelocity().x, 0 });
		}
	}
}

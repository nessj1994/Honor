#include "Enemy.h"
#include <cmath>
#include <Windows.h>

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}


/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Enemy::Update(float elapsedTime)
{


}


void Enemy::Render(void)
{

}


int Enemy::GetType(void) const
{
	return 1;
}


SGD::Rectangle Enemy::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };
}


void Enemy::HandleCollision(const IEntity* pOther)
{
	
	//Creat a Mutant Man Rect;
	RECT Enemy;

	Enemy.top = m_ptPosition.y;
	Enemy.left = m_ptPosition.x;
	Enemy.bottom = m_ptPosition.y + m_szSize.width;
	Enemy.right = m_ptPosition.x + m_szSize.height;

	//Create An Object Rect
	RECT Object;

	Object.top = pOther->GetRect().top;
	Object.left = pOther->GetRect().left;
	Object.bottom = pOther->GetRect().bottom;
	Object.right = pOther->GetRect().right;

	//Create a rectangle for the intersection
	RECT rIntersection = {};

	IntersectRect(&rIntersection, &Enemy, &Object);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;

	//Colliding with the side of the object
	if (nIntersectHeight > nIntersectWidth)
	{
		if (Enemy.right == rIntersection.right)
		{
			SetPosition({ (float)Object.left - GetSize().width + 1, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
		if (Enemy.left == rIntersection.left)
		{
			SetPosition({ (float)Object.right, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (Enemy.bottom == rIntersection.bottom)
		{
			SetVelocity({ GetVelocity().x, 0 });
			SetPosition({ GetPosition().x, (float)Object.top - GetSize().height + 1 /*- nIntersectHeight*/ });
		}
		if (Enemy.top == rIntersection.top)
		{
			SetPosition({ GetPosition().x, (float)Object.bottom });
			SetVelocity({ GetVelocity().x, 0 });
		}
	}
	
	Unit::HandleCollision(pOther);
}
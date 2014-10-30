#include "Enemy.h"

#include "Player.h"
#include <Windows.h>

#include "../SGD Wrappers/SGD_GraphicsManager.h"


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
	Unit::HandleCollision(pOther);

	RECT rSwingRect;
	rSwingRect.left= GetPlayer()->GetSwingRect().left;
	rSwingRect.top = GetPlayer()->GetSwingRect().top;
	rSwingRect.right = GetPlayer()->GetSwingRect().right;
	rSwingRect.bottom = GetPlayer()->GetSwingRect().bottom;

	RECT rObject;
	rObject.left =		GetRect().left;
	rObject.top =		GetRect().top;
	rObject.right =		GetRect().right;
	rObject.bottom =	GetRect().bottom;

	RECT rIntersection = {};

	IntersectRect(&rIntersection, &rObject, &rSwingRect);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;

	if (nIntersectHeight > nIntersectWidth)
	{
		//if switch, activate switch
		//if evenmy call event to kill enemy
		SGD::GraphicsManager::GetInstance()->DrawString("PRESSED A", { 300, 300 }, { 255, 255, 0, 0 });

	}

	if (nIntersectHeight < nIntersectWidth)
	{
		//if switch, activate switch
		//if evenmy call event to kill enemy
		SGD::GraphicsManager::GetInstance()->DrawString("PRESSED A", { 300, 300 }, { 255, 255, 0, 0 });

	}

	

}
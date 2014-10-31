#include "Enemy.h"

#include "Player.h"
#include <Windows.h>
#include "Camera.h"
#include "DestroyEntityMessage.h"

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
	Camera::GetInstance()->Draw(SGD::Rectangle(GetRect()), SGD::Color::Color(255, 0, 255, 0));
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
	rObject.left =		GetRect().left- Camera::GetInstance()->GetCameraPos().x;
	rObject.top =		GetRect().top - Camera::GetInstance()->GetCameraPos().y;
	rObject.right = GetRect().right - Camera::GetInstance()->GetCameraPos().x;
	rObject.bottom = GetRect().bottom - Camera::GetInstance()->GetCameraPos().y;

	RECT rIntersection = {};

	IntersectRect(&rIntersection, &rObject, &rSwingRect);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;



	if (nIntersectHeight > nIntersectWidth)
	{
		//if switch, activate switch
		//if evenmy call event to kill enemy
		//SGD::GraphicsManager::GetInstance()->DrawString("PRESSED A", { 300, 300 }, { 255, 255, 0, 0 });

		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;

	}

	if (nIntersectHeight < nIntersectWidth)
	{
		//if switch, activate switch
		//if evenmy call event to kill enemy
		//SGD::GraphicsManager::GetInstance()->DrawString("PRESSED A", { 300, 300 }, { 255, 255, 0, 0 });

		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;

	}

	

}
#include "Enemy.h"
#include <cmath>
#include <Windows.h>

#include "Player.h"
#include <Windows.h>
#include "Camera.h"
#include "DestroyEntityMessage.h"
#include "SwordSwing.h"
#include "Pouncer.h"

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
	if (m_pPlayer->GetIsSwinging() == true)
	{
		RECT rSwingRect;
		rSwingRect.left =	GetPlayer()->GetSword()->GetRect().left;
		rSwingRect.top =	GetPlayer()->GetSword()->GetRect().top;
		rSwingRect.right =	GetPlayer()->GetSword()->GetRect().right;
		rSwingRect.bottom = GetPlayer()->GetSword()->GetRect().bottom;

		RECT rObject;
		rObject.left = GetRect().left - Camera::GetInstance()->GetCameraPos().x;
		rObject.top = GetRect().top - Camera::GetInstance()->GetCameraPos().y;
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

			if (this->GetType() == ENT_POUNCER)
			{
				Pouncer* pnc = dynamic_cast<Pouncer*>(this);
				if (pnc->GetInAir() == true)
				{
					DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
					pMsg->QueueMessage();
					pMsg = nullptr;
				}
			}
			else
			{
				DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
				pMsg->QueueMessage();
				pMsg = nullptr;
			}

		}

		if (nIntersectHeight < nIntersectWidth)
		{
			//if switch, activate switch
			//if evenmy call event to kill enemy
			//SGD::GraphicsManager::GetInstance()->DrawString("PRESSED A", { 300, 300 }, { 255, 255, 0, 0 });

			if (this->GetType() == ENT_POUNCER)
			{
				Pouncer* pnc = dynamic_cast<Pouncer*>(this);
				if (pnc->GetInAir() == true)
				{
					DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
					pMsg->QueueMessage();
					pMsg = nullptr;
				}
			}
			else
			{
				DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
				pMsg->QueueMessage();
				pMsg = nullptr;
			}

		}

		if (nIntersectHeight == nIntersectWidth
			&& nIntersectHeight != 0)
		{
			if (this->GetType() == ENT_POUNCER)
			{
				Pouncer* pnc = dynamic_cast<Pouncer*>(this);
				if (pnc->GetInAir() == true)
				{
					DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
					pMsg->QueueMessage();
					pMsg = nullptr;
				}
			}
			else
			{
				DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
				pMsg->QueueMessage();
				pMsg = nullptr;
			}
		}
	}


}


void Enemy::Render(void)
{
	Camera::GetInstance()->Draw(SGD::Rectangle(GetRect()), SGD::Color::Color(255, 0, 255, 0));
}


int Enemy::GetType(void) const
{
	return ENT_ENEMY;
}


SGD::Rectangle Enemy::GetRect(void) const
{

	return SGD::Rectangle({ m_ptPosition, m_szSize });
}


void Enemy::HandleCollision(const IEntity* pOther)
{
	
	//Creat a Mutant Man Rect;
	RECT Enemy;
	
	//GetPlayer()->GetSword()->GetRect()

	
	
	Unit::HandleCollision(pOther);
}
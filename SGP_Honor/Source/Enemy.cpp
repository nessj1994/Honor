#include "Enemy.h"
#include <cmath>
#include <Windows.h>

#include "Player.h"
#include "Camera.h"
#include "DestroyEntityMessage.h"
#include "SwordSwing.h"
#include "Pouncer.h"
#include "Squid.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
	if (m_pPlayer)
	{
		m_pPlayer->Release();
	}
}


/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Enemy::Update(float elapsedTime)
{
	if (m_pPlayer->GetIsSwinging() == true)
	{
		RECT rSwingRect;
		rSwingRect.left =	(long)GetPlayer()->GetSword()->GetRect().left;
		rSwingRect.top = (long)GetPlayer()->GetSword()->GetRect().top;
		rSwingRect.right = (long)GetPlayer()->GetSword()->GetRect().right;
		rSwingRect.bottom = (long)GetPlayer()->GetSword()->GetRect().bottom;

		RECT rObject;
		rObject.left = (long)GetRect().left - (long)Camera::GetInstance()->GetCameraPos().x;
		rObject.top = (long)GetRect().top - (long)Camera::GetInstance()->GetCameraPos().y;
		rObject.right = (long)GetRect().right - (long)Camera::GetInstance()->GetCameraPos().x;
		rObject.bottom = (long)GetRect().bottom - (long)Camera::GetInstance()->GetCameraPos().y;

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
					SGD::AudioManager::GetInstance()->PlayAudio(pnc->GetDeathSound());
					SetAlive(false);
				}
			}
			else if (this->GetType() == ENT_SQUID)
			{
				Squid* squid = dynamic_cast<Squid*>(this);
				SGD::AudioManager::GetInstance()->PlayAudio(squid->GetDeathSound());
				SetAlive(false);
			}
			else
			{
				SetAlive(false);
			}

		}

		if (nIntersectHeight < nIntersectWidth)
		{
			SetAlive(false);
			//if switch, activate switch
			//if evenmy call event to kill enemy
			//SGD::GraphicsManager::GetInstance()->DrawString("PRESSED A", { 300, 300 }, { 255, 255, 0, 0 });

			if (this->GetType() == ENT_POUNCER)
			{
				Pouncer* pnc = dynamic_cast<Pouncer*>(this);
				if (pnc->GetInAir() == true)
				{
					SGD::AudioManager::GetInstance()->PlayAudio(pnc->GetDeathSound());
					SetAlive(false);
				}
			}
			else if (this->GetType() == ENT_SQUID)
			{
				Squid* squid = dynamic_cast<Squid*>(this);
				SGD::AudioManager::GetInstance()->PlayAudio(squid->GetDeathSound());
				SetAlive(false);
			}
			else
			{
				SetAlive(false);
			}

		}

		if (nIntersectHeight == nIntersectWidth
			&& nIntersectHeight != 0)
		{
			SetAlive(false);
			if (this->GetType() == ENT_POUNCER)
			{
				Pouncer* pnc = dynamic_cast<Pouncer*>(this);
				if (pnc->GetInAir() == true)
				{
					SGD::AudioManager::GetInstance()->PlayAudio(pnc->GetDeathSound());
					SetAlive(false);
				}
			}
			else if (this->GetType() == ENT_SQUID)
			{
				Squid* squid = dynamic_cast<Squid*>(this);
				SGD::AudioManager::GetInstance()->PlayAudio(squid->GetDeathSound());
				SetAlive(false);
			}
			else
			{
				SetAlive(false);
			}
		}
	}

	// Apply gravity
	/*if (GetFalling() == true)
	{
		SetGravity(-3000);
		SetVelocity({ GetVelocity().x, GetVelocity().y - GetGravity() * elapsedTime });
	}*/

	Unit::Update(elapsedTime);

	SetFalling(true);
}


void Enemy::Render(void)
{
	// Render the collision rect
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({ -camPos.x, -camPos.y });

	//Render us with the camera
	Camera::GetInstance()->Draw(rMyRect, SGD::Color::Color(255, 255, 0, 0));

	//SGD::GraphicsManager::GetInstance()->DrawString("PRESSED A", { 300, 300 }, { 255, 255, 0, 0 });
}


int Enemy::GetType(void) const
{
	return ENT_ENEMY;
}


void Enemy::HandleCollision(const IEntity* pOther)
{
	
		//RECT rPouncer;
		//rPouncer.left = (LONG)GetRect().left;
		//rPouncer.top = (LONG)GetRect().top;
		//rPouncer.right = (LONG)GetRect().right;
		//rPouncer.bottom = (LONG)GetRect().bottom;

		////Create a rectangle for the other object
		//RECT rObject;
		//rObject.left = (LONG)pOther->GetRect().left;
		//rObject.top = (LONG)pOther->GetRect().top;
		//rObject.right = (LONG)pOther->GetRect().right;
		//rObject.bottom = (LONG)pOther->GetRect().bottom;

		////Create a rectangle for the intersection
		//RECT rIntersection = {};

		//IntersectRect(&rIntersection, &rPouncer, &rObject);

		//int nIntersectWidth = rIntersection.right - rIntersection.left;
		//int nIntersectHeight = rIntersection.bottom - rIntersection.top;

		////Colliding with the side of the object
		//if (nIntersectHeight > nIntersectWidth)
		//{
		//	if (rPouncer.right == rIntersection.right)
		//	{
		//		SetPosition({ (float)rObject.left - GetRect().ComputeWidth() / 2, GetPosition().y });
		//		SetVelocity({ 0, GetVelocity().y });
		//	}
		//	if (rPouncer.left == rIntersection.left)
		//	{
		//		SetPosition({ (float)rObject.right + GetRect().ComputeWidth() / 2, GetPosition().y });
		//		SetVelocity({ 0, GetVelocity().y });
		//	}
		//}

		//if (nIntersectWidth > nIntersectHeight)
		//{
		//	if (rPouncer.bottom == rIntersection.bottom)
		//	{
		//		SetVelocity({ GetVelocity().x, 0 });
		//		SetPosition({ GetPosition().x, (float)rObject.top - nIntersectHeight});
		//	}
		//	if (rPouncer.top == rIntersection.top)
		//	{
		//		SetPosition({ GetPosition().x, (float)rObject.bottom });
		//		SetVelocity({ GetVelocity().x, 0 });
		//	}
		//}
	

	Unit::HandleCollision(pOther);
}

///////////////////////////////////////////////////
// BasicCollision
// -Clip out of the given entity
void Enemy::BasicCollision(const IEntity* pOther)
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
		//SetHitWall(true);
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
			SetFalling(false);
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
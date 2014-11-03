#include "MutantBat.h"
#include "Player.h"
#include "EntityManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "AnimationEngine.h"
#include "CreatePoopMessage.h"
#include "Camera.h"

#include <cmath>
#include <Windows.h>

MutantBat::MutantBat()
{
	m_ptPosition = { 96, 672 };
	m_pPatrolPoint = m_ptPosition;
	AnimationEngine::GetInstance()->LoadAnimation("Assets/MutantBat.xml");
	m_ts.SetCurrAnimation("Flying");
	m_ts.SetPlaying(true);
	m_szSize = { 50, 70 };
	SetGravity(-1000.0f);
	m_fPoopTimer = 0;
}


MutantBat::~MutantBat()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

SGD::Rectangle MutantBat::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void MutantBat::Update(float _elapsedTime)
{
	//Timer Updates
	m_fPoopTimer += _elapsedTime;
	//
	//Update Animation
	AnimationEngine::GetInstance()->Update(_elapsedTime, m_ts, this);
	SGD::Vector distance;


	if (GetPlayer() != nullptr)
	{
		//Set Point if you have a target
		m_pPatrolPoint = GetPlayer()->GetPosition();
		m_pPatrolPoint.y -= 200;
	}

	//Find Distance to point
	distance = m_ptPosition - m_pPatrolPoint;
	if (distance.ComputeLength() < 500)
	{		
		//Move to patrol point
		if (m_ptPosition.x < m_pPatrolPoint.x - 5 || m_ptPosition.x > m_pPatrolPoint.x + 5)
		{
			if (m_ptPosition.x < m_pPatrolPoint.x)
			{
				//m_bFacingRight = false;
				m_vtVelocity.x = 300;
			}
			if (m_ptPosition.x > m_pPatrolPoint.x)
			{
				//m_bFacingRight = true;
				m_vtVelocity.x = -300;
			}
		}
		else
		{
			m_vtVelocity.x = 0;
		}
		if (m_ptPosition.y < m_pPatrolPoint.y - 2 || m_ptPosition.y > m_pPatrolPoint.y + 2)
		{
			if (m_ptPosition.x < m_pPatrolPoint.x)
			{
				m_vtVelocity.y = 300;
			}
			if (m_ptPosition.y > m_pPatrolPoint.y)
			{
				m_vtVelocity.y = -100;
			}
		}
		else
		{
			m_vtVelocity.y = 0;
		}		
		
		////Patrol if no target
		//if (m_pPatrolPoint == m_ptPosition && m_bFacingRight)
		//{
		//	m_bFacingRight = false;
		//	m_pPatrolPoint = m_ptPosition;
		//	m_pPatrolPoint.x -= 50;
		//}
		//else if (m_pPatrolPoint == m_ptPosition && !m_bFacingRight)
		//{
		//	m_bFacingRight = true;
		//	m_pPatrolPoint = m_ptPosition;
		//	m_pPatrolPoint.x += 50;
		//}
		if (distance.ComputeLength() < 20 && m_fPoopTimer > .5f)
		{
			//Attack the player
			if (m_ts.GetCurrAnimation() != "Pooping")
			{
				m_ts.ResetCurrFrame();
				m_ts.SetCurrAnimation("Pooping");
				m_ts.SetPlaying(true);
			}

			if (m_fPoopTimer > .8f && m_fPoopTimer < .84f)
			{
				CreatePoopMessage* Temp = new CreatePoopMessage(this);
				Temp->QueueMessage();
				Temp = nullptr;
			}

			if (m_fPoopTimer > 1.0f)
			{
				m_fPoopTimer = 0;
			}
		
			m_vtVelocity.x = 0;
			m_vtVelocity.y = 0;
		}
		else
		{
			if (m_ts.GetCurrAnimation() != "Flying")
			{
				m_ts.ResetCurrFrame();
				m_ts.SetCurrAnimation("Flying");
				m_ts.SetPlaying(true);
			}
		}
	}

	//Use the Units Update
	Unit::Update(_elapsedTime);
}

void MutantBat::Render()
{
	SGD::GraphicsManager::GetInstance()->DrawRectangle({ { m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y }, m_szSize }, { 255, 255, 255, 255 });
	Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, 1/*m_bFacingRight*/, 1);
}

void MutantBat::HandleCollision(const IEntity* pOther)
{


	if (pOther->GetType() == Entity::ENT_PLAYER && GetRect().IsIntersecting(pOther->GetRect()) == true)
	{
			//if so move back up but kill the player
			SGD::Event Event = { "KILL_PLAYER", nullptr, this };
			SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}

	RECT rMutant;
	rMutant.left = GetRect().left;
	rMutant.top = GetRect().top;
	rMutant.right = GetRect().right;
	rMutant.bottom = GetRect().bottom;

	RECT rObject;
	rObject.left = pOther->GetRect().left;
	rObject.top = pOther->GetRect().top;
	rObject.right = pOther->GetRect().right;
	rObject.bottom = pOther->GetRect().bottom;

	RECT rIntersection = {};

	IntersectRect(&rIntersection, &rObject, &rMutant);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;



	if (nIntersectHeight > nIntersectWidth)
	{
		if (rMutant.right == rIntersection.right)
		{
			SetPosition({ (float)rObject.left - GetSize().width + 1, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
		if (rMutant.left == rIntersection.left)
		{
			SetPosition({ (float)rObject.right, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (rMutant.bottom == rIntersection.bottom)
		{
			SetVelocity({ GetVelocity().x, 0 });
			SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 /*- nIntersectHeight*/ });
		}
		if (rMutant.top == rIntersection.top)
		{
			SetPosition({ GetPosition().x, (float)rObject.bottom });
			SetVelocity({ GetVelocity().x, 0 });
		}
	}


	Enemy::HandleCollision(pOther);
}

#include "MutantMan.h"
#include "Player.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "AnimationEngine.h"
#include "Camera.h"


MutantMan::MutantMan() : Listener(this)
{
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	m_ptPosition = { 96, 672 };
	m_pPatrolPoint = m_ptPosition;
	AnimationEngine::GetInstance()->LoadAnimation("Assets/MutantManAnim.xml");
	m_ts.SetCurrAnimation("Mutant_Walking");
	m_ts.SetPlaying(true);
	m_szSize = {50,70};
	SetGravity(-1000.0f);
}


MutantMan::~MutantMan()
{
	SetTarget(nullptr);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

void MutantMan::Update(float _elapsedTime)
{
	//Update Animation
	AnimationEngine::GetInstance()->Update(_elapsedTime, m_ts, this);
	SGD::Vector distance;
	
	
	if (m_pptarget != nullptr)
	{
		//Set Point if you have a target
		m_pPatrolPoint = m_pptarget->GetPosition();
	}

	//Find Distance to point
	distance = m_ptPosition - m_pPatrolPoint;
	if (distance.ComputeLength() < 255)
	{
		//Move to patrol point
		if (m_ptPosition.x < m_pPatrolPoint.x)
		{
			SetFacingRight(false);
			m_vtVelocity.x = 300;
		}
		if (m_ptPosition.x > m_pPatrolPoint.x)
		{
			SetFacingRight(true);
			m_vtVelocity.x = -300;
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
	}
	else
	{
		m_vtVelocity = { m_vtVelocity.x != 0 ? m_vtVelocity.x -= GetGravity() * _elapsedTime : m_vtVelocity.x = 0, 300 };
	}

	SetVelocity({ GetVelocity().x, (GetVelocity().y - GetGravity() * _elapsedTime) });

	Unit::Update(_elapsedTime);
}

void MutantMan::Render()
{
	SGD::GraphicsManager::GetInstance()->DrawRectangle({{ m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x,m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y}, m_szSize}, { 255, 255, 255, 255 });
	Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, GetFacingRight(),1);
}

SGD::Rectangle MutantMan::GetRect(void) const 
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void MutantMan::HandleCollision(const IEntity* pOther) 
{


	if (pOther->GetType() == Entity::ENT_PLAYER && GetRect().IsIntersecting(pOther->GetRect()) == true)
	{
		//if so move back up but kill the player
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}

	
	Enemy::HandleCollision(pOther);
}

void MutantMan::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ASSESS_PLAYER_RANGE" && m_pptarget == nullptr)
	{
		SetTarget((Player*)pEvent->GetSender());
	}

}

void MutantMan::SetTarget(Player* plr)
{
	if (m_pptarget != nullptr)
		m_pptarget->Release();
	m_pptarget = plr;
	if (m_pptarget != nullptr)
		m_pptarget->AddRef();
}
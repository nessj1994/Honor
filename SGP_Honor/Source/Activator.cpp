#include "Activator.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include <Windows.h>
#include "Player.h"
#include "SwordSwing.h"


Activator::Activator(bool isPressure)
{
	m_bPressurePlate = isPressure;

	//m_ptPosition = { 68, 300 };
	m_szSize = { 32, 32 };

}


Activator::~Activator()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Activator::Update(float elapsedTime)
{
	if (m_fSwitchTimer > 0.0f)
	{
		m_fSwitchTimer -= elapsedTime;

	}


	if(m_fSwitchTimer < 0.0f && m_bPressurePlate == false)
	{
		m_fSwitchTimer = 0.0f;
	}
	else if(m_fSwitchTimer < 0.0f && m_bPressurePlate == true)
	{
		m_fSwitchTimer = 0.0f;
		//Close Door
		SGD::Event* pATEvent = new SGD::Event("CLOSE_DOOR", nullptr, this);
		SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
		pATEvent = nullptr;
	}


	if (m_pPlayer->GetIsSwinging() == true
		&& GetType() == ENT_SWITCH)
	{
		
		RECT rSwingRect;
		rSwingRect.left = m_pPlayer->GetSword()->GetRect().left;
		rSwingRect.top = m_pPlayer->GetSword()->GetRect().top;
		rSwingRect.right = m_pPlayer->GetSword()->GetRect().right;
		rSwingRect.bottom = m_pPlayer->GetSword()->GetRect().bottom;

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
			//Open Door
			SGD::Event* pATEvent = new SGD::Event("FLIP_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;

			pATEvent = new SGD::Event("FLIP_LASER", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 3.0f;
		}

		if (nIntersectHeight < nIntersectWidth)
		{
			//Open Door
			SGD::Event* pATEvent = new SGD::Event("FLIP_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;

			pATEvent = new SGD::Event("FLIP_LASER", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 3.0f;
		}
		if (nIntersectHeight == nIntersectWidth)
		{
			//Open Door
			SGD::Event* pATEvent = new SGD::Event("FLIP_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;

			pATEvent = new SGD::Event("FLIP_LASER", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 3.0f;
		}
	}






}

void Activator::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//Get our rectangle
	SGD::Rectangle rMyRect = GetRect();

	rMyRect.Offset({ -camPos.x, -camPos.y });

	Camera::GetInstance()->Draw(rMyRect, { 255, 255, 0, 0 });
}

int Activator::GetType(void) const
{

	//return 1;

	if (m_bPressurePlate == false)
	{
		return ENT_SWITCH;
	}
	else
	{
		return ENT_PRESSURE_PLATE;
	}
}

SGD::Rectangle Activator::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}

void Activator::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		if(m_bPressurePlate == false && m_fSwitchTimer == 0.0f)
		{
		//	//Open Door
		//	SGD::Event* pATEvent = new SGD::Event("FLIP_DOOR", nullptr, this);
		//	SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
		//	pATEvent = nullptr;
		//	m_fSwitchTimer = 3.0f;

		}
		else if(m_bPressurePlate == true)
		{
			//Open Door
			SGD::Event* pATEvent = new SGD::Event("OPEN_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 0.25f;
		}

	}


	if (pOther->GetType() == ENT_HAWK)
	{
		if (m_bPressurePlate == false && m_fSwitchTimer == 0.0f)
		{

			//Open Door
			SGD::Event* pATEvent = new SGD::Event("FLIP_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;

			pATEvent = new SGD::Event("FLIP_LASER", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 3.0f;
		}

	}

	if (pOther->GetType() == ENT_SWORD)
	{
		int x = 1;
	}



	if (m_pPlayer != nullptr)
	{
		
		RECT rSwingRect;
		rSwingRect.left =	m_pPlayer->GetSwingRect().left;
		rSwingRect.top =	m_pPlayer->GetSwingRect().top;
		rSwingRect.right =	m_pPlayer->GetSwingRect().right;
		rSwingRect.bottom = m_pPlayer->GetSwingRect().bottom;

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
			//Open Door
			SGD::Event* pATEvent = new SGD::Event("FLIP_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;

			pATEvent = new SGD::Event("FLIP_LASER", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 3.0f;
		}

		if (nIntersectHeight < nIntersectWidth)
		{
			//Open Door
			SGD::Event* pATEvent = new SGD::Event("FLIP_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;

			pATEvent = new SGD::Event("FLIP_LASER", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 3.0f;
		}

	}

}


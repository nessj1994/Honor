#include "Activator.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include <Windows.h>
#include "Player.h"
#include "SwordSwing.h"
#include "../SGD Wrappers/SGD_AudioManager.h"


Activator::Activator(bool isPressure) : Listener(this)
{
	Listener::RegisterForEvent("ResetRoom");

	m_bPressurePlate = isPressure;

	//m_ptPosition = { 68, 300 };
	m_szSize = { 32, 32 };

	m_hEffect = SGD::AudioManager::GetInstance()->LoadAudio("assets/audio/Switch.wav");

}


Activator::~Activator()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hEffect);
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
		&& GetType() == ENT_SWITCH  && m_fSwitchTimer <= 0.0f)
	{
		
		RECT rSwingRect;
		rSwingRect.left = (LONG)m_pPlayer->GetSword()->GetRect().left;
		rSwingRect.top = (LONG)m_pPlayer->GetSword()->GetRect().top;
		rSwingRect.right = (LONG)m_pPlayer->GetSword()->GetRect().right;
		rSwingRect.bottom = (LONG)m_pPlayer->GetSword()->GetRect().bottom;

		RECT rObject;
		rObject.left = (LONG)(GetRect().left - Camera::GetInstance()->GetCameraPos().x);
		rObject.top = (LONG)(GetRect().top - Camera::GetInstance()->GetCameraPos().y);
		rObject.right = (LONG)(GetRect().right - Camera::GetInstance()->GetCameraPos().x);
		rObject.bottom = (LONG)(GetRect().bottom - Camera::GetInstance()->GetCameraPos().y);

		RECT rIntersection = {};

		IntersectRect(&rIntersection, &rObject, &rSwingRect);

		int nIntersectWidth = rIntersection.right - rIntersection.left;
		int nIntersectHeight = rIntersection.bottom - rIntersection.top;

		if (nIntersectHeight < nIntersectWidth)
		{

			if(!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hEffect))
			{
				SGD::AudioManager::GetInstance()->PlayAudio(m_hEffect);
			}
			//Open Door
			SGD::Event* pATEvent = new SGD::Event("FLIP_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;

			pATEvent = new SGD::Event("FLIP_LASER", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 0.4f;
		}
		if (nIntersectHeight > nIntersectWidth)
		{


			if(!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hEffect))
			{
				SGD::AudioManager::GetInstance()->PlayAudio(m_hEffect);
			}
			//Open Door
			SGD::Event* pATEvent = new SGD::Event("FLIP_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;

			pATEvent = new SGD::Event("FLIP_LASER", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 0.4f;
		}
		if (nIntersectHeight == nIntersectWidth
			&& nIntersectHeight > 0)
		{

			if(!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hEffect))
			{
				SGD::AudioManager::GetInstance()->PlayAudio(m_hEffect);
			}
			//Open Door
			SGD::Event* pATEvent = new SGD::Event("FLIP_DOOR", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;

			pATEvent = new SGD::Event("FLIP_LASER", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			m_fSwitchTimer = 0.4f;
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
			m_fSwitchTimer = .5f;
		}

	}


	if (pOther->GetType() == ENT_HAWK)
	{
		if (m_bPressurePlate == false && m_fSwitchTimer == 0.0f)
		{

			if(!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hEffect))
			{
				SGD::AudioManager::GetInstance()->PlayAudio(m_hEffect);
			}
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



	if (m_pPlayer != nullptr )
	{
		
		RECT rSwingRect;
		rSwingRect.left = (LONG)m_pPlayer->GetSwingRect().left;
		rSwingRect.top = (LONG)m_pPlayer->GetSwingRect().top;
		rSwingRect.right = (LONG)m_pPlayer->GetSwingRect().right;
		rSwingRect.bottom = (LONG)m_pPlayer->GetSwingRect().bottom;

		RECT rObject;
		rObject.left = (LONG)(GetRect().left - Camera::GetInstance()->GetCameraPos().x);
		rObject.top = (LONG)(GetRect().top - Camera::GetInstance()->GetCameraPos().y);
		rObject.right = (LONG)(GetRect().right - Camera::GetInstance()->GetCameraPos().x);
		rObject.bottom = (LONG)(GetRect().bottom - Camera::GetInstance()->GetCameraPos().y);

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
			m_fSwitchTimer = 1.0f;
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
			m_fSwitchTimer = 1.0f;
		}

	}

}

////////////////////////////////////////////////
////////////// Listener Interface //////////////
void Activator::HandleEvent(const SGD::Event* pEvent)
{
	//which event
	// Restarting the room
	if (pEvent->GetEventID() == "ResetRoom")
	{
		m_bIsOn = m_bStartOn;
	}
}

#include "Door.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include <Windows.h>

Door::Door() : Listener(this)
{
	Listener::RegisterForEvent("OPEN_DOOR");
	Listener::RegisterForEvent("CLOSE_DOOR");
	Listener::RegisterForEvent("FLIP_DOOR");
	Listener::RegisterForEvent("ResetRoom");

	m_ptPosition = { 200, 150 };
	m_szSize = { 100, 200 };

	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/DoorObstacle.png");
	m_hOpenImage = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/DoorOpen.png");

}


Door::~Door()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hOpenImage);

}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Door::Update(float elapsedTime)
{
	//if(m_pActivator->isOn() == true)
	//{
	//	m_bOpen = true;
	//}
	//else if(m_pActivator->isOn() == false)
	//{
	//	m_bOpen = false;
	//}

	if (m_bOpen)
	{
		m_szSize = { 0, 0 };
	}
	else
	{
		if (m_bHorizontal)
		{
			//m_szSize = { 128.0f, 32.0f };
			m_szSize = { 192.0f, 32.0f };

		}
		else
		{
			//m_szSize = { 32, 128.0f };
			m_szSize = { 32, 192.0f };
		}
	}

}
void Door::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//Get our rectangle
	SGD::Rectangle rMyRect = GetRect();

	rMyRect.Offset({ -camPos.x, -camPos.y });

	//Camera::GetInstance()->Draw(rMyRect, { 255, 0, 0, 255 });

	if(m_bOpen == false && m_bHorizontal == false)
		Camera::GetInstance()->DrawTexture({ m_ptPosition.x + m_szSize.width, m_ptPosition.y }, 0.0f, m_hImage, false, 1.0f, {}, {});
	else if(m_bOpen == true && m_bHorizontal == false)
	{
		Camera::GetInstance()->DrawTexture({ m_ptPosition.x + m_szSize.width, m_ptPosition.y }, 0.0f, m_hOpenImage, false, 1.0f, {}, {});

	}
	else if(m_bOpen == false && m_bHorizontal == true)
	{
		Camera::GetInstance()->DrawTexture({ m_ptPosition.x + m_szSize.width - 50, m_ptPosition.y }, 1.57f, m_hImage, false, 1.0f, {}, {});

	}
	else if(m_bOpen == true && m_bHorizontal == true)
	{
		Camera::GetInstance()->DrawTexture({ m_ptPosition.x + m_szSize.width, m_ptPosition.y }, 1.57f, m_hOpenImage, false, 1.0f, {}, {});

	}

}

int Door::GetType(void) const
{
	return ENT_DOOR;

}

SGD::Rectangle Door::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}

////////////////////////////////////////////////
////////////// Listener Interface //////////////
void Door::HandleEvent(const SGD::Event* pEvent)
{
	//which event

	//Set the player back to his last checkpoint 
	//This is usually back to the level start
	if(pEvent->GetEventID() == "OPEN_DOOR")
	{
		Activator* pActivator = reinterpret_cast<Activator*>(pEvent->GetSender());
		if(pActivator->GetKeyID() == m_unKeyID)
		{
			m_bOpen = true;
		}
	}
	if(pEvent->GetEventID() == "CLOSE_DOOR")
	{

		Activator* pActivator = reinterpret_cast<Activator*>(pEvent->GetSender());
		if(pActivator->GetKeyID() == m_unKeyID)
		{
			m_bOpen = false;
		}
	}
	if(pEvent->GetEventID() == "FLIP_DOOR")
	{
		Activator* pActivator = reinterpret_cast<Activator*>(pEvent->GetSender());
		unsigned int i = pActivator->GetKeyID();
		if(pActivator->GetKeyID() == m_unKeyID)
		{
			m_bOpen = !m_bOpen;
		}
	}
	if (pEvent->GetEventID() == "ResetRoom")
	{
		m_bOpen = m_bStartOpen;
	}
}
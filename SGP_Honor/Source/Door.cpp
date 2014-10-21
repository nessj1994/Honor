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

	m_ptPosition = { 200, 150 };
	m_szSize = { 100, 200 };

}


Door::~Door()
{
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
		m_szSize = { 100, 200 };
	}

}
void Door::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//Get our rectangle
	SGD::Rectangle rMyRect = GetRect();

	rMyRect.Offset({ -camPos.x, -camPos.y });

	Camera::GetInstance()->Draw(rMyRect, { 255, 0, 0, 255 });
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
	if (pEvent->GetEventID() == "OPEN_DOOR")
	{
		Activator* pActivator = reinterpret_cast<Activator*>(pEvent->GetSender());
		if (pActivator->GetKeyID() == m_unKeyID)
		{
			m_bOpen = true;
		}
	}
	if (pEvent->GetEventID() == "CLOSE_DOOR")
	{

		Activator* pActivator = reinterpret_cast<Activator*>(pEvent->GetSender());
		if (pActivator->GetKeyID() == m_unKeyID)
		{
			m_bOpen = false;
		}
	}
	if (pEvent->GetEventID() == "FLIP_DOOR")
	{
		Activator* pActivator = reinterpret_cast<Activator*>(pEvent->GetSender());
		if (pActivator->GetKeyID() == m_unKeyID)
		{
			m_bOpen = !m_bOpen;
		}
	}
}
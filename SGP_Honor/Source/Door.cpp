#include "Door.h"
#include "Camera.h"

#include <Windows.h>

Door::Door()
{
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
	if(m_pActivator->isOn() == true)
	{
		m_bOpen = true;
	}
	else if(m_pActivator->isOn() == false)
	{
		m_bOpen = false;
	}

	if(m_bOpen)
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

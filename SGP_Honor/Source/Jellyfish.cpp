#include "Jellyfish.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#define BTime 0.2f

Jellyfish::Jellyfish() : Listener(this)
{
	Listener::RegisterForEvent("RESET_JELLYFISH_BOUNCE");
	m_ptPosition = { 200, 600 };
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/jellyfishPlaceholder.png");
	m_szSize = SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage);
}


Jellyfish::~Jellyfish()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

void Jellyfish::Update(float elapsedTime)
{
	if (bouncecounting == true)
	{
		bouncetimer += elapsedTime;
		if (bouncetimer >= BTime)
		{
			bouncetimer = 0;
			bouncecounting = false;
		}
	}
}

void Jellyfish::Render(void)
{
	////Get the camera position for our offset
	//SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	////create a reference to our rectangle
	//SGD::Rectangle rMyRect = GetRect();

	////Offset our rectangle by the camera position for rendering
	//rMyRect.Offset({ -camPos.x, -camPos.y });

	////Render us with the camera
	//Camera::GetInstance()->Draw(rMyRect,
	//	SGD::Color::Color(255, 255, 0, 0));
	Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false);
}

SGD::Rectangle Jellyfish::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Jellyfish::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == Entity::ENT_PLAYER && bouncecounting == false)
	{
		if (numOfBounces < 5)
			numOfBounces++;
		bouncecounting = true;
		SGD::Event Event = { "RESET_JELLYFISH_BOUNCE", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}
}

void Jellyfish::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "RESET_JELLYFISH_BOUNCE" && pEvent->GetSender() != this)
	{
		numOfBounces = 0;
	}
}
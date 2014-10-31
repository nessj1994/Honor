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
	//m_ptPosition = { 700, 700 };
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/Jellyfish.png");
	m_szSize = SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage) * Camera::GetInstance()->GetZoomScale();
	m_vtVelocity = SGD::Vector(20, 0);
}


Jellyfish::~Jellyfish()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

void Jellyfish::SetPatrol()
{
	patrolTotal = SGD::Vector({ m_ptPosition.x + patrolDistance.x, m_ptPosition.y + patrolDistance.y });
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
	SGD::Vector distance = SGD::Vector({ patrolTotal.x - m_ptPosition.x, patrolTotal.y - m_ptPosition.y });
	if (distance.ComputeLength() < 20)
	{
		patrolDistance = -patrolDistance;
		m_vtVelocity = -m_vtVelocity;
		SetPatrol();
	}
	Entity::Update(elapsedTime);
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
	Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false, 1, SGD::Color(255, 255, 255, 255), {});
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
	if (pOther->GetType() == Entity::ENT_SOLID_WALL)
	{
		patrolDistance = -patrolDistance;
		m_vtVelocity = -m_vtVelocity;
		SetPatrol();
	}
}

void Jellyfish::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "RESET_JELLYFISH_BOUNCE" && pEvent->GetSender() != this)
	{
		numOfBounces = 0;
	}
}
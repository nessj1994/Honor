#include "Pouncer.h"
#include "Camera.h"
#include "Player.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include <cmath>

Pouncer::Pouncer() : Listener(this)
{
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	m_ptPosition = { 800, 600 };
	m_szSize = { 32, 32 };
}


Pouncer::~Pouncer()
{
	SetTarget(nullptr);
}

void Pouncer::Update(float elapsedTime)
{
	if (target != nullptr)
	{
		float distance = target->GetPosition().x - m_ptPosition.x;
		if (fabsf(distance) < 250)
		{
			if (target->GetPosition().x < m_ptPosition.x)
				m_ptPosition.x -= 1;
			else if (target->GetPosition().x > m_ptPosition.x)
				m_ptPosition.x += 1;
		}
	}
}

void Pouncer::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({ -camPos.x, -camPos.y });

	//Render us with the camera
	Camera::GetInstance()->Draw(rMyRect,
		SGD::Color::Color(255, 255, 0, 0));
}

SGD::Rectangle Pouncer::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Pouncer::HandleCollision(const IEntity* pOther)
{

}

void Pouncer::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ASSESS_PLAYER_RANGE" && target == nullptr)
	{
		SetTarget((Player*)pEvent->GetSender());
	}
}

void Pouncer::SetTarget(Player* plr)
{
	if (target != nullptr)
		target->Release();
	target = plr;
	if (target != nullptr)
		target->AddRef();
}
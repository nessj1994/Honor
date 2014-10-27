#include "Squid.h"
#include "CreateProjectileMessage.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "Camera.h"
#include "Player.h"

#define ShootSpeed 3.0f

Squid::Squid() : Listener(this)
{
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	m_ptPosition = { 800, 300 };
	m_szSize = { 32, 32 };
}


Squid::~Squid()
{
	SetTarget(nullptr);
}

void Squid::Update(float elapsedTime)
{
	if (target != nullptr)
	{
		if (target->GetPosition().x <= m_ptPosition.x)
			m_vtDirection = { -1, 0 };
		else if (target->GetPosition().x > m_ptPosition.x)
			m_vtDirection = { 1, 0 };
	}

	shotTimer += elapsedTime;

	if (shotTimer >= ShootSpeed)
	{
		shotTimer = 0.0f;
		CreateProjectileMessage* pMsg = new CreateProjectileMessage(this);
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}

void Squid::Render(void)
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

SGD::Rectangle Squid::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Squid::HandleCollision(const IEntity* pOther)
{

}

void Squid::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ASSESS_PLAYER_RANGE" && target == nullptr)
	{
		SetTarget((Player*)pEvent->GetSender());
	}
}

void Squid::SetTarget(Player* plr)
{
	if (target != nullptr)
		target->Release();
	target = plr;
	if (target != nullptr)
		target->AddRef();
}
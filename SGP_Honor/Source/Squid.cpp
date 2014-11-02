#include "Squid.h"
#include "CreateGravProjectileMessage.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "Camera.h"
#include "Player.h"
#include "AnimationEngine.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#define ShootSpeed 1.2f

Squid::Squid() : Listener(this)
{
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	m_ptPosition = { 800, 200 };
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Squid.xml");
	m_ts.SetCurrAnimation("Squid Idle");
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
		{
			m_vtDirection = { -1, 0 };
			SetFacingRight(true);
		}
		else if (target->GetPosition().x > m_ptPosition.x)
		{
			m_vtDirection = { 1, 0 };
			SetFacingRight(false);
		}

		m_ts.SetCurrAnimation("Squid Shooting");
	}

	shotTimer += elapsedTime;

	if (shotTimer >= 0.7f && shotTimer < 0.7f + elapsedTime)
	{
		CreateGravProjectileMessage* pMsg = new CreateGravProjectileMessage(this);
		pMsg->QueueMessage();
		pMsg = nullptr;
	}

	if (shotTimer >= ShootSpeed)
	{
		shotTimer = 0.0f;
		m_ts.ResetCurrFrame();
		m_ts.SetPlaying(true);
	}

	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
}

void Squid::Render(void)
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

	Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, GetFacingRight(), 1.0f);
}

SGD::Rectangle Squid::GetRect(void) const
{
	//return SGD::Rectangle{ m_ptPosition, m_szSize };
	return AnimationEngine::GetInstance()->GetRect(m_ts, GetFacingRight(), 1, m_ptPosition);
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
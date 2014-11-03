#include "Squid.h"
#include "CreateGravProjectileMessage.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Camera.h"
#include "Player.h"
#include "AnimationEngine.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "DestroyEntityMessage.h"

#define ShootSpeed 1.2f

Squid::Squid() : Listener(this)
{
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	m_ptPosition = { 800, 200 };
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Squid.xml");
	m_ts.SetCurrAnimation("Squid Idle");
	spit = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/SquidSpit.wav");
	m_aDeath = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/SquidDeath.wav");
}


Squid::~Squid()
{
	SetTarget(nullptr);
	SGD::AudioManager::GetInstance()->UnloadAudio(spit);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_aDeath);
}

void Squid::Update(float elapsedTime)
{
	if (IsAlive())
	{
		Enemy::Update(elapsedTime);
		if (target != nullptr)
		{
			if (target->GetPosition().x <= m_ptPosition.x)
			{
				m_vtDirection = { -1, 0 };
				m_bFacingRight = true;
			}
			else if (target->GetPosition().x > m_ptPosition.x)
			{
				m_vtDirection = { 1, 0 };
				m_bFacingRight = false;
			}

			m_ts.SetCurrAnimation("Squid Shooting");
		}

		shotTimer += elapsedTime;

		if (shotTimer >= 0.7f && shotTimer < 0.7f + elapsedTime)
		{
			CreateGravProjectileMessage* pMsg = new CreateGravProjectileMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
			SGD::AudioManager::GetInstance()->PlayAudio(spit);
		}

		if (shotTimer >= ShootSpeed)
		{
			shotTimer = 0.0f;
			m_ts.ResetCurrFrame();
			m_ts.SetPlaying(true);
		}

		AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
	}
	else
	{
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_aDeath) == false)
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
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

	if (IsAlive())
	{
		Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, m_bFacingRight, 1);
	}
}

SGD::Rectangle Squid::GetRect(void) const
{
	//return SGD::Rectangle{ m_ptPosition, m_szSize };
	return AnimationEngine::GetInstance()->GetRect(m_ts, m_bFacingRight, 1, m_ptPosition);
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
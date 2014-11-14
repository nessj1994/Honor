#include "Pouncer.h"
#include "Camera.h"
#include "Player.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include <cmath>
#include <Windows.h>
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "AnimationEngine.h"
#include "DestroyEntityMessage.h"

#define MaxJumpTime 0.6f

Pouncer::Pouncer() : Listener(this)
{
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	Listener::RegisterForEvent("ResetRoom");
	//m_ptPosition = { 800, 600 };
	AnimationEngine::GetInstance()->LoadAnimation("Assets/HermitCrab.xml");
	m_ts.SetCurrAnimation("Hermit Crab Hide");
	m_ts.SetPlaying(true);
	//m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/hermitcrabPlaceholder.png");
	//m_szSize = SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage);
	SetGravity(-800.0f);
	m_bFacingRight = true;
	m_aPounce = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Pouncer.wav");
	m_aDeath = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/PouncerDeath.wav");
}


Pouncer::~Pouncer()
{
	SetTarget(nullptr);
	//SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_aPounce);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_aDeath);
}

void Pouncer::Update(float elapsedTime)
{
	if (GetAlive())
	{
		Enemy::Update(elapsedTime);
		if (target != nullptr)
		{
			SGD::Vector distance = target->GetPosition() - m_ptPosition;
			if (distance.ComputeLength() < 250 && distance.ComputeLength() > 80)
			{
				if (target->GetPosition().x < m_ptPosition.x && fabsf(distance.y) < 70 && inAir == false)
				{
					m_bFacingRight = true;
					SetVelocity({ -200, -300 });
					isPouncing = true;
					inAir = true;
					SGD::AudioManager::GetInstance()->PlayAudio(m_aPounce);
					m_ts.SetCurrAnimation("Hermit Crab Pounce");
					m_ts.ResetCurrFrame();
					m_ts.SetPlaying(true);
				}
				else if (target->GetPosition().x > m_ptPosition.x && fabsf(distance.y) < 70 && inAir == false)
				{
					m_bFacingRight = false;
					SetVelocity({ 200, -300 });
					isPouncing = true;
					inAir = true;
					SGD::AudioManager::GetInstance()->PlayAudio(m_aPounce);
					m_ts.SetCurrAnimation("Hermit Crab Pounce");
					m_ts.ResetCurrFrame();
					m_ts.SetPlaying(true);
				}
			}
			else if (distance.ComputeLength() < 80)
			{
				isPouncing = false;
			}
			if (isPouncing == false && inAir == false)
			{
				m_ts.SetCurrAnimation("Hermit Crab Hide");
				m_ts.ResetCurrFrame();
				m_ts.SetPlaying(true);
			}

			if (apex >= MaxJumpTime && isPouncing == true)
			{
				apex = 0.0f;
				isPouncing = false;
			}
			else if (apex < MaxJumpTime && isPouncing == true)
			{
				apex += elapsedTime;
			}

			SetVelocity({ GetVelocity().x, GetVelocity().y - GetGravity() * elapsedTime });

			Entity::Update(elapsedTime);
			AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
		}
	}
	else
	{
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_aDeath) == false)
		{
			//Reseting Enemys
			SetAlive(false);
			/*DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
			pMsg->QueueMessage();
			pMsg = nullptr;*/
		}
	}
}

void Pouncer::Render(void)
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
	//Enemy::Render();
	//Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false, 1, SGD::Color(255, 255, 255, 255), {});
	if (GetAlive())
	{
		Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, m_bFacingRight, 1);
	}
}

SGD::Rectangle Pouncer::GetRect(void) const
{
	//return SGD::Rectangle{ m_ptPosition, m_szSize };
	return AnimationEngine::GetInstance()->GetRect(m_ts, m_bFacingRight, 1, m_ptPosition);
}

void Pouncer::HandleCollision(const IEntity* pOther)
{

	if (GetAlive())
	{
		Unit::HandleCollision(pOther);

		if (pOther->GetType() == Entity::ENT_SOLID_WALL || pOther->GetType() == Entity::ENT_DOOR || pOther->GetType() == Entity::ENT_LEFT_RAMP
			|| pOther->GetType() == Entity::ENT_RIGHT_RAMP)
		{
			//SetVelocity({ 0, GetVelocity().y });
			RECT rPouncer;
			rPouncer.left = (LONG)GetRect().left;
			rPouncer.top = (LONG)GetRect().top;
			rPouncer.right = (LONG)GetRect().right;
			rPouncer.bottom = (LONG)GetRect().bottom;

			//Create a rectangle for the other object
			RECT rObject;
			rObject.left = (LONG)pOther->GetRect().left;
			rObject.top = (LONG)pOther->GetRect().top;
			rObject.right = (LONG)pOther->GetRect().right;
			rObject.bottom = (LONG)pOther->GetRect().bottom;

			//Create a rectangle for the intersection
			RECT rIntersection = {};

			IntersectRect(&rIntersection, &rPouncer, &rObject);

			int nIntersectWidth = rIntersection.right - rIntersection.left;
			int nIntersectHeight = rIntersection.bottom - rIntersection.top;

			//Colliding with the side of the object
			if (nIntersectHeight > nIntersectWidth)
			{
				if (rPouncer.right == rIntersection.right)
				{
					SetPosition({ (float)rObject.left - nIntersectWidth - 30, GetPosition().y });
					SetVelocity({ 0, GetVelocity().y });
				}
				if (rPouncer.left == rIntersection.left)
				{
					SetPosition({ (float)rObject.right + nIntersectWidth + 26, GetPosition().y });
					SetVelocity({ 0, GetVelocity().y });
				}
			}

			if (nIntersectWidth > nIntersectHeight)
			{
				if (rPouncer.bottom == rIntersection.bottom)
				{
					SetVelocity({ GetVelocity().x, 0 });
					SetPosition({ GetPosition().x, (float)rObject.top - nIntersectHeight + 1 });
					inAir = false;
				}
				if (rPouncer.top == rIntersection.top)
				{
					SetPosition({ GetPosition().x, (float)rObject.bottom + nIntersectHeight + 50 });
					SetVelocity({ GetVelocity().x, 0 });
				}
			}
		}

		if (pOther->GetType() == Entity::ENT_PLAYER && GetRect().IsIntersecting(pOther->GetRect()) == true)
		{
			//if so move back up but kill the player
			SGD::Event Event = { "KILL_PLAYER", nullptr, this };
			SGD::EventManager::GetInstance()->SendEventNow(&Event);
		}
		if (pOther->GetType() == ENT_DEATH)
		{
			SetAlive(false);
		}
	}
}

void Pouncer::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ResetRoom")
	{
		SetAlive(true);
		SetPosition(GetOriginalPos());
		SetVelocity({ 0, 0 });
		m_ts.SetCurrAnimation("Hermit Crab Hide");
		m_ts.ResetCurrFrame();
		m_ts.SetPlaying(true);
	}
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
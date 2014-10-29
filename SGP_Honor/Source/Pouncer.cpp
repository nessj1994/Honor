#include "Pouncer.h"
#include "Camera.h"
#include "Player.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include <cmath>
#include <Windows.h>
#include "../SGD Wrappers/SGD_GraphicsManager.h"

#define MaxJumpTime 0.6f

Pouncer::Pouncer() : Listener(this)
{
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	m_ptPosition = { 800, 600 };
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/hermitcrabPlaceholder.png");
	m_szSize = SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage);
	SetGravity(-500.0f);
}


Pouncer::~Pouncer()
{
	SetTarget(nullptr);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

void Pouncer::Update(float elapsedTime)
{
	if (target != nullptr)
	{
		SGD::Vector distance = target->GetPosition() - m_ptPosition;
		if (distance.ComputeLength() < 250)
		{
			if (target->GetPosition().x < m_ptPosition.x && fabsf(distance.y) < 40 && inAir == false)
			{
				m_bFacingRight = false;
				SetVelocity({ -200, -300 });
				isPouncing = true;
				inAir = true;
			}
			else if (target->GetPosition().x > m_ptPosition.x && fabsf(distance.y) < 40 && inAir == false)
			{
				m_bFacingRight = true;
				SetVelocity({ 200, -300 });
				isPouncing = true;
				inAir = true;
			}
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

		Unit::Update(elapsedTime);
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
	Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false);
}

SGD::Rectangle Pouncer::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Pouncer::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == Entity::ENT_SOLID_WALL)
		inAir = false;

	if (pOther->GetType() == Entity::ENT_PLAYER && GetRect().IsIntersecting(pOther->GetRect()) == true)
	{
		//if so move back up but kill the player
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}

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
			SetPosition({ (float)rObject.left - GetSize().width + 1, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
		if (rPouncer.left == rIntersection.left)
		{
			SetPosition({ (float)rObject.right, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (rPouncer.bottom == rIntersection.bottom)
		{
			SetVelocity({ GetVelocity().x, 0 });
			SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 /*- nIntersectHeight*/ });
		}
		if (rPouncer.top == rIntersection.top)
		{
			SetPosition({ GetPosition().x, (float)rObject.bottom });
			SetVelocity({ GetVelocity().x, 0 });
		}
	}
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
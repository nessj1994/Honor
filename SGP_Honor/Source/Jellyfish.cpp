#include "Jellyfish.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <Windows.h>
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "AnimationEngine.h"

#define BTime 0.2f

Jellyfish::Jellyfish() : Listener(this)
{
	Listener::RegisterForEvent("RESET_JELLYFISH_BOUNCE");
	//m_ptPosition = { 700, 700 };
	//m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/Jellyfish.png");
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Jellyfish.xml");
	m_ts.SetCurrAnimation("Jellyfish Moving");
	m_ts.SetPlaying(true);
	m_bFacingRight = false;
	m_szSize.width = 80;
	m_szSize.height = 64;

	//m_szSize = SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage) * Camera::GetInstance()->GetZoomScale();
	m_vtVelocity = SGD::Vector(20, 0);
}


Jellyfish::~Jellyfish()
{
	//SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
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
		m_bFacingRight = !m_bFacingRight;
		SetPatrol();
	}
	Entity::Update(elapsedTime);
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
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
	//Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false, 1, SGD::Color(255, 255, 255, 255), {});



	Camera::GetInstance()->Draw(SGD::Rectangle(
		(GetRect().left - Camera::GetInstance()->GetCameraPos().x),
		(GetRect().top  - Camera::GetInstance()->GetCameraPos().y),
		(GetRect().left - Camera::GetInstance()->GetCameraPos().x + GetSize().width),
		(GetRect().top - Camera::GetInstance()->GetCameraPos().y + GetSize().height)),
		SGD::Color::Color(255, 255, 0, 0));

	Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, m_bFacingRight, 1);

}

SGD::Rectangle Jellyfish::GetRect(void) const
{
	return SGD::Rectangle{ { m_ptPosition.x - 40, m_ptPosition.y - 32 }, m_szSize };
	//return AnimationEngine::GetInstance()->GetRect(m_ts, m_bFacingRight, 1, m_ptPosition);
}

void Jellyfish::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == Entity::ENT_PLAYER && bouncecounting == false)
	{
		if (numOfBounces < 3)
			numOfBounces++;
		bouncecounting = true;
		SGD::Event Event = { "RESET_JELLYFISH_BOUNCE", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}
	if (pOther->GetType() == Entity::ENT_SOLID_WALL)
	{
		RECT rjFish;
		rjFish.left = (LONG)GetRect().left;
		rjFish.top = (LONG)GetRect().top;
		rjFish.right = (LONG)GetRect().right;
		rjFish.bottom = (LONG)GetRect().bottom;

		//Create a rectangle for the other object
		RECT rObject;
		rObject.left = (LONG)pOther->GetRect().left;
		rObject.top = (LONG)pOther->GetRect().top;
		rObject.right = (LONG)pOther->GetRect().right;
		rObject.bottom = (LONG)pOther->GetRect().bottom;

		//Create a rectangle for the intersection
		RECT rIntersection = {};

		IntersectRect(&rIntersection, &rjFish, &rObject);

		int nIntersectWidth = rIntersection.right - rIntersection.left;
		int nIntersectHeight = rIntersection.bottom - rIntersection.top;

		//Colliding with the side of the object
		if (nIntersectHeight > nIntersectWidth)
		{
			if (rjFish.right == rIntersection.right)
			{
				SetPosition({ (float)rObject.left - GetSize().width / 2, GetPosition().y });
				patrolDistance = -patrolDistance;
				m_vtVelocity = -m_vtVelocity;
				m_bFacingRight = !m_bFacingRight;
				SetPatrol();
			}
			if (rjFish.left == rIntersection.left)
			{
				SetPosition({ (float)rObject.right + GetSize().width /2, GetPosition().y });
				patrolDistance = -patrolDistance;
				m_vtVelocity = -m_vtVelocity;
				m_bFacingRight = !m_bFacingRight;
				SetPatrol();
			}
		}
	}
}

void Jellyfish::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "RESET_JELLYFISH_BOUNCE" && pEvent->GetSender() != this)
	{
		numOfBounces = 1;
	}
}
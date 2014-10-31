#include "Crab.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "CreateHorizontalBubble.h"
#include "CreateVerticalBubble.h"

#define SlamCooldown 1.5f
#define BubbleCastLength 2.0f
#define BubbleSpawnRate 0.2f

Crab::Crab() : Listener(this)
{
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	//SetCurrentState(idle);
	SetCurrentState(bubbles);
	m_ptPosition = SGD::Point(350, 120);
	m_szSize = SGD::Size(1024, 512);
	m_szLeftArm = SGD::Size(128, 128);
	m_ptLeftArm = SGD::Point({ m_ptPosition.x, m_ptPosition.y });
	m_szRightArm = SGD::Size(128, 128);
	m_ptRightArm = SGD::Point({ m_ptPosition.x + m_szSize.width - m_szRightArm.width, m_ptPosition.y });
}


Crab::~Crab()
{
}

void Crab::Update(float elapsedTime)
{
	if (GetPlayer() != nullptr)
	{
		/*if (GetCurrentState() == idle)
		{
			if ((GetPlayer()->GetPosition().x < m_ptPosition.x && leftSlamTimer == 0) ||
				(GetPlayer()->GetPosition().x > m_ptPosition.x + m_szSize.width && rightSlamTimer == 0) &&
				GetPlayer()->GetPosition().y < m_ptPosition.y)
				SetCurrentState(slamming);
			else if (GetPlayer()->GetPosition().x >= m_ptPosition.x && GetPlayer()->GetPosition().x <= m_ptPosition.x + m_szSize.width &&
				GetPlayer()->GetPosition().y < m_ptPosition.y)
				SetCurrentState(swipping);
		}*/

		switch (GetCurrentState())
		{
		case idle:
			break;
		case slamming:
			break;
		case swipping:
			break;
		case bubbles:
		{
			bubbleTimer += elapsedTime;
			bubbleSpawn += elapsedTime;
			if (bubbleTimer <= BubbleCastLength && bubbleSpawn >= BubbleSpawnRate)
			{
				if (rand() % 2 == 0)
				{
					CreateHorizontalBubble* pMsg = new CreateHorizontalBubble(this);
					pMsg->QueueMessage();
					pMsg = nullptr;
				}
				else
				{
					CreateVerticalBubble* pMsg = new CreateVerticalBubble(this);
					pMsg->QueueMessage();
					pMsg = nullptr;
				}
				bubbleSpawn = 0.0f;
			}

			if (bubbleTimer >= BubbleCastLength)
			{
				bubbleTimer = 0;
				SetCurrentState(idle);
			}

			break;
		}
		case hurt:
			break;
		default:
			break;
		}
	}
}

void Crab::Render(void)
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

	//SGD::Rectangle rLeftArm = SGD::Rectangle(m_ptLeftArm, m_szLeftArm);
	//rLeftArm.Offset({ -camPos.x, -camPos.y });
	//Camera::GetInstance()->Draw(rLeftArm,
	//	SGD::Color::Color(255, 0, 0, 255));

	//SGD::Rectangle rRightArm = SGD::Rectangle(m_ptRightArm, m_szRightArm);
	//rRightArm.Offset({ -camPos.x, -camPos.y });
	//Camera::GetInstance()->Draw(rRightArm,
	//	SGD::Color::Color(255, 0, 0, 255));
}

void Crab::BasicCollision(const IEntity* pOther)
{

}

void Crab::HandleCollision(const IEntity* pOther)
{

}

void Crab::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ASSESS_PLAYER_RANGE" && GetPlayer() == nullptr)
	{
		SetPlayer((Entity*)pEvent->GetSender());
	}
}
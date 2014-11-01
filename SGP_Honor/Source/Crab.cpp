#include "Crab.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "CreateHorizontalBubble.h"
#include "CreateVerticalBubble.h"
#include "AnimationEngine.h"
#include "DestroyEntityMessage.h"
#include <Windows.h>

#define SlamCooldown 1.5f
#define BubbleCastLength 2.0f
#define BubbleSpawnRate 0.2f
#define BubbleCooldown 3.0f
#define SlamTime 1.3f
#define SwipeTime 1.3f
#define SwipeCooldown 1.5f
#define HitCooldown 0.5f

Crab::Crab() : Listener(this)
{
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	//SetCurrentState(idle);
	SetCurrentState(idle);

	AnimationEngine::GetInstance()->LoadAnimation("Assets/testBoss.xml");
	m_ts.SetCurrAnimation("Boss Idle");
	SetHitPoints(4);
	//m_ptPosition = SGD::Point(350, 120);
}


Crab::~Crab()
{
}

void Crab::Update(float elapsedTime)
{
	/*if (GetHitPoints() > 0)
	{
		if (GetPlayer() != nullptr)
		{
			if (castedLeftSlam == true)
			{
				leftSlamTimer += elapsedTime;
				if (leftSlamTimer >= SlamTime)
				{
					leftSlamTimer = 0;
					LeftSlamOnCD = true;
					m_ts.ResetCurrFrame();
					m_ts.SetCurrAnimation("Boss Idle");
					m_ts.SetPlaying(true);
					SetCurrentState(idle);
				}
			}
			if (castedRightSlam == true)
			{
				rightSlamTimer += elapsedTime;
				if (rightSlamTimer >= SlamTime)
				{
					rightSlamTimer = 0;
					RightSlamOnCD = true;
					m_ts.ResetCurrFrame();
					m_ts.SetCurrAnimation("Boss Idle");
					m_ts.SetPlaying(true);
					SetCurrentState(idle);
				}
			}
			if (castedSwipe == true)
			{
				swipeTimer += elapsedTime;
				if (swipeTimer >= SwipeTime)
				{
					swipeTimer = 0;
					SwipeOnCD = true;
					m_ts.ResetCurrFrame();
					m_ts.SetCurrAnimation("Boss Idle");
					m_ts.SetPlaying(true);
					SetCurrentState(idle);
				}
			}
			if (wasHit == true)
			{
				wasHitCD += elapsedTime;
				if (wasHitCD >= HitCooldown)
				{
					wasHitCD = 0.0f;
					wasHit = false;
				}
			}

			switch (GetCurrentState())
			{
			case idle:
			{
						 SGD::Rectangle rect = GetRect();
						 if ((GetPlayer()->GetPosition().x < rect.left ||
							 GetPlayer()->GetPosition().x > rect.right) &&
							 GetPlayer()->GetPosition().y > rect.top && castedLeftSlam == false && castedRightSlam == false)
							 SetCurrentState(slamming);
						 else if (GetPlayer()->GetPosition().x >= rect.left && GetPlayer()->GetPosition().x <= rect.right &&
							 GetPlayer()->GetPosition().y < m_ptPosition.y && castedSwipe == false)
							 SetCurrentState(swipping);
						 else if (castedBubbles == false)
							 SetCurrentState(bubbles);

						 if (castedBubbles == true)
						 {
							 bubbleCD += elapsedTime;
							 if (bubbleCD >= BubbleCooldown)
							 {
								 bubbleCD = 0;
								 castedBubbles = false;
							 }
						 }
						 if (SwipeOnCD == true)
						 {
							 swipeCD += elapsedTime;
							 if (swipeCD >= SwipeCooldown)
							 {
								 swipeCD = 0;
								 castedSwipe = false;
								 SwipeOnCD = false;
							 }
						 }
						 if (LeftSlamOnCD == true)
						 {
							 leftSlamCD += elapsedTime;
							 if (leftSlamCD >= SlamCooldown)
							 {
								 leftSlamCD = 0;
								 LeftSlamOnCD = false;
								 castedLeftSlam = false;
							 }
						 }
						 if (RightSlamOnCD == true)
						 {
							 rightSlamCD += elapsedTime;
							 if (rightSlamCD >= SlamCooldown)
							 {
								 rightSlamCD = 0;
								 RightSlamOnCD = false;
								 castedRightSlam = false;
							 }
						 }
						 break;
			}
			case slamming:
			{
							 SGD::Rectangle rect = GetRect();
							 if (GetPlayer()->GetPosition().x < rect.left && castedLeftSlam == false && castedRightSlam == false)
							 {
								 m_ts.ResetCurrFrame();
								 m_ts.SetCurrAnimation("Boss Left Slam");
								 m_ts.SetPlaying(true);
								 castedLeftSlam = true;
							 }
							 if (GetPlayer()->GetPosition().x > rect.right && castedLeftSlam == false && castedRightSlam == false)
							 {
								 m_ts.ResetCurrFrame();
								 m_ts.SetCurrAnimation("Boss Right Slam");
								 m_ts.SetPlaying(true);
								 castedRightSlam = true;
							 }
							 break;
			}
			case swipping:
			{
							 if (castedSwipe == false && castedLeftSlam == false && castedRightSlam == false)
							 {
								 m_ts.ResetCurrFrame();
								 m_ts.SetCurrAnimation("Boss Swipe");
								 m_ts.SetPlaying(true);
								 castedSwipe = true;
							 }
							 break;
			}
			case bubbles:
			{
							if (bubbleCD == 0.0f)
							{
								castedBubbles = true;
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
							}

							break;
			}
			case hurt:
			{
						 SetHitPoints(GetHitPoints() - 1);
						 SGD::Event* pATEvent = new SGD::Event("FLIP_LASER", nullptr, this);
						 SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
						 pATEvent = nullptr;
						 wasHit = true;
						 SetCurrentState(idle);
						 break;
			}
			default:
				break;
			}
		}
		AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
	}
	else
	{
		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;
	}*/
}

void Crab::Render(void)
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

	Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, IsFacingRight(),1.0f);
}

SGD::Rectangle Crab::GetRect(void) const
{
	return AnimationEngine::GetInstance()->GetRect(m_ts, IsFacingRight(), 1.0f, m_ptPosition);
}

void Crab::BasicCollision(const IEntity* pOther)
{

}

void Crab::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_LASER)
	{
		RECT rBoss;
		rBoss.left = (LONG)GetRect().left;
		rBoss.top = (LONG)GetRect().top;
		rBoss.right = (LONG)GetRect().right;
		rBoss.bottom = (LONG)GetRect().bottom;

		//Create a rectangle for the other object
		RECT rObject;
		rObject.left = (LONG)pOther->GetRect().left;
		rObject.top = (LONG)pOther->GetRect().top;
		rObject.right = (LONG)pOther->GetRect().right;
		rObject.bottom = (LONG)pOther->GetRect().bottom;

		//Create a rectangle for the intersection
		RECT rIntersection = {};


		IntersectRect(&rIntersection, &rBoss, &rObject);


		int nIntersectWidth = rIntersection.right - rIntersection.left;
		int nIntersectHeight = rIntersection.bottom - rIntersection.top;

		//if (nIntersectHeight > nIntersectWidth)
		//{
		//	if (rBoss.right == rIntersection.right)
		//	{
		//		SetCurrentState(hurt);
		//	}
		//	if (rBoss.left == rIntersection.left)
		//	{
		//		SetCurrentState(hurt);
		//	}
		//}

		if (nIntersectWidth > nIntersectHeight)
		{
			/*if (rBoss.bottom == rIntersection.bottom)
			{
				SetCurrentState(hurt);
			}*/
			if (rBoss.top == rIntersection.top)
			{
				if (wasHit == false)
				{
					SetCurrentState(hurt);
				}
			}
		}
	}
}

void Crab::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ASSESS_PLAYER_RANGE" && GetPlayer() == nullptr)
	{
		SetPlayer((Entity*)pEvent->GetSender());
	}
}
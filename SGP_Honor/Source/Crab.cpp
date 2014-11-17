#include "Crab.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "CreateHorizontalBubble.h"
#include "CreateVerticalBubble.h"
#include "AnimationEngine.h"
#include "DestroyEntityMessage.h"
#include <Windows.h>
#include "Player.h"
#include "GameplayState.h"

#define SlamCooldown 1.5f
#define BubbleCastLength 2.0f
#define BubbleSpawnRate 0.2f
#define BubbleCooldown 3.0f
#define SlamTime 2.2f
#define SwipeTime 3.2f
#define SwipeCooldown 1.5f
#define HitCooldown 0.5f
#define Death 2.0f
#define RoarWait 20.0f

Crab::Crab() : Listener(this)
{
	Listener::RegisterForEvent("ASSESS_PLAYER_RANGE");
	Listener::RegisterForEvent("KILL_PLAYER");
	Listener::RegisterForEvent("ResetRoom");
	SetCurrentState(idle);

	AnimationEngine::GetInstance()->LoadAnimation("Assets/KingClang.xml");
	m_ts.SetCurrAnimation("Clang Idle");
	SetHitPoints(4);

	// Sound
	m_hRoar = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/CrabRoar.wav");
	m_hBubble = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Bubble.wav");
	m_hSlam2 = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Slam2.wav");
	m_hHurt = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/ClangHurt.wav");
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(L"Assets/graphics/ClangEyes.png");
	m_hSwipe = SGD::AudioManager::GetInstance()->LoadAudio(L"assets/audio/swordswing.wav");
}


Crab::~Crab()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hRoar);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hBubble);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hSlam2);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hHurt);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hSwipe);
}

void Crab::Update(float elapsedTime)
{
	if (StartWaitTime >= 2.0f)
	{
		if (GetHitPoints() > 0)
		{
			if (GetPlayer() != nullptr)
			{
				if (castedLeftSlam == true && castedRightSlam == false && castedSwipe == false && LeftSlamOnCD == false)
				{
					leftSlamTimer += elapsedTime;
					if ((leftSlamTimer >= 1.2f && leftSlamTimer < 1.4f) && !(SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hSlam2)))
					{
						SGD::AudioManager::GetInstance()->PlayAudio(m_hSlam2);
					}
					if (leftSlamTimer >= SlamTime)
					{
						leftSlamTimer = 0.0f;
						LeftSlamOnCD = true;
						m_ts.ResetCurrFrame();
						m_ts.SetCurrAnimation("Clang Idle");
						m_ts.SetPlaying(true);
						SetCurrentState(idle);
					}
				}
				if (castedRightSlam == true && castedLeftSlam == false && castedSwipe == false && RightSlamOnCD == false)
				{
					rightSlamTimer += elapsedTime;
					if ((rightSlamTimer >= 1.2f && rightSlamTimer < 1.4f) && !(SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hSlam2)))
					{
						SGD::AudioManager::GetInstance()->PlayAudio(m_hSlam2);
					}
					if (rightSlamTimer >= SlamTime)
					{
						rightSlamTimer = 0.0f;
						RightSlamOnCD = true;
						m_ts.ResetCurrFrame();
						m_ts.SetCurrAnimation("Clang Idle");
						m_ts.SetPlaying(true);
						SetCurrentState(idle);
					}
				}
				if (castedSwipe == true && castedRightSlam == false && castedLeftSlam == false && SwipeOnCD == false)
				{
					swipeTimer += elapsedTime;
					if (swipeTimer >= SwipeTime)
					{
						swipeTimer = 0.0f;
						SwipeOnCD = true;
						m_ts.ResetCurrFrame();
						m_ts.SetCurrAnimation("Clang Idle");
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
						SetCurrentState(idle);
						m_ts.SetCurrAnimation("Clang Idle");
						m_ts.SetPlaying(true);
						m_ts.ResetCurrFrame();
					}
				}

				switch (GetCurrentState())
				{
				case idle:
				{
							 SGD::Rectangle rect = GetRect();
							 if ((GetPlayer()->GetPosition().x < rect.left ||
								 GetPlayer()->GetPosition().x > rect.right) &&
								 GetPlayer()->GetPosition().y > rect.top && castedLeftSlam == false && castedRightSlam == false && castedSwipe == false)
								 SetCurrentState(slamming);
							 else if (GetPlayer()->GetPosition().x >= rect.left && GetPlayer()->GetPosition().x <= rect.right &&
								 GetPlayer()->GetPosition().y < m_ptPosition.y && castedSwipe == false && castedLeftSlam == false && castedRightSlam == false && SwipeOnCD == false)
								 SetCurrentState(swipping);
							 else if (castedBubbles == false/* && castedSwipe == true && ( castedLeftSlam == true || castedRightSlam == true)*/)
							 {
								 SGD::AudioManager::GetInstance()->PlayAudio(m_hBubble);
								 SetCurrentState(bubbles);
								 redEyes = true;
							 }

							 if (castedBubbles == true)
							 {
								 bubbleCD += elapsedTime;
								 if (bubbleCD >= BubbleCooldown)
								 {
									 bubbleCD = 0.0f;
									 castedBubbles = false;
								 }
							 }
							 if (SwipeOnCD == true)
							 {
								 swipeCD += elapsedTime;
								 if (swipeCD >= SwipeCooldown)
								 {
									 swipeCD = 0.0f;
									 castedSwipe = false;
									 SwipeOnCD = false;
								 }
							 }
							 if (LeftSlamOnCD == true)
							 {
								 leftSlamCD += elapsedTime;
								 if (leftSlamCD >= SlamCooldown)
								 {
									 leftSlamCD = 0.0f;
									 LeftSlamOnCD = false;
									 castedLeftSlam = false;
								 }
							 }
							 if (RightSlamOnCD == true)
							 {
								 rightSlamCD += elapsedTime;
								 if (rightSlamCD >= SlamCooldown)
								 {
									 rightSlamCD = 0.0f;
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
									 m_ts.SetCurrAnimation("Clang Left Slam");
									 m_ts.SetPlaying(true);
									 castedLeftSlam = true;
								 }
								 if (GetPlayer()->GetPosition().x > rect.right && castedLeftSlam == false && castedRightSlam == false)
								 {
									 m_ts.ResetCurrFrame();
									 m_ts.SetCurrAnimation("Clang Right Slam");
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
									 m_ts.SetCurrAnimation("Clang Swipe");
									 m_ts.SetPlaying(true);
									 castedSwipe = true;
									 SGD::AudioManager::GetInstance()->PlayAudio(m_hSwipe);
								 }
								 break;
				}
				case bubbles:
				{
								if (redEyeTimer >= 0.5f)
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
											if (redEyeCD >= 0.5f)
											{
												bubbleTimer = 0.0f;
												SetCurrentState(idle);
												redEyes = false;
												redEyeCD = 0.0f;
												redEyeTimer = 0.0f;
												eyealpha = 0;
											}
											else
											{
												redEyeCD += elapsedTime;
												eyealpha -= (unsigned char)(200 * elapsedTime);
											}
										}
									}
								}
								else
								{
									redEyeTimer += elapsedTime;
									eyealpha += (unsigned char)(255 * elapsedTime);
								}
								break;
				}
				case hurt:
				{
							 if (m_ts.GetCurrAnimation() != "Clang Hurt")
							 {
								 SetHitPoints(GetHitPoints() - 1);
								 SGD::Event* pATEvent = new SGD::Event("FLIP_LASER", nullptr, this);
								 SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
								 pATEvent = nullptr;
								 wasHit = true;
								 m_ts.SetCurrAnimation("Clang Hurt");
								 m_ts.SetPlaying(true);
								 m_ts.ResetCurrFrame();
								 SGD::AudioManager::GetInstance()->PlayAudio(m_hHurt);
								 castedLeftSlam = false;
								 castedRightSlam = false;
								 castedSwipe = false;
								 castedBubbles = false;
								 LeftSlamOnCD = false;
								 RightSlamOnCD = false;
								 leftSlamTimer = 0.0f;
								 rightSlamTimer = 0.0f;
								 swipeTimer = 0.0f;
								 bubbleTimer = 0.0f;
								 bubbleSpawn = 0.0f;
								 leftSlamCD = 0.0f;
								 rightSlamCD = 0.0f;
								 swipeCD = 0.0f;
								 bubbleTimer = 0.0f;
								 eyealpha = 0;
								 redEyeTimer = 0.0f;
								 SGD::AudioManager::GetInstance()->StopAudio(m_hSlam2);
								 SGD::AudioManager::GetInstance()->StopAudio(m_hBubble);
							 }
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
			if (deathTime >= Death)
			{
				GetPlayer()->SetHasBounce(true);
				GameplayState::GetInstance()->CreateTeleporter((int)m_ptPosition.x, (int)m_ptPosition.y - 30, "Level5_1", false);
				DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
				pMsg->QueueMessage();
				pMsg = nullptr;
			}
			else
			{
				deathTime += elapsedTime;
				alpha -= (unsigned char)(80 * elapsedTime);
			}
		}
	}
	else
	{
		StartWaitTime += elapsedTime;
	}

	if (GetHitPoints() > 0 && roarTimer >= RoarWait)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hRoar);
		roarTimer = 0.0f;
	}
	else if (GetHitPoints() > 0 && roarTimer < RoarWait)
		roarTimer += elapsedTime;
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

	Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, IsFacingRight(), 1.0f, {}, 
		SGD::Color(alpha, 255, 255 - ((4 - GetHitPoints()) * 60), 255 - ((4 - GetHitPoints()) * 60)));

	if (redEyes == true)
	{
		Camera::GetInstance()->DrawTexture({ m_ptPosition.x - 155, m_ptPosition.y - 142 }, 0, m_hImage, false, 1, SGD::Color(eyealpha, 255, 255, 255), {});
	}
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
				if (wasHit == false)
				{
					SetCurrentState(hurt);
				}
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

	if (pOther->GetType() == ENT_PLAYER)
	{
		SGD::Rectangle atkRect = AnimationEngine::GetInstance()->GetAttackRect(m_ts, IsFacingRight(), 1, m_ptPosition);

		if (pOther->GetRect().IsIntersecting(atkRect) == true)
		{
			SGD::Event Event = { "KILL_PLAYER", nullptr, this };
			SGD::EventManager::GetInstance()->SendEventNow(&Event);
		}
	}
}

void Crab::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ASSESS_PLAYER_RANGE" && GetPlayer() == nullptr)
	{
		SetPlayer((Player*)pEvent->GetSender());
	}

	if (pEvent->GetEventID() == "KILL_PLAYER" || pEvent->GetEventID() == "ResetRoom")
	{
		if (GetPlayer()->HasArmor() == false)
		{
			SetHitPoints(4);
			StartWaitTime = 0.0f;
			m_ts.SetCurrAnimation("Clang Idle");
			m_ts.SetPlaying(true);
			m_ts.ResetCurrFrame();
			SetCurrentState(idle);
			castedLeftSlam = false;
			castedRightSlam = false;
			castedSwipe = false;
			castedBubbles = false;
			LeftSlamOnCD = false;
			RightSlamOnCD = false;
			leftSlamTimer = 0.0f;
			rightSlamTimer = 0.0f;
			swipeTimer = 0.0f;
			bubbleTimer = 0.0f;
			bubbleSpawn = 0.0f;
			leftSlamCD = 0.0f;
			rightSlamCD = 0.0f;
			bubbleCD = 0.0f;
			swipeCD = 0.0f;
			roarTimer = 60.0f;
			eyealpha = 0;
			redEyeTimer = 0.0f;
			SGD::AudioManager::GetInstance()->StopAudio(m_hSlam2);
			SGD::AudioManager::GetInstance()->StopAudio(m_hBubble);
			SGD::AudioManager::GetInstance()->StopAudio(m_hRoar);
			SGD::AudioManager::GetInstance()->StopAudio(m_hHurt);
		}
	}
}
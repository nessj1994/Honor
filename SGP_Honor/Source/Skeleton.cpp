#include "Skeleton.h"
#include "AnimationEngine.h"
#include "DestroyEntityMessage.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Player.h"
#include "Camera.h"
#include "CreateGravProjectileMessage.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

/////////////////////////////
// Ctor/Dtor
Skeleton::Skeleton() : Listener(this)
{
	Listener::RegisterForEvent("ResetRoom");
	m_szSize = { 64, 64 };
	m_fShootTimer = 1.0f;
	SetFacingRight(true);
	m_ssCurrState = SS_IDLE;
	AnimationEngine::GetInstance()->LoadAnimation("Assets/Skeleton_Animation.xml");
	m_ts.SetCurrAnimation("Skeleton_Idle");
	m_hThrow = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/BoneThrow.wav");
	m_hDeath = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/BoneBreak.wav");
}

Skeleton::~Skeleton()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hThrow);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hDeath);
}

/////////////////////////////
// Update
// -Main update loop
void Skeleton::Update(float elapsedTime)
{
	m_unPrevFrame = m_ts.GetCurrFrame();
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
	SGD::AudioManager * pAudio = SGD::AudioManager::GetInstance();

	// only play audio if the player is close
	float playerDeltaX = abs(GetPlayer()->GetPosition().x - m_ptPosition.x);
	float playerDeltaY = abs(GetPlayer()->GetPosition().y - m_ptPosition.y);
	m_bPlayAudio = (playerDeltaX < 600.0f && playerDeltaY < 400.0f);	

	// Check if dying
	if (!GetAlive() && !m_bDying)
	{
		m_bDying = true;
		m_fDeathTimer = 0.5f;
		m_ssCurrState = SS_DEATH;
		m_ts.ResetCurrFrame();
		// play sound
		if (m_bPlayAudio)
		{
			pAudio->PlayAudio(m_hDeath);
		}
	}

	

	// Update direction for bones
	if (GetFacingRight())
	{
		SetDirection({ 1, 0 });
	}
	else
	{
		SetDirection({ -1, 0 });
	}

	// Update the current state
	switch (m_ssCurrState)
	{
		case SS_IDLE:
		{
			// Update animation
			m_ts.SetCurrAnimation("Skeleton_Idle");
			m_ts.SetPlaying(true);
			m_ts.SetSpeed(1.0f);

			// Turn to face the player
			float playerX = GetPlayer()->GetPosition().x;
			SetFacingRight(playerX > m_ptPosition.x + 32);

			// Start throwing if the player is close enough
			if (abs(playerX - m_ptPosition.x + 32) < 512.0f)
			{
				m_fShootTimer = 2.0f;
				m_ts.ResetCurrFrame();
				m_ssCurrState = SS_THROWING;
			}

			break;
		}
		case SS_THROWING:
		{
			// Change animation if throwing or not
			if (m_bThrowing)
			{
				m_ts.SetCurrAnimation("Skeleton_Throw");
				m_ts.SetPlaying(true);
				m_ts.SetSpeed(0.25f);

				// Throw bone
				if (m_ts.GetCurrFrame() == 2 && m_bCanThrow)
				{
					CreateGravProjectileMessage* pMsg = new CreateGravProjectileMessage(this);
					pMsg->QueueMessage();
					pMsg = nullptr;
					m_bCanThrow = false;
					// play sound
					if (m_bPlayAudio && m_unPrevFrame != 2)
					{
						pAudio->PlayAudio(m_hThrow);
					}
				}
				// End on the last frame
				if (m_ts.GetCurrFrame() == 3)
				{
					m_bCanThrow = true;
					m_bThrowing = false;
				}
			}
			else // Idle animation
			{
				m_ts.SetCurrAnimation("Skeleton_Idle");
				m_ts.SetPlaying(true);
				m_ts.SetSpeed(1.0f);

				// Update shooting timer
				m_fShootTimer -= elapsedTime;
				if (m_fShootTimer <= 0.0f)
				{
					m_ts.ResetCurrFrame();
					m_fShootTimer = 2.0f;
					m_bThrowing = true;
				}
			}

			// Stop throwing when the player is far away
			float playerX = GetPlayer()->GetPosition().x;
			SetFacingRight(playerX > m_ptPosition.x + 32);
			if (abs(playerX - m_ptPosition.x + 32) > 512.0f)
			{
				m_ssCurrState = SS_IDLE;
			}

			break;
		}
		case SS_DEATH:
		{
			// Update animation
			m_ts.SetCurrAnimation("Skeleton_Death");
			m_ts.SetPlaying(true);
			m_ts.SetSpeed(1.0f);

			// Timer to death
			m_fDeathTimer -= elapsedTime;
			if (m_fDeathTimer <= 0.0f)
			{
				//Reseting Enemys
				SetAlive(false);
				/*DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
				pMsg->QueueMessage();
				pMsg = nullptr;*/
			}

			break;
		}
	}

	//m_fShootTimer -= elapsedTime;
	//if (m_fShootTimer <= 0.0f)
	//{
	//	m_fShootTimer = 1.0f;

	//	CreateGravProjectileMessage* pMsg = new CreateGravProjectileMessage(this);
	//	pMsg->QueueMessage();
	//	pMsg = nullptr;
	//}

	Enemy::Update(elapsedTime);
}

/////////////////////////////
// Render
// -Main render loop
void Skeleton::Render()
{
	Enemy::Render();
	SGD::Point newPosition = m_ptPosition;
	newPosition.y += 64;
	newPosition.x += 32;
	Camera::GetInstance()->DrawAnimation(newPosition, 0, m_ts, !GetFacingRight(), 1.0f);
}

/////////////////////////////
// HandleCollision
// -Handles collision between this entity and another
void Skeleton::HandleCollision(const IEntity * pOther)
{
	if (pOther->GetType() == ENT_SOLID_WALL)
	{
		BasicCollision(pOther);
	}

	if (pOther->GetType() == ENT_DOOR)
	{
		BasicCollision(pOther);
	}
}

/////////////////////////////
// HandleEvent
// -Handles the given event
void Skeleton::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ResetRoom")
	{
		m_ssCurrState = SS_IDLE;
		m_bDying = false;
		SetAlive(true);
		SetPosition(GetOriginalPos());
	}

}
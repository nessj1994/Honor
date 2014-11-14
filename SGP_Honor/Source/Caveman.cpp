#include "Caveman.h"
#include "Hawk.h"
#include "CreateStalactite.h"
#include "GameplayState.h"
#include "DestroyEntityMessage.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "ParticleEngine.h"
#include "AnimationEngine.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Camera.h"
#include <random>

Caveman::Caveman() : SGD::Listener(this)
{
	Listener::RegisterForEvent("CaveMan_Door");
	Listener::RegisterForEvent("CaveMan_Jump_Right");
	Listener::RegisterForEvent("CaveMan_Jump_Left");
	Listener::RegisterForEvent("CaveMan_Stop");
	Listener::RegisterForEvent("FLIP_LASER");
	Listener::RegisterForEvent("KILL_PLAYER");
	SetSize({ 64.0f, 96.0f });
	m_szSize = { 64, 96 };
	m_bFacingRight = false;
	m_bsCurrState = CM_THINKING;
	SetHitPoints(3);
	m_fThinkingTimer = 0;
	SetGravity(-500);
	m_iDoorSkip = 1;
	m_fStopTimer = 0;
	m_fDoorTimer = 0;
	m_fStalacTimer = 0;
	m_pHawk = new Hawk();
	m_pHawk->SetOwner(this);
	m_hHawkExplode = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/FeatherExplosion.xml", "FeatherExplosion", m_ptPosition);
	m_emEYES = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/EYEEffect.xml", "RedEyes", m_ptPosition);
	m_emVictoryEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/VictoryEmitter.xml", "VictoryEffect", m_ptPosition);
	AnimationEngine::GetInstance()->LoadAnimation("Assets/CaveManAnim.xml");
	m_ts.SetCurrAnimation("CaveManWalking");
	m_ts.SetPlaying(true);
	m_bLaserOn = false;

	// Sound
	m_hGrunt1 = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Grunt1.wav");
	m_hGrunt2 = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Grunt2.wav");
	m_hGrunt3 = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Grunt3.wav");
	m_hJump = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Jump.wav");
	m_hLand = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Land.wav");
	m_hKick = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Kick.wav");

}


Caveman::~Caveman()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hGrunt1);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hGrunt2);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hGrunt3);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hJump);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hLand);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hKick);
	delete m_emEYES;
	delete m_emVictoryEffect;
	delete m_hHawkExplode;
}

void Caveman::Update(float elapsedTime)
{

	//Emitter updates
	m_hHawkExplode->Update(elapsedTime);
	m_emEYES->Update(elapsedTime);
	//
	//Animation Update
	AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);
	//Timer additions 
	m_fHitTimer += elapsedTime;
	m_fThinkingTimer += elapsedTime;
	m_fInTheAir += elapsedTime;
	m_fDoorTimer += elapsedTime;
	m_fStopTimer += elapsedTime;
	m_fHidingTimer += elapsedTime;
	m_fAttackingTimer += elapsedTime;
	m_fStalacTimer += elapsedTime;
	m_fStunnedTimer += elapsedTime;
	m_fSwitchJumpTimer += elapsedTime;
	//

	//Slash at player if close and not jumping or falling or attacking or emerging or hideing
	if (m_bsCurrState == CM_RUNING && !GetPlayer()->GetSnared())
	{
		//Find Distance to point
		SGD::Vector distance = m_ptPosition - GetPlayer()->GetPosition();
		if (distance.ComputeLength() < 50)
		{
			//m_bsCurrState = CM_SLASHING;
		}
			
	}

	//Hawk Update
	m_pHawk->Update(elapsedTime);
	//Drop stalactite if hawk is ready
	DropStalactites();
	//Die if zero HitPoints
	if (GetHitPoints() == 0 && m_bsCurrState != CM_JUMPING && m_bsCurrState != CM_FALLING && m_bsCurrState != CM_STUNNED)
	{
		m_bsCurrState = CM_DEATH;
	}
	//Switch statement inisialisations
	int Door;
	//AI State MAchine
	switch (m_bsCurrState)
	{
	case Caveman::CM_SLASHING:
		SetVelocity({ 0, 0 });
		if (m_ts.GetCurrAnimation() != "CaveManStunned")
		{
			m_ts.ResetCurrFrame();
			m_ts.SetCurrAnimation("CaveManStunned");
			m_ts.SetPlaying(true);
			GetPlayer()->SetSnared(true);
		}
		if (m_ts.GetCurrFrame() == 4)
		{			
			m_bsCurrState = CM_RUNING;
		}
		break;
	case Caveman::CM_RUNING:
		if (m_ts.GetCurrAnimation() != "CaveManWalking")
		{
			m_ts.ResetCurrFrame();
			m_ts.SetCurrAnimation("CaveManWalking");
			m_ts.SetPlaying(true);
		}
		if (m_bFacingRight)
		{
			SetVelocity({200,0});
		}
		else
		{
			SetVelocity({-200,0});
		}
		break;
	case Caveman::CM_JUMPING:
		if (m_ts.GetCurrAnimation() != "CaveManJumping")
		{
			m_ts.ResetCurrFrame();
			m_ts.SetCurrAnimation("CaveManJumping");
			m_ts.SetPlaying(true);
		}
		if (m_fInTheAir < .5f)
		{
			if (m_bFacingRight)
			{
				SetVelocity({ 200, -200 });
			}
			else
			{
				SetVelocity({ -200, -200 });
			}
		}
		else
		{
			m_bsCurrState = CM_FALLING;
		}
		break;
	case Caveman::CM_HIDING:
		//hide the caveman in a place far away
		m_ptPosition = { -1000, -1000 };
		//If Done Hideing Emerege at a random door
		if (m_fHidingTimer > .8f)
		{
			m_fHidingTimer = 0;
			Door = rand() % (5 - (1 + 1)) + 1;
			m_fDoorTimer = 0;
			switch (Door)
			{
			case 1:
				SetPosition({ 227, 500 });
				break;
			case 2:
				SetPosition({ 510, 500 });
				break;
			case 3:
				SetPosition({ 798, 500 });
				break;
			default:
				break;
			}
			m_bsCurrState = CM_EMERGING;
		}
		break;
	case Caveman::CM_EMERGING:
		m_bsCurrState = CM_ATTACKING;
		m_fAttackingTimer = 0;
		m_pHawk->SetPosition(m_ptPosition);
		break;
	case Caveman::CM_FALLING:
		break;
	case Caveman::CM_ATTACKING:		
		if (m_ts.GetCurrAnimation() != "CaveManAttack")
		{
			m_ts.ResetCurrFrame();
			m_ts.SetCurrAnimation("CaveManAttack");
			m_ts.SetPlaying(true);
		}
		if (GetPlayer()->IsFacingRight())
		{
			m_pHawk->Attack({ GetPlayer()->GetPosition().x + 300, 160 },GetPlayer()->IsFacingRight());
		}
		else
		{
			m_pHawk->Attack({ GetPlayer()->GetPosition().x - 300, 160 }, GetPlayer()->IsFacingRight());
		}		
		m_fStalacTimer = 0;
		SetVelocity({ 0, 0 });
		if (m_fAttackingTimer > 1)
		{
			m_fAttackingTimer = 0;
			m_bsCurrState = CM_RUNING;
		}
		break;
	case Caveman::CM_THINKING:
		SetVelocity({ 0, 0 });
		if (m_fThinkingTimer > .2f)
		{
			m_bsCurrState = CM_RUNING;
		}
		break;
	case Caveman::CM_STUNNED:
		if (m_ts.GetCurrAnimation() != "CaveManStunned")
		{
			m_ts.ResetCurrFrame();
			m_ts.SetCurrAnimation("CaveManStunned");
			m_ts.SetPlaying(true);
		}
		if (m_bFacingRight)
		{
			SetVelocity({ -350, -50 });
		}
		else
		{
			SetVelocity({ 350, -50 });
		}
		if (m_fStunnedTimer > .4f)
		{
			m_bFacingRight ? m_bFacingRight = false : m_bFacingRight = true;
			m_bsCurrState = CM_FALLING;
		}
		break;
	case Caveman::CM_DEATH:
		if (m_ts.GetCurrAnimation() != "CaveManDeath")
		{
			m_ts.ResetCurrFrame();
			m_ts.SetCurrAnimation("CaveManDeath");
			m_ts.SetPlaying(true);
		}
		SetVelocity({ 0, 0 });	
		
		break;
	default:
		break;
	}
	if (m_bsCurrState == CM_DEATH)
	{
		//Update the Timer and Emitter
		m_fVictoryTimer += elapsedTime;
		m_emVictoryEffect->Update(elapsedTime);
		//
		if (!m_bWon)
		{
			SGD::Event* pATEvent = new SGD::Event("GainedHawk", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
			GameplayState::GetInstance()->SetScreenFadeout(9);
			// TODO Delete bull, give player dash, update room
			GameplayState::GetInstance()->CreateTeleporter(510, 578, "Level3_1", false);
			GameplayState::GetInstance()->CreateHintStatue(440, 513, "You have The Hawk!(Press D or Right Trigger)");
			m_bWon = true;
		}
		if (m_fVictoryTimer > 50)
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
	Boss::Update(elapsedTime);
}

void Caveman::HawkExplode(SGD::Point _Pos)
{
	if (!m_pHawk->IsDead())
	{
		//reset the timer
		m_fStalacTimer = 0;
		m_bDrop = false;
		//start the emitter 
		m_hHawkExplode->Finish();
		m_hHawkExplode->Burst(_Pos);		
	}
}

void Caveman::DropStalactites()
{
	if (m_fStalacTimer > .18f && m_bDrop)
	{
		m_fStalacTimer = 0;
		//make a stalactite
		CreateStalactiteMessage* Temp1 = new CreateStalactiteMessage(m_pHawk);
		Temp1->QueueMessage();
		Temp1 = nullptr;
	}
}

void Caveman::Render(void)
{
	//Emitter Renders
	m_hHawkExplode->Render();
	m_emEYES->Render();
	// 
	if (!m_pHawk->IsDead())
	{
		m_pHawk->Render();
	}
	
	//Boss::Render();
	//Animation Render
	Camera::GetInstance()->DrawAnimation({ m_ptPosition.x + m_szSize.width / 2, m_ptPosition.y + m_szSize.height / 2 }, 0, m_ts, m_bFacingRight, 1);

	if (m_bsCurrState == CM_DEATH)
	{
		m_emVictoryEffect->Render({ 470, 220 });
		font.DrawString("VICTORY!", (int)(400 - Camera::GetInstance()->GetCameraPos().x), (int)(200 - Camera::GetInstance()->GetCameraPos().y), 3, SGD::Color{ 255, 255, 0, 0 });
	}
}

void Caveman::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_SOLID_WALL)
	{
		if (m_bsCurrState == CM_FALLING)
		{
			m_bsCurrState = CM_RUNING;
		}
	}
	if (pOther->GetType() == ENT_LASER && m_fHitTimer > 1)
	{
		if (m_bsCurrState == CM_JUMPING || m_bsCurrState == CM_FALLING)
		{
			m_fHitTimer = 0;
			m_bsCurrState = CM_STUNNED;
			m_fStunnedTimer = 0;
			m_iDoorSkip = 0;
			SetHitPoints(GetHitPoints() - 1);
		}
	}
	Boss::HandleCollision(pOther);
}

////////////////////////////////////////////////
////////////// Listener Interface //////////////
void Caveman::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "FLIP_LASER")
	{
		m_bLaserOn ? m_bLaserOn = false : m_bLaserOn = true;
	}

	if (pEvent->GetEventID() == "KILL_PLAYER")
	{
		if (m_bsCurrState != CM_DEATH)
		{
			SetHitPoints(3);
		}		
	}

	if (pEvent->GetSender() != this)
	{
		return;
	}


	//Skip or Hide
	if (pEvent->GetEventID() == "CaveMan_Door" &&
		pEvent->GetSender() == this)
	{
		if (m_bsCurrState == CM_FALLING || m_bsCurrState == CM_STUNNED)
		{
			return;
		}
		//Check if your abel to do door event
		if (m_fDoorTimer > 1 && m_bsCurrState != CM_ATTACKING)
		{
			//Reset Timer
			m_fDoorTimer = 0;
			if (m_iDoorSkip == 0)
			{
				m_iDoorSkip = rand() % (5 - (1 + 1)) + 1;
				//Reset Hiding Timer so it can hide
				m_fHidingTimer = 0;
				m_bsCurrState = CM_HIDING;
			}
			else
			{
				m_iDoorSkip--;
			}
		}
		//If attacking dont let Door event happen
		else if (m_bsCurrState == CM_ATTACKING)
		{
			m_fDoorTimer = 0;
		}

	}
	//Jump RIght
	else if (pEvent->GetEventID() == "CaveMan_Jump_Right" &&
		pEvent->GetSender() == this)
	{
 		if (m_bLaserOn && m_bsCurrState != CM_JUMPING && m_fSwitchJumpTimer > .5f)
		{
			m_fSwitchJumpTimer = 0;
			m_bsCurrState = CM_RUNING;
			m_bFacingRight ? m_bFacingRight = false : m_bFacingRight = true;
			return;
		}
		if (m_bsCurrState != CM_FALLING && !m_bLaserOn)
		{
			m_bFacingRight = true;
			m_bsCurrState = CM_JUMPING;
			m_fInTheAir = 0;
		}
	}
	//Jump Left
	else if (pEvent->GetEventID() == "CaveMan_Jump_Left" &&
		pEvent->GetSender() == this )
	{
		if (m_bLaserOn && m_bsCurrState != CM_JUMPING && m_fSwitchJumpTimer > .5)
		{
			m_fSwitchJumpTimer = 0;
			m_bsCurrState = CM_RUNING;
			m_bFacingRight ? m_bFacingRight = false : m_bFacingRight = true;
			return;		
		}
		if (m_bsCurrState != CM_FALLING && !m_bLaserOn)
		{
			m_bFacingRight = false;
			m_bsCurrState = CM_JUMPING;
			m_fInTheAir = 0;
		}
	}
	//Turn the Other Way
	else if (pEvent->GetEventID() == "CaveMan_Stop" )
	{
		if (m_fStopTimer > .4f)
		{
			m_fStopTimer = 0;
			m_bsCurrState = CM_RUNING;
			m_bFacingRight ? m_bFacingRight = false : m_bFacingRight = true;
			int x = 0;
		}
	}
}

bool Caveman::GetAttacking() const
{
	return false;
}

void Caveman::ResetCaveMan()
{

}
#include "Caveman.h"
#include "Hawk.h"
#include "CreateStalactite.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "ParticleEngine.h"
#include "AnimationEngine.h"
#include "Camera.h"
#include <random>

Caveman::Caveman() : SGD::Listener(this)
{
	Listener::RegisterForEvent("CaveMan_Door");
	Listener::RegisterForEvent("CaveMan_Jump_Right");
	Listener::RegisterForEvent("CaveMan_Jump_Left");
	Listener::RegisterForEvent("CaveMan_Stop");
	SetSize({ 64.0f, 96.0f });
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
	AnimationEngine::GetInstance()->LoadAnimation("Assets/CaveManAnim.xml");
	m_ts.SetCurrAnimation("CaveManWalking");
	m_ts.SetPlaying(true);

}


Caveman::~Caveman()
{
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
	m_fThinkingTimer += elapsedTime;
	m_fInTheAir += elapsedTime;
	m_fDoorTimer += elapsedTime;
	m_fStopTimer += elapsedTime;
	m_fHidingTimer += elapsedTime;
	m_fAttackingTimer += elapsedTime;
	m_fStalacTimer += elapsedTime;
	//

	//Slash at player if close and not jumping or falling or attacking or emerging or hideing
	if (m_bsCurrState == CM_RUNING && !GetPlayer()->GetSnared())
	{
		//Find Distance to point
		SGD::Vector distance = m_ptPosition - GetPlayer()->GetPosition();
		if (distance.ComputeLength() < 50)
		{
			m_bsCurrState = CM_SLASHING;
		}
			
	}

	//Hawk Update
	m_pHawk->Update(elapsedTime);
	//Drop stalactite if hawk is ready
	DropStalactites();
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
		if (m_fInTheAir < 1)
		{
			if (m_bFacingRight)
			{
				SetVelocity({ 350, -200 });
			}
			else
			{
				SetVelocity({ -350, -200 });
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
				SetPosition({ 303, 545 });
				break;
			case 2:
				SetPosition({ 1071, 545 });
				break;
			case 3:
				SetPosition({ 1839, 545 });
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
			m_pHawk->Attack({ GetPlayer()->GetPosition().x + 300, 33 });
		}
		else
		{
			m_pHawk->Attack({ GetPlayer()->GetPosition().x - 300, 33 });
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
		if (m_fThinkingTimer > .4f)
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
		break;
	default:
		break;
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
	if (m_fStalacTimer > .1f && m_bDrop)
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
	
	Boss::Render();
	//Animation Render
	Camera::GetInstance()->DrawAnimation({ m_ptPosition.x + m_szSize.width / 2, m_ptPosition.y + m_szSize.height / 2 }, 0, m_ts, m_bFacingRight, 1);

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
	Boss::HandleCollision(pOther);
}

////////////////////////////////////////////////
////////////// Listener Interface //////////////
void Caveman::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetSender() != this)
	{
		return;
	}
	//Skip or Hide
	if (pEvent->GetEventID() == "CaveMan_Door" &&
		pEvent->GetSender() == this)
	{
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
		if (m_bsCurrState != CM_FALLING)
		{
			m_bFacingRight = true;

			m_bsCurrState = CM_JUMPING;

			m_fInTheAir = 0;
		}
	}
	//Jump Left
	else if (pEvent->GetEventID() == "CaveMan_Jump_Left")
	{
		if (m_bsCurrState != CM_FALLING)
		{
			m_bFacingRight = false;

			m_bsCurrState = CM_JUMPING;

			m_fInTheAir = 0;
		}
	}
	//Turn the Other Way
	else if (pEvent->GetEventID() == "CaveMan_Stop")
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
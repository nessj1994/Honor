#include "MutantMan.h"
#include "Player.h"
#include "Vomit.h"
#include "EntityManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "CreateVomitMessage.h"
#include "DestroyEntityMessage.h"
#include "AnimationEngine.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Camera.h"

#include <cmath>
#include <Windows.h>

MutantMan::MutantMan() : Listener(this)
{
	Listener::RegisterForEvent("ResetRoom");
	m_ptPosition = { 96, 672 };
	m_pPatrolPoint = m_ptPosition;
	AnimationEngine::GetInstance()->LoadAnimation("Assets/MutantManAnim.xml");
	m_ts.SetCurrAnimation("Mutant_Idle");
	m_ts.SetPlaying(true);
	m_szSize = {50,70};
	SetGravity(-1000.0f);
	m_bPlayerAttacked = false;
	m_hHurt = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/ZombieAttacked.wav");
	m_hGag = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Gag.wav");
	m_hPunch = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Punch-Mark.wav");
	m_fVomitTimer = 6;
}


MutantMan::~MutantMan()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hGag);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hPunch);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hHurt);

}

void MutantMan::Update(float _elapsedTime)
{
	//Use the Units Update
	Enemy::Update(_elapsedTime);
	
	//Timer Updates
	m_fVomitTimer += _elapsedTime;
	m_fDeadTImer += _elapsedTime;
	//
	//Update Animation
	AnimationEngine::GetInstance()->Update(_elapsedTime, m_ts, this);
	SGD::Vector distance;
	
	//Dont do anything if dead switch animation and destroy entity after death timer reaches 1 second
	if (!GetAlive())
	{
		SetVelocity({ GetVelocity().x, (GetVelocity().y - GetGravity() * _elapsedTime) });
		if (m_ts.GetCurrAnimation() != "Mutant_Death")
		{
			m_ts.ResetCurrFrame();
			m_ts.SetCurrAnimation("Mutant_Death");
			m_ts.SetPlaying(true);
		}
		if (m_fDeadTImer > 1)
		{
			//Reseting Enemys
			SetAlive(false);
			/*DestroyEntityMessage* Temp = new DestroyEntityMessage(this);
			Temp->QueueMessage();
			Temp = nullptr;*/
		}

		return;
	}
	else
	{
		m_fDeadTImer = 0;
	}
	
	if (GetPlayer() != nullptr)
	{
		//Set Point if you have a target
		m_pPatrolPoint = GetPlayer()->GetPosition();
	}

	//Find Distance to point
	distance = m_ptPosition - m_pPatrolPoint;
	if (distance.ComputeLength() < 600)
	{	
		//Move to patrol point
		if (m_ptPosition.x < m_pPatrolPoint.x)
		{
			SetFacingRight(true);
			m_vtVelocity.x = -300;
		}
		if (m_ptPosition.x > m_pPatrolPoint.x)
		{
			SetFacingRight(false);
			m_vtVelocity.x = 300;
		}

		if (distance.ComputeLength() < 60)
		{
			//No Longer implemented in the Mutant Mane
		}
		else 
		//if Mutant is going towards player change to walking animation if he doesnt need to vomit
		if ((m_ts.GetCurrAnimation() != "Mutant_Walking" || m_ts.GetCurrAnimation() != "Mutant_Vomiting") && m_fVomitTimer < 5)
		{
			if (m_ts.GetCurrAnimation() != "Mutant_Walking")
			{
				m_ts.ResetCurrFrame();
				m_ts.SetCurrAnimation("Mutant_Walking");
				m_ts.SetPlaying(true);
			}
		}
		else
		{
			//if he does need to vomit, Vomit and stop at the current location
			if (m_ts.GetCurrAnimation() != "Mutant_Vomiting")
			{
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hGag))
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hGag);
				}
				CreateVomitMessage* temp = new CreateVomitMessage(this);
				temp->QueueMessage();
				temp = nullptr;
				m_ts.ResetCurrFrame();
				m_ts.SetCurrAnimation("Mutant_Vomiting");
				m_ts.SetPlaying(true);
			}
			if (m_fVomitTimer > 6.5f)
			{
				m_fVomitTimer = 0;
			}
			m_vtVelocity.x = 0;
		}
		
	}
	else
	{
		//Make Mutant Change ANimation when he need to vomit without reseting animations over and over
		if ((m_ts.GetCurrAnimation() != "Mutant_Idle" || m_ts.GetCurrAnimation() != "Mutant_Vomiting") && m_fVomitTimer < 5)
		{
			if (m_ts.GetCurrAnimation() != "Mutant_Idle")
			{
				m_ts.ResetCurrFrame();
				m_ts.SetCurrAnimation("Mutant_Idle");
				m_ts.SetPlaying(true);
			}
		}
		//Vomit if its Time
		else
		{
			if (m_ts.GetCurrAnimation() != "Mutant_Vomiting")
			{
				if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hGag))
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hGag);
				}		
				CreateVomitMessage* temp = new CreateVomitMessage(this);
				temp->QueueMessage();
				temp = nullptr;
				m_ts.ResetCurrFrame();
				m_ts.SetCurrAnimation("Mutant_Vomiting");
				m_ts.SetPlaying(true);
			}
			if (m_fVomitTimer > 6.5f)
			{
				m_fVomitTimer = (float)(rand() % (6 - (3 + 1)) + 3);
			}
		}
		m_vtVelocity = { 0, 300 };
	}


}

void MutantMan::Render()
{
	//SGD::GraphicsManager::GetInstance()->DrawRectangle({{ m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x,m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y}, m_szSize}, { 255, 255, 255, 255 });
	Camera::GetInstance()->DrawAnimation({ (m_ptPosition.x + m_szSize.width / 2), (m_ptPosition.y + m_szSize.height/2)+33 }, 0, m_ts, GetFacingRight(), 1);
}

SGD::Rectangle MutantMan::GetRect(void) const 
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void MutantMan::HandleCollision(const IEntity* pOther) 
{

	RECT rMutant;
	rMutant.left = (LONG)GetRect().left;
	rMutant.top = (LONG)GetRect().top;
	rMutant.right = (LONG)GetRect().right;
	rMutant.bottom = (LONG)GetRect().bottom;

	RECT rObject;
	rObject.left = (LONG)pOther->GetRect().left;
	rObject.top = (LONG)pOther->GetRect().top;
	rObject.right = (LONG)pOther->GetRect().right;
	rObject.bottom = (LONG)pOther->GetRect().bottom;

	RECT rIntersection = {};

	IntersectRect(&rIntersection, &rObject, &rMutant);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;



	if (nIntersectHeight > nIntersectWidth)
	{
		if (rMutant.right == rIntersection.right)
		{
			SetPosition({ (float)rObject.left - GetSize().width + 1, GetPosition().y });
		}
		if (rMutant.left == rIntersection.left)
		{
			SetPosition({ (float)rObject.right, GetPosition().y });
		}
	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (rMutant.bottom == rIntersection.bottom)
		{
			SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 /*- nIntersectHeight*/ });
		}
		if (rMutant.top == rIntersection.top)
		{
			SetPosition({ GetPosition().x, (float)rObject.bottom });
		}
	}

	
	Enemy::HandleCollision(pOther);
}

void MutantMan::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ResetRoom")
	{
		SetAlive(true);
		SetPosition(GetOriginalPos());
	}
}

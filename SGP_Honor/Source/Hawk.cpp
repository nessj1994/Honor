#include "Hawk.h"
#include "Game.h"
#include "Camera.h"
#include "Player.h"
#include "DestroyEntityMessage.h"
#include "ParticleEngine.h"
#include "Emitter.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Caveman.h"

Hawk::Hawk()
{
	if (GetOwner() != nullptr)
	{
		//if (GetOwner()->GetType() == Entity::ENT_PLAYER)
		//{
			//GetOwner()->SetHawkPtr(this);
		//}
		
	}
	SetSpeed(500);

	m_hEffect = SGD::AudioManager::GetInstance()->LoadAudio("assets/audio/HawkEffect.wav");
	SGD::AudioManager::GetInstance()->PlayAudio(m_hEffect);
	m_szSize = { 32, 32 };
	m_bDead = false;
}


Hawk::~Hawk()
{
	//DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
	//pMsg->QueueMessage();
	//pMsg = nullptr;
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hEffect);
}


void Hawk::Update(float elapsedTime)
{

	if (GetDirection().x == 1)
	{
		//SetVelocity({ GetVelocity().x /*+ GetSpeed() * elapsedTime*/, GetVelocity().y });
		if (GetVelocity().x > 450)
		{
			SetVelocity({ 450, GetVelocity().y });
		}
	}

	else
	{
		//SetVelocity({ GetVelocity().x /*- GetSpeed() * elapsedTime*/, GetVelocity().y });
		if (GetVelocity().x < -450)
		{
			SetVelocity({ -450, GetVelocity().y });
		}
	}

	//if (GetPosition().x == -100)
	//	m_fCooldown += elapsedTime;
	//else
	//	m_fCooldown = 0.0f;

	//For Caveman Boss use
	if (m_pOwner->GetType() == ENT_BOSS_CAVEMAN)
	{
		SGD::Vector distance = m_poTarget - m_ptPosition;
		if (distance.ComputeLength() > 20)
		{
			SGD::Vector VEL = { 0, 0 };
			if (m_poTarget.x + 5 < m_ptPosition.x)
			{
				VEL.x = -300;
			}
			if (m_poTarget.y + 5 < m_ptPosition.y)
			{
				VEL.y = -300;
			}
			if (m_poTarget.x - 5 > m_ptPosition.x)
			{
				VEL.x = 300;
			}
			if (m_poTarget.y - 5 > m_ptPosition.y)
			{
				VEL.y = 300;
			}

			SetVelocity(VEL);
		}
		else
		{
			((Caveman*)m_pOwner)->HawkExplode(m_ptPosition);
			m_bDead = true;
		}
	}



		Entity::Update(elapsedTime);

		SGD::Rectangle rSelf = this->GetRect();
		SGD::Rectangle rScreen =
		{
			0, 0,
			Game::GetInstance()->GetScreenWidth() + Camera::GetInstance()->GetCameraPos().x,
			Game::GetInstance()->GetScreenHeight() + Camera::GetInstance()->GetCameraPos().y
		};


		if (rSelf.IsIntersecting(rScreen) == false)
		{
			//DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
			//pMsg->QueueMessage();
			//pMsg = nullptr;
			if (GetOwner()->GetType() == ENT_PLAYER)
			{
				Player* temp = dynamic_cast<Player*>(GetOwner());
				temp->HawkExplode(m_ptPosition);
			}
			SetPosition({ -1000, -1000 });

			SetVelocity({ 0, 0 });


		}


}


void Hawk::Attack(SGD::Point _Pos)
{
	if (m_pOwner->GetType() == ENT_BOSS_CAVEMAN)
	{
		m_poTarget = _Pos;
		m_bDead = false;
	}
}


void Hawk::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_SOLID_WALL &&
		pOther != GetOwner())
	{
	//	DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
	//	pMsg->QueueMessage();
	//	pMsg = nullptr;

		//EVENT SYSTEM
		//SGD::Event m_Eevent = { "PLAYER_HIT", GetOwner(), this };
		//m_Eevent.SendEventNow((void*)pOther);


		if (GetOwner()->GetType() == ENT_PLAYER)
		{
			Player* temp = dynamic_cast<Player*>(GetOwner());
			temp->HawkExplode(m_ptPosition);
		}
		SetPosition({ -1000, -1000 });

		SetVelocity({ 0, 0 });

	}

	if (pOther->GetType() == ENT_GEYSER &&
		pOther != GetOwner())
	{
		//	DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		//	pMsg->QueueMessage();
		//	pMsg = nullptr;

		//EVENT SYSTEM
		//SGD::Event m_Eevent = { "PLAYER_HIT", GetOwner(), this };
		//m_Eevent.SendEventNow((void*)pOther);


		if (GetOwner()->GetType() == ENT_PLAYER)
		{
			Player* temp = dynamic_cast<Player*>(GetOwner());
			temp->HawkExplode(m_ptPosition);
		}
		SetPosition({ -1000, -1000 });

		SetVelocity({ 0, 0 });

	}



	if (pOther->GetType() == ENT_STALACTITE &&
		pOther != GetOwner())
	{
		//	DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		//	pMsg->QueueMessage();
		//	pMsg = nullptr;

		//EVENT SYSTEM
		//SGD::Event m_Eevent = { "PLAYER_HIT", GetOwner(), this };
		//m_Eevent.SendEventNow((void*)pOther);


		if (GetOwner()->GetType() == ENT_PLAYER)
		{
			Player* temp = dynamic_cast<Player*>(GetOwner());
			temp->HawkExplode(m_ptPosition);
		}
		SetPosition({ -1000, -1000 });

		SetVelocity({ 0, 0 });

	}

	if(pOther->GetType() == ENT_BOSS_WIZARD &&
		pOther != GetOwner())
	{
		if (GetOwner()->GetType() == ENT_PLAYER)
		{
			Player* temp = dynamic_cast<Player*>(GetOwner());
			temp->HawkExplode(m_ptPosition);
		}

		SetPosition({ -100, -100 });

		SetVelocity({ 0, 0 });
	}

	if (pOther->GetType() == ENT_SWITCH &&
		pOther != GetOwner())
	{
		//	DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		//	pMsg->QueueMessage();
		//	pMsg = nullptr;

		//EVENT SYSTEM
		//SGD::Event m_Eevent = { "PLAYER_HIT", GetOwner(), this };
		//m_Eevent.SendEventNow((void*)pOther);
		if (GetOwner()->GetType() == ENT_PLAYER)
		{
			Player* temp = dynamic_cast<Player*>(GetOwner());
			temp->HawkExplode(m_ptPosition);
		}

		SetPosition({ -1000, -1000 });

		SetVelocity({ 0, 0 });

	}

}

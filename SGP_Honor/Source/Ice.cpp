#include "Ice.h"
#include "Game.h"
#include "Camera.h"
#include "DestroyEntityMessage.h"
#include "ParticleEngine.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Emitter.h"
#include <cmath>
#include <Windows.h>

Ice::Ice()
{
	m_pEmitter = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/SprayParticle.xml", "Ice", m_ptPosition);
}


Ice::~Ice()
{
	delete m_pEmitter;
}

void Ice::Update(float elapsedTime) 
{

		m_pEmitter->SetPosition(m_ptPosition);

	m_fTimer += elapsedTime;

	m_pEmitter->Update(elapsedTime);
	//m_pEmitter->Recylce()

	//if (GetDirection().x == 1)
	//{
	//	SetVelocity({ GetVelocity().x + GetSpeed() * elapsedTime, GetVelocity().y });
	//	if (GetVelocity().x > 850)
	//	{
	//		SetVelocity({ 850, GetVelocity().y + 100 });
	//	}
	//}

	//else
	//{
	//	SetVelocity({ GetVelocity().x - GetSpeed() * elapsedTime, GetVelocity().y });
	//	if (GetVelocity().x < -850)
	//	{
	//		SetVelocity({ -850, GetVelocity().y + 100 });
	//	}
	//}

	SetVelocity({ GetVelocity().x, (GetVelocity().y - (-500) * elapsedTime) });

	if (m_fTimer >= .4f)
	{
		m_pEmitter->Finish();
		if (m_pEmitter->Done())
		{
			DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
	else
	{
		m_pEmitter->Finish(false);
	}




	Entity::Update(elapsedTime);
	//Delete Ice particle when it goes of screen
	SGD::Rectangle rSelf = this->GetRect();
	SGD::Rectangle rScreen =
	{
		0, 0,
		Game::GetInstance()->GetScreenWidth() + Camera::GetInstance()->GetCameraPos().x,
		Game::GetInstance()->GetScreenHeight() + Camera::GetInstance()->GetCameraPos().y
	};


	if (rSelf.IsIntersecting(rScreen) == false)
	{
		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;

	}

}

void Ice::HandleCollision(const IEntity* pOther)
{
	RECT rMutant;
	rMutant.left = GetRect().left;
	rMutant.top = GetRect().top;
	rMutant.right = GetRect().right;
	rMutant.bottom = GetRect().bottom;

	RECT rObject;
	rObject.left = pOther->GetRect().left;
	rObject.top = pOther->GetRect().top;
	rObject.right = pOther->GetRect().right;
	rObject.bottom = pOther->GetRect().bottom;

	RECT rIntersection = {};

	IntersectRect(&rIntersection, &rObject, &rMutant);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;



	if (nIntersectHeight > nIntersectWidth)
	{
		if (rMutant.right == rIntersection.right)
		{
			SetPosition({ (float)rObject.left - GetSize().width + 1, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
		if (rMutant.left == rIntersection.left)
		{
			SetPosition({ (float)rObject.right, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}

	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (rMutant.bottom == rIntersection.bottom)
		{
			SetVelocity({ GetVelocity().x, 0 });
			SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 /*- nIntersectHeight*/ });
		}
		if (rMutant.top == rIntersection.top)
		{
			SetPosition({ GetPosition().x, (float)rObject.bottom });
			SetVelocity({ GetVelocity().x, 0 });
		}
	}
}

void Ice::Render(void)
{
	m_pEmitter->Render();
}
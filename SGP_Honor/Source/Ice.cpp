#include "Ice.h"
#include "Game.h"
#include "Camera.h"
#include "DestroyEntityMessage.h"
#include "ParticleEngine.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Emitter.h"

Ice::Ice()
{
	m_pEmitter = ParticleEngine::GetInstance()->LoadEmitter("Assets/SprayParticle.xml", "Spray", {96,672});

}


Ice::~Ice()
{
	delete m_pEmitter;
}

void Ice::Update(float elapsedTime) 
{


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
		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;

	}

}

void Ice::Render(void)
{
	m_pEmitter->Render(m_ptPosition);
}
#include "Hawk.h"
#include "Game.h"
#include "Camera.h"
#include "DestroyEntityMessage.h"

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
}


Hawk::~Hawk()
{
	//DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
	//pMsg->QueueMessage();
	//pMsg = nullptr;
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

		SetPosition({ -100, -100 });

		SetVelocity({ 0, 0 });

		
	}


}
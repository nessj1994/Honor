#include "Projectile.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "DestroyEntityMessage.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"

#include "Camera.h"
#include "Game.h"

Projectile::Projectile() : Listener(this)
{
	Listener::RegisterForEvent("ResetRoom");
	SetSpeed(5000);
	if(m_hImage == SGD::INVALID_HANDLE)
		m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(L"Assets/graphics/TurretShot.png");
}


Projectile::~Projectile()
{
	//if (m_pOwner)
	//{
	//	m_pOwner->Release();
	//}
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}



/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Projectile::Update(float elapsedTime)
{
	//moving right
	if (GetDirection().x == 1)
	{
		SetVelocity({ GetVelocity().x + GetSpeed() * elapsedTime, GetVelocity().y });
		if (GetVelocity().x > 550)
		{
			SetVelocity({ 550, GetVelocity().y });
		}
	}
	//Moving Left
	else if (GetDirection().x == -1)
	{
		SetVelocity({ GetVelocity().x - GetSpeed() * elapsedTime, GetVelocity().y });
		if (GetVelocity().x < -550)
		{
			SetVelocity({ -550, GetVelocity().y });
		}
	}
	// Moving up
	else if (GetDirection().y == -1)
	{
		SetVelocity({ GetVelocity().x, GetVelocity().y - GetSpeed() * elapsedTime });
		if (GetVelocity().y < -550)
		{
			SetVelocity({ GetVelocity().x, -550 });
		}
	}
	// Moving down
	else if (GetDirection().y == 1)
	{
		SetVelocity({ GetVelocity().x, GetVelocity().y + GetSpeed() * elapsedTime });
		if (GetVelocity().y > 550)
		{
			SetVelocity({ GetVelocity().x, 550 });
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
		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;

	}


}

void Projectile::Render(void)
{
	/*Camera::GetInstance()->Draw(SGD::Rectangle(
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
		SGD::Color::Color(255, 0, 255, 0));*/
	Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false, 1, {}, {});

}




SGD::Rectangle Projectile::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}
void Projectile::HandleCollision(const IEntity* pOther)
{
	if ((pOther->GetType() == ENT_SOLID_WALL || pOther->GetType() == ENT_DOOR) &&
		pOther != GetOwner())
	{
		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;

		//EVENT SYSTEM
		//SGD::Event m_Eevent = { "PLAYER_HIT", GetOwner(), this };
		//m_Eevent.SendEventNow((void*)pOther);

	}
	//Did we hit the player
	if(pOther->GetType() == ENT_PLAYER
		&& pOther != GetOwner())
	{
		//Destroy ourself
		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;


		//Kill the Player
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}

	SGD::Rectangle entityRect = pOther->GetRect();

}

void Projectile::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ResetRoom")
	{
		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}
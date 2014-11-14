#include "Stalactite.h"
#include "LevelCollider.h"
#include "Player.h"
#include "DestroyEntityMessage.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Camera.h"

Stalactite::Stalactite()
{
	m_szSize = { 16, 16 };
	m_ptPosition = { 900, 200 };
	m_ptStartPosition = { 900, 200 };
	m_fFallSpeed = 100;
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/Fallingspike.png", {});
}


Stalactite::~Stalactite()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Stalactite::Update(float elapsedTime)
{
	m_fFallSpeed = 600;
    if(m_bIsFalling)
	{
		m_vtVelocity.y = m_fFallSpeed;
	}

	if(m_fRespawnTimer > 0.0f)
	{
		m_fRespawnTimer -= elapsedTime;
	}


	if(m_fRespawnTimer < 0.0f)
	{
		m_fRespawnTimer = 0.0f;
		m_szSize = { 16, 16 };

	}

	m_ptPosition += m_vtVelocity * elapsedTime;

}
void Stalactite::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({ -camPos.x, -camPos.y });

	//Camera::GetInstance()->Draw(rMyRect, { 255, 0, 255, 255 });
	Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false, 1, {}, {});
}

int Stalactite::GetType(void) const
{

	return ENT_STALACTITE;
}

SGD::Rectangle Stalactite::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Stalactite::HandleCollision(const IEntity* pOther)
{
	if(pOther->GetType() == ENT_HAWK)
	{
		m_bIsFalling = true;
	}
	if(pOther->GetType() == ENT_SOLID_WALL)
	{
		DestroyEntityMessage* Temp = new DestroyEntityMessage(this);
		Temp->QueueMessage();
		Temp = nullptr;
	}
	if (pOther->GetType() == ENT_PLAYER)
	{
		if (((Player*)pOther)->IsDashing())
		{
			return;
		}
	  //if so move back up but kill the player
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}


}
#include "Armor.h"
#include "Entity.h"
#include "Player.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_Event.h"


Armor::Armor() : Listener(this)
{
	Listener::RegisterForEvent("ResetRoom");
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/armor.png");
	SetSize(SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage));
	m_ptPosition = SGD::Point(100, 300);
}


Armor::~Armor()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}


/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Armor::Update(float elapsedTime)
{


}
void Armor::Render(void)
{
	if (isCollected == false)
		Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false, .5f, {}, {});
}

SGD::Rectangle Armor::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Armor::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == Entity::ENT_PLAYER)
	{
		if (GetRect().IsIntersecting(pOther->GetRect()) == true)
		{
			isCollected = true;
		}
	}
}

void Armor::HandleEvent(const SGD::Event* pEvent)
{
	if (pEvent->GetEventID() == "ResetRoom")
	{
		isCollected = false;
	}
}
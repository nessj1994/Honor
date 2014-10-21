#include "Honor.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"
#include "Player.h"

Honor::Honor()
{
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("../Assets/graphics/star.png");
	m_ptPosition = SGD::Point(200, 300);
	SetSize(SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage));
}


Honor::~Honor()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Honor::Update(float elapsedTime)
{

}
void Honor::Render(void)
{
	if (isCollected == false)
		Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false);
	//SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, SGD::Point(200, 400));
}

SGD::Rectangle Honor::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void Honor::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == Entity::ENT_PLAYER)
	{
		if (GetRect().IsIntersecting(pOther->GetRect()) == true)
			isCollected = true;

	}
}
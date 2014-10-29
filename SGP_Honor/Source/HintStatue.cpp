#include "HintStatue.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_InputManager.h"

HintStatue::HintStatue()
{
	m_hSBImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/speechbubble.png");
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/statue.png");
	m_ptPosition = SGD::Point(400, 300);
	SetSize(SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage));
}


HintStatue::~HintStatue()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hSBImage);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void HintStatue::Update(float elapsedTime)
{
	if (showMessage == true)
		showMessage = false;
}
void HintStatue::Render(void)
{
	//SGD::GraphicsManager::GetInstance()->DrawTexture(m_hImage, m_ptPosition);
	Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false, 1.0f, {});
	if (showMessage == true)
	{
		Camera::GetInstance()->DrawTexture({ m_ptPosition.x - SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage).width * 2.5f
			, m_ptPosition.y - SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage).height }, 0, m_hSBImage, false, 1 , {});

		Camera::GetInstance()->DrawString(st_message, { m_ptPosition.x - SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage).width * 2.5f +
			SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hSBImage).width * 0.25f,
			m_ptPosition.y - SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage).height +
			SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hSBImage).height * 0.25f });
	}
}

SGD::Rectangle HintStatue::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void HintStatue::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == Entity::ENT_PLAYER)
	{
		if (GetRect().IsIntersecting(pOther->GetRect()) == true)
			showMessage = true;
	}
}
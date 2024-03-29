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
	Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false, 0.5f, {}, {});
	if (showMessage == true)
	{
		//if (GetPosition().y < 200)
		//{

		//	Camera::GetInstance()->DrawTexture({ (m_ptPosition.x - SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage).width * 2.5f) + 50
		//		, 200 - SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage).height }, 0, m_hSBImage, false, 0.8f, {}, {});

		//	Camera::GetInstance()->DrawString(st_message, { (m_ptPosition.x - SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage).width * 2.5f +
		//		SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hSBImage).width * 0.25f) - 125,
		//		(200 - SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage).height +
		//		SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hSBImage).height * 0.25f) + 50 });
		//}
		//else
		//{

			Camera::GetInstance()->DrawTexture({ (m_ptPosition.x - SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage).width * 2.5f) + 50
				, m_ptPosition.y - SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage).height + 50 }, 0, m_hSBImage, false, 0.8f, {}, {});


			Camera::GetInstance()->DrawString(st_message, { (m_ptPosition.x - SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage).width * 2.5f +
				SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hSBImage).width * 0.25f) - 50,
				(m_ptPosition.y - SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage).height +
				SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hSBImage).height * 0.25f) + 50 });

		//}
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
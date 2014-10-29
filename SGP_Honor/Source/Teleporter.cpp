#include "Teleporter.h"
#include "Camera.h"
#include "GameplayState.h"
#include "ChangeLevelMessage.h"
#include "../SGD Wrappers/SGD_InputManager.h"

/////////////////////////////////////////////////
// Ctor/dtor
Teleporter::Teleporter()
{
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(L"Assets/Graphics/Teleporter.png");
}

Teleporter::~Teleporter()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

/////////////////////////////////////////////////
// Render
// -Renders a square around the collision box
void Teleporter::Render()
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//Get our rectangle
	SGD::Rectangle rMyRect = GetRect();

	rMyRect.Offset({ -camPos.x, -camPos.y });

	//Camera::GetInstance()->Draw(rMyRect, { 255, 0, 0, 255 });
	
	Camera::GetInstance()->DrawTexture({ m_ptPosition.x - 35, m_ptPosition.y - 40 }, 0.0f, m_hImage, false, 1.0f, {});

}

/////////////////////////////////////////////////
// HandleCollision
// -If the player collides, change the level
void Teleporter::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::W))
		{
			ChangeLevelMessage* pMsg = new ChangeLevelMessage{ this };
			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
}
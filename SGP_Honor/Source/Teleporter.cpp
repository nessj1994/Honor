#include "Teleporter.h"
#include "Camera.h"
#include "GameplayState.h"
#include "ChangeLevelMessage.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

/////////////////////////////////////////////////
// Ctor/dtor
Teleporter::Teleporter()
{
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(L"Assets/Graphics/Teleporter.png");
	m_hDoorClosed = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Sharp_Fart.wav");
}

Teleporter::~Teleporter()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hDoorClosed);
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
	
	Camera::GetInstance()->DrawTexture({ m_ptPosition.x - 35, m_ptPosition.y - 40 }, 0.0f, m_hImage, false, 1.0f, {}, {});

}

/////////////////////////////////////////////////
// HandleCollision
// -If the player collides, change the level
void Teleporter::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		float leftStickYOff = SGD::InputManager::GetInstance()->GetLeftJoystick(0).y;

		// Hub Teleporters will only take you to a level if you have unlocked it
		if (m_bHubTeleporter)
		{
			//if (leftStickYOff < -0.8)
			if (SGD::InputManager::GetInstance()->IsButtonPressed(0, 3) == true ||
				SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::S))
			{
				if (GameplayState::GetInstance()->GetLevelUnlocked(m_sLevel))
				{
					ChangeLevelMessage* pMsg = new ChangeLevelMessage{ this };
					pMsg->QueueMessage();
					pMsg = nullptr;
				}
				else
				{
					SGD::AudioManager::GetInstance()->PlayAudio(m_hDoorClosed);
				}
			}
		}
		else
		{
			//if (leftStickYOff < -0.8)
			if (SGD::InputManager::GetInstance()->IsButtonPressed(0, 3) == true ||
				SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::S))
			{
				GameplayState::GetInstance()->UnlockLevel(m_sLevel);
				ChangeLevelMessage* pMsg = new ChangeLevelMessage{ this };
				pMsg->QueueMessage();
				pMsg = nullptr;
			}
		}

	}
}
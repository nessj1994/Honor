#include "BossDoor.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "Player.h"
#include "Game.h"
#include "GameplayState.h"
#include "Font.h"
#include "BitmapFont.h"
#include "ChangeLevelMessage.h"

BossDoor::BossDoor()
{
	//m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture(L"Assets/Graphics/Teleporter.png");
	//m_hDoorClosed = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/Sharp_Fart.wav");

}


BossDoor::~BossDoor()
{
	//SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
	//SGD::AudioManager::GetInstance()->UnloadAudio(m_hDoorClosed);
}

void BossDoor::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//Get our rectangle
	SGD::Rectangle rMyRect = GetRect();

	rMyRect.Offset({ -camPos.x, -camPos.y });

	//Camera::GetInstance()->Draw(rMyRect, { 255, 0, 0, 255 });

	Camera::GetInstance()->DrawTexture({ m_ptPosition.x - 35, m_ptPosition.y - 40 }, 0.0f, m_hImage, false, 1.0f, {}, {});

	// Draw required honor
	SGD::OStringStream output;
	output << ": " << m_unRequiredHonor;
	//Local refernce to the font
	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");
	font.DrawString(output.str().c_str(), (int)m_ptPosition.x, (int)m_ptPosition.y, 1, SGD::Color{ 255, 255, 0, 0 });
}

int BossDoor::GetType(void) const
{
	return ENT_BOSS_DOOR;

}

/////////////////////////////////////////////////
// HandleCollision
// -If the player collides, change the level
void BossDoor::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		float leftStickYOff = SGD::InputManager::GetInstance()->GetLeftJoystick(0).y;

		//if (leftStickYOff < -0.8)
		if (SGD::InputManager::GetInstance()->IsButtonPressed(0, 3) == true ||
			SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::E))
		{
			Player * pPlayer = (Player*)pOther;
			if (pPlayer->GetHonorCollected() >= m_unRequiredHonor)
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
}

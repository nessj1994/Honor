#include "BossDoor.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "Player.h"

BossDoor::BossDoor() : Listener(this)
{
	Listener::RegisterForEvent("HONOR_GAINED");
	m_ptPosition = { 400, 150 };
	m_szSize = { 100, 200 };

}


BossDoor::~BossDoor()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void BossDoor::Update(float elapsedTime)
{

	if(m_bOpen)
	{
		m_szSize = { 0, 0 };
	}
	else
	{
		m_szSize = { 100, 200 };
	}

}
void BossDoor::Render(void)
{
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//Get our rectangle
	SGD::Rectangle rMyRect = GetRect();

	rMyRect.Offset({ -camPos.x, -camPos.y });

	Camera::GetInstance()->Draw(rMyRect, { 255, 0, 0, 255 });
}

int BossDoor::GetType(void) const
{
	return ENT_BOSS_DOOR;

}

SGD::Rectangle BossDoor::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };

}


////////////////////////////////////////////////
////////////// Listener Interface //////////////
void BossDoor::HandleEvent(const SGD::Event* pEvent)
{
	//which event

	//Set the player back to his last checkpoint 
	//This is usually back to the level start
	if(pEvent->GetEventID() == "HONOR_GAINED")
	{
		Player* pPlayer = reinterpret_cast<Player*>(pEvent->GetSender());
		
		if(pPlayer->GetHonorCollected() >= m_unRequiredHonor)
		{
			m_bOpen = true;
		}
	}
}

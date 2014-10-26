#include "Teleporter.h"
#include "Camera.h"
#include "GameplayState.h"
#include "ChangeLevelMessage.h"

/////////////////////////////////////////////////
// Ctor/dtor
Teleporter::Teleporter()
{

}

Teleporter::~Teleporter()
{

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

	Camera::GetInstance()->Draw(rMyRect, { 255, 0, 0, 255 });
}

/////////////////////////////////////////////////
// HandleCollision
// -If the player collides, change the level
void Teleporter::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		ChangeLevelMessage* pMsg = new ChangeLevelMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
}
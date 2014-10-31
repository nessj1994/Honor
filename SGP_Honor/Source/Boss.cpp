#include "Boss.h"
#include "Camera.h"

#include <Windows.h>

Boss::Boss()
{
}


Boss::~Boss()
{
}


///////////////////////////////////////////////////
// Update
// -Main update loop
void Boss::Update(float elapsedTime)
{
	// Apply gravity
	if (GetIsFalling() == true)
	{
		SetGravity(-3000);
		SetVelocity({ GetVelocity().x, GetVelocity().y - GetGravity() * elapsedTime });
	}

	// Update movement
	Unit::Update(elapsedTime);
}

///////////////////////////////////////////////////
// Render
// -Draw the collision rect
void Boss::Render()
{
	// Render the collision rect
	//Get the camera position for our offset
	SGD::Point camPos = Camera::GetInstance()->GetCameraPos();

	//create a reference to our rectangle
	SGD::Rectangle rMyRect = GetRect();

	//Offset our rectangle by the camera position for rendering
	rMyRect.Offset({ -camPos.x, -camPos.y });

	//Render us with the camera
	Camera::GetInstance()->Draw(rMyRect, SGD::Color::Color(255, 255, 0, 0));

	//SGD::GraphicsManager::GetInstance()->DrawString("PRESSED A", { 300, 300 }, { 255, 255, 0, 0 });
}

SGD::Rectangle Boss::GetRect(void) const
{
	return SGD::Rectangle();
}

///////////////////////////////////////////////////
// BasicCollision
// -Clip out of the given entity
void Boss::BasicCollision(const IEntity* pOther)
{
	SetGravity(-3000);

	RECT rBoss;
	rBoss.left = (LONG)GetRect().left;
	rBoss.top = (LONG)GetRect().top;
	rBoss.right = (LONG)GetRect().right;
	rBoss.bottom = (LONG)GetRect().bottom;

	//Create a rectangle for the other object
	RECT rObject;
	rObject.left = (LONG)pOther->GetRect().left;
	rObject.top = (LONG)pOther->GetRect().top;
	rObject.right = (LONG)pOther->GetRect().right;
	rObject.bottom = (LONG)pOther->GetRect().bottom;

	//Create a rectangle for the intersection
	RECT rIntersection = {};

	RECT rBossWall;
	rBossWall.left = (LONG)GetRect().left - 1;
	rBossWall.top = (LONG)GetRect().top;
	rBossWall.right = (LONG)GetRect().right + 1;
	rBossWall.bottom = (LONG)GetRect().bottom;

	IntersectRect(&rIntersection, &rObject, &rBossWall);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;

	//if (nIntersectHeight > nIntersectWidth)
	//{
	//	if (GetIsFalling() == true
	//		|| GetIsJumping() == true)
	//		SetIsInputStuck(true);

	//}

	IntersectRect(&rIntersection, &rBoss, &rObject);

	nIntersectWidth = rIntersection.right - rIntersection.left;
	nIntersectHeight = rIntersection.bottom - rIntersection.top;

	//Colliding with the side of the object
	if (nIntersectHeight > nIntersectWidth)
	{
		if (rBoss.right == rIntersection.right)
		{

			SetPosition({ (float)rObject.left - GetSize().width + 1, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
			//SetDashTimer(0);

			//is_Right_Coll = true;
		}
		if (rBoss.left == rIntersection.left)
		{
			SetPosition({ (float)rObject.right, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
			//SetDashTimer(0);

			//is_Left_Coll = true;

		}
	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (rBoss.bottom == rIntersection.bottom)
		{
			if (IsBouncing() == true)
			{
				SetVelocity({ GetVelocity().x, GetVelocity().y * -1 });
				SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height  /*- nIntersectHeight*/ });
			}

			else
			{
				SetVelocity({ GetVelocity().x, 0 });
				SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 /*- nIntersectHeight*/ });
			}


			//SetJumpVelCur(0);
			//SetIsJumping(false);
			SetIsFalling(false);
			//SetIsInputStuck(false);

			//is_Left_Coll = false;
			//is_Right_Coll = false;
		}
		if (rBoss.top == rIntersection.top)
		{
			SetPosition({ GetPosition().x, (float)rObject.bottom });
			SetVelocity({ GetVelocity().x, 0 });
		}
	}

}

///////////////////////////////////////////////////
// HandleCollision
// -Handle collisions
void Boss::HandleCollision(const IEntity* pOther)
{
	// Solid wall
	if (pOther->GetType() == ENT_SOLID_WALL)
	{
		BasicCollision(pOther);
	}
}
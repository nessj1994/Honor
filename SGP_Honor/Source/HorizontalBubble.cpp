#include "HorizontalBubble.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "DestroyEntityMessage.h"
#include "Camera.h"

HorizontalBubble::HorizontalBubble()
{

//	if (GetOwner()->GetType() == ENT_BOSS_WIZARD)
//	{
//		if (rand() % 2 == 1)
//			SetDirection({ 1, 0 });
//		else
//			SetDirection({ -1, 0 });
//
//	}
//	else
//	{
//		if (rand() % 2 == 1)
//			SetDirection({ 1, 0 });
//		else
//			SetDirection({ -1, 0 });
//	}

	//if (rand() % 2 == 1)
	//	SetDirection({ 1, 0 });
	//else
	//	SetDirection({ -1, 0 });

	SetSpeed(5000);
	if(m_hImage == SGD::INVALID_HANDLE)
	m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/HorizontalBubble.png");
	m_szSize = SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage);
}


HorizontalBubble::~HorizontalBubble()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

void HorizontalBubble::Update(float elapsedTime)
{
	////moving right
	//if (GetDirection().x == 1)
	//{
	//	SetVelocity({ GetVelocity().x + GetSpeed() * elapsedTime, GetVelocity().y });
	//	if (GetVelocity().x > 550)
	//	{
	//		SetVelocity({ 550, GetVelocity().y });
	//	}
	//}
	////Moving Left
	//else
	//{
	//	SetVelocity({ GetVelocity().x - GetSpeed() * elapsedTime, GetVelocity().y });
	//	if (GetVelocity().x < -550)
	//	{
	//		SetVelocity({ -550, GetVelocity().y });
	//	}
	//}
	if (GetOwner()->GetType() == ENT_BOSS_WIZARD)
	{
		SetSpeed(500);
	}


	SetVelocity({ GetVelocity().x + (GetSpeed() * GetDirection().x)* elapsedTime, GetVelocity().y + (GetSpeed() * GetDirection().y)* elapsedTime });

	if (GetVelocity().x > 550)
	{
		SetVelocity({ 550, GetVelocity().y });
	}

	if (GetVelocity().x < -550)
	{
		SetVelocity({ -550, GetVelocity().y });
	}

	Entity::Update(elapsedTime);
}

void HorizontalBubble::Render(void)
{
	/*Camera::GetInstance()->Draw(SGD::Rectangle(
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
		SGD::Color::Color(255, 0, 255, 0));*/
	Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false, 1, SGD::Color(255, 255, 255, 255), {});
}

SGD::Rectangle HorizontalBubble::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void HorizontalBubble::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_SOLID_WALL &&
		pOther != GetOwner())
	{
		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;
	}
	//Did we hit the player
	if (pOther->GetType() == ENT_PLAYER
		&& pOther != GetOwner())
	{
		//Destroy ourself
		DestroyEntityMessage* pMsg = new DestroyEntityMessage{ this };
		pMsg->QueueMessage();
		pMsg = nullptr;


		float i = GetDirection().x;
		SGD::Event Event = { "BOUNCE_HORIZONTAL", &i, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}
}
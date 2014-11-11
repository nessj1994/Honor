#include "GravProjectile.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

GravProjectile::GravProjectile()
{
	SetSpeed(5000);
	//m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/SquidProjectile.png");
	m_szSize = SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage);
}


GravProjectile::~GravProjectile()
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hImage);
}

void GravProjectile::Update(float elapsedTime)
{
	gravTimer += elapsedTime;
	if (gravTimer >= 0.4f)
		gravity = -500.0f;
	//moving right
	if (GetDirection().x == 1)
	{
		SetVelocity({ GetVelocity().x + GetSpeed() * elapsedTime, GetVelocity().y - gravity * elapsedTime });
		if (GetVelocity().x > 550)
		{
			SetVelocity({ 550, GetVelocity().y });
		}
	}
	//Moving Left
	else
	{
		SetVelocity({ GetVelocity().x - GetSpeed() * elapsedTime, GetVelocity().y - gravity * elapsedTime });
		if (GetVelocity().x < -550)
		{
			SetVelocity({ -550, GetVelocity().y });
		}
	}

	Entity::Update(elapsedTime);
}

void GravProjectile::Render(void)
{
	//Camera::GetInstance()->Draw(SGD::Rectangle(
	//	m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
	//	m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
	//	SGD::Color::Color(255, 0, 255, 0));
	Camera::GetInstance()->DrawTexture(m_ptPosition, 0, m_hImage, false, 1, SGD::Color(255, 255, 255, 255), {});
}

SGD::Rectangle GravProjectile::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void GravProjectile::HandleCollision(const IEntity* pOther)
{
	Projectile::HandleCollision(pOther);
}

void GravProjectile::SetProjectileType(ProjectileType _type)
{
	// Save the type
	m_ptType = _type;

	// Load the correct image with the sprite
	switch (_type)
	{
		case INK:
		{
			m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/SquidProjectile.png");
			m_szSize = SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage);
			break;
		}
		case BONE:
		{
			m_hImage = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/BoneProjectile.png");
			m_szSize = SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hImage);
			break;
		}
	}
}
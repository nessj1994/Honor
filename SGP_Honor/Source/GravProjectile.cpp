#include "GravProjectile.h"
#include "Camera.h"


GravProjectile::GravProjectile()
{
	SetSpeed(5000);
}


GravProjectile::~GravProjectile()
{
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
	Camera::GetInstance()->Draw(SGD::Rectangle(
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
		SGD::Color::Color(255, 0, 255, 0));
}

SGD::Rectangle GravProjectile::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}

void GravProjectile::HandleCollision(const IEntity* pOther)
{
	Projectile::HandleCollision(pOther);
}
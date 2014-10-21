#include "Enemy.h"


Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}


/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Enemy::Update(float elapsedTime)
{


}


void Enemy::Render(void)
{

}


int Enemy::GetType(void) const
{
	return 1;
}


SGD::Rectangle Enemy::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };
}


void Enemy::HandleCollision(const IEntity* pOther)
{
	Unit::HandleCollision(pOther);

	

}
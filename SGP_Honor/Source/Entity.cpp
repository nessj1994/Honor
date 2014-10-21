#include "Entity.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <cassert>



Entity::~Entity()
{
}


/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Entity::Update(float elapsedTime)
{
	m_ptPosition += m_vtVelocity * elapsedTime;

}

void Entity::Render(void)
{
	SGD::GraphicsManager::GetInstance()->DrawTexture(
		m_hImage, m_ptPosition,
		(0.0f), m_szSize / 2, {}, m_szScale);

}

int Entity::GetType(void) const
{

	return 1;
}


SGD::Rectangle Entity::GetRect(void) const
{

	return SGD::Rectangle{ m_ptPosition, m_szSize };
	
}
void Entity::HandleCollision(const IEntity* pOther)
{

	(void)pOther;
}

///////////////////////////////////////////////
//////////////Reference Counting//////////////
void Entity::AddRef(void)
{

	++m_unRefCount;
}
void Entity::Release(void)
{
	--m_unRefCount;

	if(m_unRefCount == 0)
	{
		delete this;
		
	}
}







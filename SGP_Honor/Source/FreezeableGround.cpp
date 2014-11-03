#include "FreezeableGround.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "ParticleEngine.h"
#include "Camera.h"
#include "Unit.h"
#include "Emitter.h"

FreezeableGround::FreezeableGround()
{

	///////Enum type for Temp or non Temp type (Set in Factory method) used in method to alter the Timer of the freeze to change or not change depending on the type

	m_eEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/SprayParticle.xml", "Ice", m_ptPosition);

}


FreezeableGround::~FreezeableGround()
{
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void FreezeableGround::Update(float elapsedTime)
{
	//Emitter Update
	m_eEffect->Update(elapsedTime);
	//
	if (m_bPermFrozen == false
		&& m_bIsFrozen == true)
	{
		m_fFreezeTimer += elapsedTime;
	}
	else if (m_bPermFrozen == false
		&& m_bIsFrozen == false)
	{
		m_fFreezeTimer = 0.0f;
	}

	

	if (m_fFreezeTimer > 2.0f
		&& m_bPermFrozen == false)
	{
		m_bIsFrozen = false;
		m_fFreezeTimer = 0.0f;
		this->SetType(ENT_NOT_FROZEN);


	}

}
void FreezeableGround::Render(void)
{
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	

	//Camera::GetInstance()->Draw(SGD::Rectangle(10, 300, 20, 320), SGD::Color::Color(255, 0, 0, 255));

	if (GetIsFrozen() == false)
	{

	Camera::GetInstance()->Draw(SGD::Rectangle(
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
		SGD::Color::Color(255, 255, 0, 255));

	}

	if (GetIsFrozen() == true)
	{
		m_eEffect->Render(m_ptPosition);
		Camera::GetInstance()->Draw(SGD::Rectangle(
			m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
			m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
			SGD::Color::Color(255, 0, 255, 255));
	}

}



SGD::Rectangle FreezeableGround::GetRect(void) const
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };


}

void FreezeableGround::HandleCollision(const IEntity* pOther)
{

	if (m_bPermFrozen == false
		&& pOther->GetType() == ENT_SPRAY)
	{
		this->SetIsFrozen(true);
		this->SetType(ENT_FROZEN);
	}


	if (m_bIsFrozen == true)
	{
		//GetType() == ENT
	
	}

}
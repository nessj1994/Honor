#include "Dash.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

Dash::Dash()
{
	m_hEffect = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/DashEffect.wav");
}


Dash::~Dash()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hEffect);
	m_hEffect = SGD::INVALID_HANDLE;
}


void Dash:: CastDash(Unit* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hEffect);
		pOther->SetIsDashing(true);
		
		if (pOther->IsFacingRight() == true)
			pOther->SetVelocity({ pOther->GetVelocity().x + 1000, pOther->GetVelocity().y });
		else
			pOther->SetVelocity({ pOther->GetVelocity().x - 1000, pOther->GetVelocity().y });



	}
}

#include "Dash.h"
#include "ParticleEngine.h"
#include "Emitter.h"
#include "Player.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

Dash::Dash()
{
	m_hEffect = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/DashEffect.wav");
	m_emDash = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/DashEffect.xml", "Dash", { 0, 0 });
}


Dash::~Dash()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hEffect);
	m_hEffect = SGD::INVALID_HANDLE;
	delete m_emDash;
}


void Dash:: CastDash(Unit* pOther)
{
	if (pOther->GetType() == ENT_PLAYER)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hEffect);
		pOther->SetIsDashing(true);
		
		if (((Player*)pOther)->Slowed())
		{
			if (pOther->IsFacingRight() == true)
			{
				pOther->SetPosition({ pOther->GetPosition().x + 1, pOther->GetPosition().y });
				pOther->SetVelocity({ 800, 0 });

			}
			else
			{
				pOther->SetPosition({ pOther->GetPosition().x - 1, pOther->GetPosition().y });

				pOther->SetVelocity({ -800, 0 });
			}
		}
		else
		{
			if (pOther->IsFacingRight() == true)
			{
				pOther->SetPosition({ pOther->GetPosition().x + 1, pOther->GetPosition().y });

				pOther->SetVelocity({ 2000, 0 });
			}
			else
				pOther->SetVelocity({ -2000, 0 });
		}
		



	}
}

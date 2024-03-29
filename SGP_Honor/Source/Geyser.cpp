#include "Geyser.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "Camera.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "ParticleEngine.h"
#include "Emitter.h"

Geyser::Geyser()
{

	m_szSize = { 32, 32 };
	m_bRising = true;
	m_ptOrigPos = m_ptPosition;
	m_szOrigSize = m_szSize;
	
	m_hEffect = SGD::AudioManager::GetInstance()->LoadAudio("assets/audio/Geyser.wav");
	m_EmWaterEffect = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/WaterEffect.xml", "Water", m_ptPosition);
}


Geyser::~Geyser()
{
	delete m_EmWaterEffect;
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hEffect);
}

/////////////////////////////////////////////////
/////////////////Interface//////////////////////
void Geyser::Update(float elapsedTime)
{
	m_EmWaterEffect->SetSize(m_szSize);
	m_EmWaterEffect->SetPosition(m_ptPosition);
	m_EmWaterEffect->Update(elapsedTime);

	ChangePillar(m_fMaxHeight, elapsedTime);
}
void Geyser::Render(void)
{

	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();



	Camera::GetInstance()->Draw(SGD::Rectangle(
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
		SGD::Color::Color(100, 0, 0, 155));

	m_EmWaterEffect->Render();

}

void Geyser::HandleCollision(const IEntity* pOther)
{
	if (pOther->GetType() == ENT_SPRAY)
	{
		m_bSPRAYED = true;

		m_fSprayTimer = 4.0f;
	}


}

void Geyser::ChangePillar(float _height, float _delta)
{

	if (m_bSPRAYED == false)
	{
		if ((m_ptPosition.y + m_szSize.height) < (m_ptOrigPos.y + m_szOrigSize.height) + _height
			&& m_bRising == true)
		{
			m_ptPosition.y -= m_fSpeed * _delta;
			m_szSize.height += m_fSpeed * _delta;
		}

		if ((m_ptPosition.y) <= m_ptOrigPos.y - _height
			&& m_bRising == true)
		{
			m_bRising = false;
			m_bApex = true;
		}

		if (m_bApex == true)
		{
			if (m_fApexTimer > 0)
			{
				m_fApexTimer -= _delta;
			}
			else
			{
				m_fApexTimer = 2.0f;
				m_bApex = false;
				m_bFalling = true;
			}
		}

		if (m_ptPosition.y <
			(m_ptOrigPos.y)
			&& m_bFalling == true)
		{
			m_ptPosition.y += m_fSpeed * _delta;
			m_szSize.height -= m_fSpeed * _delta;
		}

		if ((m_ptPosition.y) >= (m_ptOrigPos.y)
			&& m_bFalling == true)
		{
			m_bFalling = false;
			m_bResting = true;
		}

		if (m_bResting == true)
		{
			if (m_fApexTimer > 0)
			{
				m_fApexTimer -= _delta;
			}
			else
			{
				m_fApexTimer = 2.0f;
				m_bResting = false;
				m_bRising = true;
			}
		}
	}

	if (m_bSPRAYED == true)
	{

		m_ptPosition.y = m_ptOrigPos.y - _height;
		m_szSize.height = m_szOrigSize.height + _height;
		m_fSprayTimer -= _delta;

		if (m_fSprayTimer <= 0)
		{
			m_bSPRAYED = false;
			m_fSprayTimer = 4.0f;
			m_bFalling = true;
		}
	}

	if(m_bResting == false)
	{
		if(!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hEffect))
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_hEffect);
		}
	}
	else
		SGD::AudioManager::GetInstance()->StopAudio(m_hEffect);


}

int Geyser::GetType(void) const
{
	if (m_bSPRAYED == false)
		return ENT_GEYSER;
	else
		return ENT_SOLID_WALL;

	
}

void Geyser::SetState(int _state)
{
	if (_state == 0)
	{
		m_bRising = true;
		m_bApex = false;
		m_bFalling = false;
		m_bResting = false;
	}
	else if (_state == 1)
	{
		m_bRising = false;
		m_bApex = true;
		m_bFalling = false;
		m_bResting = false;
	}
}
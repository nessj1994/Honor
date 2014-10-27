#include "Emitter.h"
#include "Particle.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "Camera.h"

Emitter::Emitter()
{
	m_ptPosition = { 0, 0 };
	m_szSize = { 100, 100 };
	m_bLooping = true;
	m_unMaxParticles = 10;

}


Emitter::~Emitter()
{
}

void Emitter::StartParticles()
{
	m_Particles.clear();
	for (size_t i = 0; i < m_unMaxParticles; i++)
	{
		std::mt19937 MT(device());
		std::uniform_real_distribution<float>thing(m_ptPosition.x,m_ptPosition.x + m_szSize.width);
		std::uniform_real_distribution<float>thing2(m_ptPosition.y, m_ptPosition.y + m_szSize.height);
		std::uniform_real_distribution<float>VelX(m_vtMinVelocity.x,m_vtMaxVelocity.x );
		std::uniform_real_distribution<float>VelY(m_vtMinVelocity.y, m_vtMaxVelocity.y);
						//Position																																//Life Time
		//Particle Temp({ (m_ptPosition.x + (rand() % ((int)m_szSize.width - (0 + 1))) + 0), (m_ptPosition.y + (rand() % ((int)m_szSize.height - (0 + 1))) + 0) }, rand() % (int)(m_fMaxLifeSpan - (m_fMinLifeSpan + 1)) + m_fMinLifeSpan);
		Particle Temp({thing(MT),thing2(MT)}, rand() % (int)(m_fMaxLifeSpan - (m_fMinLifeSpan + 1)) + m_fMinLifeSpan);
		Temp.SetAlphaStart(m_fAlphaStart);
		Temp.SetAlphaFade(m_fAlphaFade);
		Temp.SetStartColor(m_cStartColor);
		Temp.SetEndColor(m_cEndColor);
		Temp.SetRotation(m_fRotation);
		float Width = m_szScale.width;
		float Height = m_szScale.height;
		Temp.SetScale({Width,Height});
		Temp.SetVelocity({VelX(MT),VelY(MT)});
		Temp.SetImage(m_hImage);
		Temp.SetColorChange(m_fColorChange);
		Temp.Reset();
		m_Particles.push_back(Temp);
	}
}

///////////////////////////////////////////////
////////////////////Interface/////////////////
void Emitter::Update(float elapsedTime)
{

		for (int i = 0; i < m_Particles.size(); i++)
		{
			if (m_Particles[i].IsDead())
			{
				Recylce(&m_Particles[i]);
			}
			m_Particles[i].Update(elapsedTime);
		}

}


void Emitter::Render(SGD::Point _Pos)
{
	m_ptPosition = _Pos;
	SGD::Rectangle Rect{ { m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y }, m_szSize };
	SGD::GraphicsManager::GetInstance()->DrawRectangle(Rect, {255,0,0,0}, {}, 2);

	for (size_t i = 0; i < m_Particles.size(); i++)
	{
		m_Particles[i].Render();
	}
}


void Emitter::Recylce(Particle* particle)
{
	if (m_bLooping)
	{
		//Random Generator
		std::mt19937 MT(device());
		std::uniform_real_distribution<float>thing(m_ptPosition.x, m_ptPosition.x + m_szSize.width);
		std::uniform_real_distribution<float>thing2(m_ptPosition.y, m_ptPosition.y + m_szSize.height);
		//
		particle->SetPosition({ thing(MT), thing2(MT) });
		particle->SetColorChange(m_fColorChange);
		particle->Reset();
	}
	
}
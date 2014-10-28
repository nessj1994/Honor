#include "Emitter.h"
#include "Particle.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include <math.h>
#include "Camera.h"

Emitter::Emitter()
{
	m_ptPosition = { 96, 672 };
	m_szSize = { 100, 100 };
	m_bLooping = true;
	m_unMaxParticles = 10;
	m_bStarted = false;

}

Emitter::Emitter(const Emitter& _Emitter)
{
	m_hImage = _Emitter.m_hImage;
	m_vtMinVelocity = _Emitter.m_vtMinVelocity;
	m_vtMaxVelocity = _Emitter.m_vtMaxVelocity;
	m_fGravity = _Emitter.m_fGravity;
	m_cStartColor = _Emitter.m_cStartColor;
	m_cEndColor = _Emitter.m_cEndColor;
	m_ptPosition = _Emitter.m_ptPosition;
	m_EndPoint = _Emitter.m_EndPoint;
	m_szSize = _Emitter.m_szSize;
	m_szMaxParticleSize = _Emitter.m_szMaxParticleSize;
	m_szMinParticleSize = _Emitter.m_szMinParticleSize;
	m_iEmitterShape = _Emitter.m_iEmitterShape;
	m_iSpinSpeed = _Emitter.m_iSpinSpeed;
	m_iRadius = _Emitter.m_iRadius;
	m_bPinEdges = _Emitter.m_bPinEdges;
	m_fColorChange = _Emitter.m_fColorChange;
	m_fRotation = _Emitter.m_fRotation;
	m_fMaxLifeSpan = _Emitter.m_fMaxLifeSpan;
	m_fMinLifeSpan = _Emitter.m_fMinLifeSpan;
	m_fGravity = _Emitter.m_fGravity;
	m_szScale = _Emitter.m_szScale;
	m_fAlphaStart = _Emitter.m_fAlphaStart;
	m_fAlphaFade = _Emitter.m_fAlphaFade;
	m_fMinScaleRate = _Emitter.m_fMinScaleRate;
	m_fMaxScaleRate = _Emitter.m_fMaxScaleRate;
	m_unMaxParticles = _Emitter.m_unMaxParticles;
	m_bLooping = _Emitter.m_bLooping;
	//Timers
	m_fSpinTimer = _Emitter.m_fSpinTimer;

	m_Particles = _Emitter.m_Particles;
	m_bStarted = _Emitter.m_bStarted;
}

Emitter& Emitter::operator= (const Emitter& _other)
{
	if (this != &_other)
	{
		Emitter(_other);
	}
	return *this;
}

Emitter::~Emitter()
{
}

void Emitter::StartParticles()
{
	if (m_bStarted)
	{
		return;
	}
	else
	{
		m_bStarted = true;
	}
	if (m_iEmitterShape)
	{
		m_EndPoint.y = m_ptPosition.y;
		m_EndPoint.x = m_ptPosition.x + m_iRadius;
	}
	m_Particles.clear();
	for (size_t i = 0; i < m_unMaxParticles; i++)
	{
		std::mt19937 MT(device());
		std::uniform_real_distribution<float>thing(m_ptPosition.x,m_ptPosition.x + m_szSize.width);
		std::uniform_real_distribution<float>thing2(m_ptPosition.y, m_ptPosition.y + m_szSize.height);
		std::uniform_real_distribution<float>VelX(m_vtMinVelocity.x,m_vtMaxVelocity.x );
		std::uniform_real_distribution<float>VelY(m_vtMinVelocity.y, m_vtMaxVelocity.y);
		float VX = VelX(MT);
		if (VX > 0)
		{
			VX += 40;
		}
		else if (VX != 0)
		{
			VX -= 40;
		}
		float VY = VelY(MT);
		if (VY > 0)
		{
			VY += 40;
		}
		else if (VY != 0)
		{
			VY -= 40;
		}
						//Position																																//Life Time
		//Particle Temp({ (m_ptPosition.x + (rand() % ((int)m_szSize.width - (0 + 1))) + 0), (m_ptPosition.y + (rand() % ((int)m_szSize.height - (0 + 1))) + 0) }, rand() % (int)(m_fMaxLifeSpan - (m_fMinLifeSpan + 1)) + m_fMinLifeSpan);
		std::uniform_real_distribution<float>LifeSpan(m_fMinLifeSpan, m_fMaxLifeSpan);
		Particle Temp({ thing(MT), thing2(MT) }, LifeSpan(MT));
		if (m_iEmitterShape)
		{
			if (m_bPinEdges)
			{
				Temp.SetPosition(m_EndPoint);
			}
			else
			{
				int MAX = (int)(__max(m_ptPosition.y, m_EndPoint.y));
				int MIN = (int)(__min(m_ptPosition.y, m_EndPoint.y));
				Temp.SetPosition({ (float)(rand() % (((int)__max(m_ptPosition.x, m_EndPoint.x) - ((int)__min(m_ptPosition.x, m_EndPoint.x) + 1)) + (int)__min(m_ptPosition.x, m_EndPoint.x))),
					(float)(rand() % (MAX - (MIN + 1)) + MIN) });
			}

		}
		Temp.SetAlphaStart(m_fAlphaStart);
		Temp.SetAlphaFade(m_fAlphaFade);
		Temp.SetStartColor(m_cStartColor);
		Temp.SetEndColor(m_cEndColor);
		Temp.SetRotation(m_fRotation);
		float Width = m_szScale.width;
		float Height = m_szScale.height;
		Temp.SetScale({Width,Height});
		Temp.SetVelocity({ VX, VY });
		Temp.SetImage(m_hImage);
		Temp.SetGravity(m_fGravity);
		Temp.SetColorChange(m_fColorChange);
		Temp.Reset();
		m_Particles.push_back(Temp);
	}
}

///////////////////////////////////////////////
////////////////////Interface/////////////////
void Emitter::Update(float elapsedTime)
{
	if (m_iEmitterShape)
	{
		m_fSpinTimer += elapsedTime;
		if (m_fSpinTimer >= .01f)
		{
			m_fSpinTimer = 0;
			double A[2] = { m_EndPoint.x, m_EndPoint.y };
			double x = (A[0] - m_ptPosition.x) * cos(((double)m_iSpinSpeed * (3.14 / 180))) - (A[1] - m_ptPosition.y) * sin(((double)m_iSpinSpeed * (3.14 / 180))) + m_ptPosition.x;
			double y = (A[1] - m_ptPosition.y) * cos(((double)m_iSpinSpeed * (3.14 / 180))) + (A[0] - m_ptPosition.x) * sin(((double)m_iSpinSpeed * (3.14 / 180))) + m_ptPosition.y;
			m_EndPoint.x = x;
			m_EndPoint.y = y;
		}
	}

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
	if (_Pos != SGD::Point(0,0))
	{
		m_ptPosition = _Pos;
	}
	
	if (m_iEmitterShape)
	{
		SGD::GraphicsManager::GetInstance()->DrawLine({ m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y }, { m_EndPoint.x - Camera::GetInstance()->GetCameraPos().x, m_EndPoint.y - Camera::GetInstance()->GetCameraPos().y });	
	}
	else
	{
		
		SGD::Rectangle Rect{ { m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y }, m_szSize };
		SGD::GraphicsManager::GetInstance()->DrawRectangle(Rect, { 255, 0, 0, 0 }, {}, 2);
	}


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
		std::uniform_real_distribution<float>X(m_ptPosition.x, m_ptPosition.x + m_szSize.width);
		std::uniform_real_distribution<float>Y(m_ptPosition.y, m_ptPosition.y + m_szSize.height);
		//
		particle->SetPosition({ X(MT), Y(MT) });
		if (m_iEmitterShape)
		{
			if (m_bPinEdges)
			{
				particle->SetPosition(m_EndPoint);
			}
			else
			{
				int MAX = (int)(__max(m_ptPosition.y, m_EndPoint.y));
				int MIN = (int)(__min(m_ptPosition.y, m_EndPoint.y))-1;
				particle->SetPosition({ (float)(rand() % (((int)__max(m_ptPosition.x, m_EndPoint.x) - ((int)__min(m_ptPosition.x, m_EndPoint.x) +1)) + (int)__min(m_ptPosition.x, m_EndPoint.x))),
					(float)(rand() % (MAX - (MIN + 1)) + MIN) });
			}

		}
		particle->SetColorChange(m_fColorChange);
		particle->Reset();
	}
	
}
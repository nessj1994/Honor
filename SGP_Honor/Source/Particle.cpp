#include "Particle.h"
#include "Camera.h"



Particle::Particle(SGD::Point Pos, float LifeSpan)
{
	m_ptPosition = Pos;
	m_szSize = { .2f, .2f };
	m_blDead = false;
	m_fLifeSpan = LifeSpan;
	std::mt19937 MT(device());
	std::uniform_real_distribution<float>thing(0,LifeSpan);
	m_fCurLifeSpan = thing(MT);
	m_fCurRotation = 0;
	m_bStart = true;

}

Particle::Particle(const Particle& _Particle)
{
	m_ptPosition = _Particle.GetPosition();
	m_szSize = _Particle.GetSize();
	m_blDead = _Particle.IsDead();
	m_fLifeSpan = _Particle.GetLifeSpan();
	m_fCurLifeSpan = _Particle.GetCurLifeSpan();
	m_fAlphaFade = _Particle.GetAlphaFade();
	m_hTexture = _Particle.GetTexture();
	m_vtVelocity = _Particle.GetVelocity();
	m_vtAcceleration = _Particle.GetAcceleration();
	m_vtGravity = _Particle.GetGravity();
	m_cStartColor = _Particle.GetStartColor();
	m_cEndColor = _Particle.GetEndColor();
	m_szScale = _Particle.GetScale();
	m_fScaleChange = _Particle.GetScaleChange();
	m_fAlphaStart = _Particle.GetAlphaStart();
	m_fCurrentAlpha = _Particle.GetCurAlpha();
	m_iColorChange = _Particle.m_iColorChange;
	m_cCurrentColor = _Particle.m_cCurrentColor;
	m_cStartColor = _Particle.m_cStartColor;
	m_cEndColor = _Particle.m_cEndColor;
	m_fRotation = _Particle.m_fRotation;
	RotationChangeTImer = 0;

}

Particle& Particle::operator=(const Particle& _other)
{
	if (this != &_other)
	{
		Particle Temp(_other);
		return Temp;
	}	
	return *this;
}

Particle::~Particle()
{
}


///////////////////////////////////////////////
////////////////////Interface/////////////////
void Particle::Update(float elapsedTime)
{
	if (m_blDead)
	{
		return;
	}
	if (m_fCurLifeSpan >= m_fLifeSpan)
	{
		m_blDead = true;
		m_bStart = true;

	}
	else
	{
		//Smooth this out Later
		m_fCurLifeSpan += elapsedTime;
	}
	//Smooth this out later
	m_ptPosition.x += m_vtVelocity.x * elapsedTime;
	m_ptPosition.y += (m_vtVelocity.y * elapsedTime) + m_vtGravity/15;
	//Timer Changes
	AlphaChangeTimer += elapsedTime;
	ColorChangeTimer += elapsedTime;
	RotationChangeTImer += elapsedTime;
	//Rotation 
	if (RotationChangeTImer >= 0.01f)
	{
		RotationChangeTImer = 0;
		m_fCurRotation += m_fRotation;
	}
	//Smooth this out later
	if (AlphaChangeTimer >= 0.04f)
	{		
		AlphaChangeTimer = 0;
		if (m_cCurrentColor.alpha > 0)
		{
			int count = 0;
			while (m_cCurrentColor.alpha > 2 && count != m_fAlphaFade)
			{
				m_cCurrentColor.alpha -= 2;
				count++;
			}
		}
	}
		if (ColorChangeTimer >= 0.06f)
		{
			ColorChangeTimer = 0;
			if (m_iColorChange > 0)
			{
				//incrementing
				if (m_cCurrentColor.red < m_cEndColor.red)
				{
					int count = 0;
					while (m_cCurrentColor.red < 255 && count != m_iColorChange)
					{
						m_cCurrentColor.red++;
						count++;
					}
				}
				if (m_cCurrentColor.green < m_cEndColor.green)
				{
					int count = 0;
					while (m_cCurrentColor.green < 255 && count != m_iColorChange)
					{
						m_cCurrentColor.green++;
						count++;
					}
				}
				if (m_cCurrentColor.blue < m_cEndColor.blue)
				{
					int count = 0;
					while (m_cCurrentColor.blue < 255 && count != m_iColorChange)
					{
						m_cCurrentColor.blue++;
						count++;
					}
				}
				///Decrementing
				if (m_cCurrentColor.red > m_cEndColor.red)
				{
					int count = 0;
					while (m_cCurrentColor.red > 2 && count != m_iColorChange + 10)
					{
						m_cCurrentColor.red -= 2;
						count++;
					}
				}
				if (m_cCurrentColor.green > m_cEndColor.green)
				{
					int count = 0;
					while (m_cCurrentColor.green > 2 && count != m_iColorChange + 10)
					{
						m_cCurrentColor.green -= 2;
						count++;
					}
				}
				if (m_cCurrentColor.blue > m_cEndColor.blue)
				{
					int count = 0;
					while (m_cCurrentColor.blue > 2 && count != m_iColorChange + 10)
					{
						m_cCurrentColor.blue -= 2;
						count++;
					}
				}
			}
		}
}


void Particle::Render(void)
{
	if (m_blDead || !m_bStart)
	{
		return;
	}
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(Rect, { (unsigned char)m_fCurrentAlpha, 0, 255, 0 }, { (unsigned char)m_fCurrentAlpha,0,255,0 }, 10);
	//{ GetPosition().x - Camera::GetInstance()->GetCameraPos().x, GetPosition().y - Camera::GetInstance()->GetCameraPos().y }
	SGD::Point Temp = { m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y };
	if (m_hTexture != SGD::INVALID_HANDLE)
	{
		Temp.x -= (SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hTexture) / 2).width * m_szScale.width;
		Temp.y -= (SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hTexture) / 2).height * m_szScale.height;
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hTexture, Temp, m_fCurRotation, {Temp.x/2,Temp.y/2}, m_cCurrentColor, m_szScale);
	}
	else
	{
		SGD::Rectangle Rect{ Temp, m_szScale };
		SGD::GraphicsManager::GetInstance()->DrawRectangle(Rect, m_cCurrentColor);
	}
}

void Particle::RenderINworld()
{
	if (m_blDead)
	{
		return;
	}
	//SGD::GraphicsManager::GetInstance()->DrawRectangle(Rect, { (unsigned char)m_fCurrentAlpha, 0, 255, 0 }, { (unsigned char)m_fCurrentAlpha,0,255,0 }, 10);
	//{ GetPosition().x - Camera::GetInstance()->GetCameraPos().x, GetPosition().y - Camera::GetInstance()->GetCameraPos().y }
	SGD::Point Temp = { m_ptPosition.x , m_ptPosition.y  };
	if (m_hTexture != SGD::INVALID_HANDLE)
	{
		Temp.x -= (SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hTexture) / 2).width * m_szScale.width;
		Temp.y -= (SGD::GraphicsManager::GetInstance()->GetTextureSize(m_hTexture) / 2).height * m_szScale.height;
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hTexture, Temp, m_fCurRotation, { Temp.x / 2, Temp.y / 2 }, m_cCurrentColor, m_szScale);
	}
	else
	{
		SGD::Rectangle Rect{ Temp, m_szScale };
		SGD::GraphicsManager::GetInstance()->DrawRectangle(Rect, m_cCurrentColor);
	}
}

void Particle::Reset()
{
	m_fCurLifeSpan = 0;
	m_blDead = false;
	m_cCurrentColor.alpha = m_fAlphaStart;
	m_cCurrentColor = m_cStartColor;
} 
#pragma once

#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

class Particle
{
public:
	Particle(SGD::Point Pos, float LifeSpan);
	Particle(const Particle& _Particle);
	Particle& operator= (const Particle& _other);
	~Particle();

	///////////////////////////////////////////////
	////////////////////Interface/////////////////
	void Update(float elapsedTime);
	void Render(void);
	void RenderINworld();

	///////////////////////////////////////////////////////////////////
	////////////////////////////Accessors/////////////////////////////
	SGD::Point GetPosition(void) const { return m_ptPosition; }
	SGD::Vector GetVelocity(void) const { return m_vtVelocity; }
	SGD::Vector GetAcceleration(void) const { return m_vtAcceleration; }
	float GetGravity(void) const { return m_vtGravity; }
	SGD::Color GetStartColor(void) const { return m_cStartColor; }
	SGD::Color GetEndColor(void) const { return m_cEndColor; }
	SGD::Size GetSize(void) const { return m_szSize; }
	float GetTangentialAcceleration(void) const { return m_fTangentialAcceleration; }
	SGD::Size GetScale(void) const { return m_szScale; }
	float GetScaleChange(void) const { return m_fScaleChange; }
	float GetAlphaFade(void) const { return m_fAlphaFade; }
	float GetAlphaStart(void) const { return m_fAlphaStart; }
	float GetLifeSpan(void) const { return m_fLifeSpan; }
	float GetCurLifeSpan(void) const { return m_fCurLifeSpan; }
	float GetCurAlpha(void) const { return m_fCurrentAlpha; }
	SGD::HTexture GetTexture() const { return m_hTexture; }
	bool IsDead() const { return m_blDead; }

	////////////////////////////////////////////////////////////////
	/////////////////////////////Mutators//////////////////////////
	void SetPosition(SGD::Point position) { m_ptPosition = position; }
	void SetVelocity(SGD::Vector velocity) { m_vtVelocity = velocity; }
	void SetAcceleration(SGD::Vector acceleration) { m_vtAcceleration = acceleration; }
	void SetGravity(float gravity) { m_vtGravity = gravity; }
	void SetStartColor(SGD::Color color) { m_cStartColor = color; }
	void SetEndColor(SGD::Color color) { m_cEndColor = color; }
	void SetSize(SGD::Size size) { m_szSize = size; }
	void SetTangentialAcceleration(float tanAccel) { m_fTangentialAcceleration = tanAccel; }
	void SetScale(SGD::Size scale) { m_szScale = scale; }
	void SetScaleChange(float change) { m_fScaleChange = change; }
	void SetAlphaFade(float alphaFade) { m_fAlphaFade = alphaFade; }
	void SetAlphaStart(float AlphaFade) { m_fAlphaStart = AlphaFade; m_fCurrentAlpha = AlphaFade; }
	void SetLifeSpan(float life) { m_fLifeSpan = life; }
	void SetImage(SGD::HTexture _Image){ m_hTexture = _Image; }
	void SetColorChange(int _Change) { m_iColorChange = _Change; }
	void SetRotation(float _Rotation) { m_fRotation = _Rotation; }
	//For the Recycle in the Emitter
	void Reset();
private:


	/////////////////////////////////////////
	////////////////Members/////////////////
	SGD::HTexture m_hTexture;
	SGD::Point m_ptPosition = { 0, 0 };
	SGD::Vector m_vtVelocity = { 0, 0 };
	SGD::Vector m_vtAcceleration = { 0, 0 };
	float m_vtGravity;
	SGD::Color m_cStartColor;
	SGD::Color m_cEndColor;
	SGD::Color m_cCurrentColor;
	float m_fRotation;
	float m_fCurRotation = 0;
	int m_iColorChange;
	SGD::Size m_szSize = { 0, 0 };
	float m_fTangentialAcceleration = 0.0f;
	SGD::Size m_szScale = { 0.0f, 0.0f };
	float m_fScaleChange = 0.0f;
	float m_fAlphaStart = 0.0f;
	float m_fCurrentAlpha = 0.0f;
	float m_fAlphaFade = 255.0f;
	float m_fLifeSpan = 0.0f;
	//Timers for accurate operation
	float ColorChangeTimer = 0.0f;
	float AlphaChangeTimer = 0.0f;
	float RotationChangeTImer = 0.0f;
	//Time its been alive;
	float m_fCurLifeSpan;
	bool m_blDead;
};


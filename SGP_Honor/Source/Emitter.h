#pragma once

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Color.h"
#include <random>

#include <vector>
//Forward Declarations
class Particle;

class Emitter
{
public:
	Emitter();
	Emitter(const Emitter& _Emitter);
	Emitter& operator= (const Emitter& _other);
	~Emitter();

	///////////////////////////////////////////////
	////////////////////Interface/////////////////
	void Update(float elapsedTime);
	void Render(SGD::Point _Pos = {});
	void RenderINworld(SGD::Point _Pos = {});
	void Recylce(Particle* particle);
	void StartParticles(bool restart = false);
	void KillParticles(SGD::Point _Pos);

	/////////////////////////////////////////////
	////////////////Accessors///////////////////
	SGD::HTexture GetImage(void) const { return m_hImage; }
	SGD::Vector GetMinVelocity(void) const { return m_vtMinVelocity; }
	SGD::Vector GetMaxVelocity(void) const { return m_vtMaxVelocity; }
	SGD::Vector GetMinAcceleration(void) const { return m_vtMinAcceleration; }
	SGD::Vector GetMaxAcceleration(void) const { return m_vtMaxAcceleration; }
	SGD::Vector GetMinGravity(void) const { return m_vtMinGravity; }
	SGD::Vector GetMaxGravity(void) const { return m_vtMaxGravity; }
	SGD::Color GetStartColor(void) const { return m_cStartColor; }
	SGD::Color GetEndColor(void) const { return m_cEndColor; }
	SGD::Point GetPosition(void) const { return m_ptPosition; }
	SGD::Size GetSize(void) const { return m_szSize; }
	float GetMaxLifeSpan(void) const { return m_fMaxLifeSpan; }
	float GetMinLifeSpan(void) const { return m_fMinLifeSpan; }
	SGD::Size GetScale(void) const { return m_szScale; }
	float GetMinScaleRate(void) const { return m_fMinScaleRate; }
	float GetMaxScaleRate(void) const { return m_fMaxScaleRate; }
	unsigned int GetMaxParticles(void) const { return m_unMaxParticles; }
	bool GetLooping(void) const { return m_bLooping; }




	/////////////////////////////////////////////
	/////////////////Mutators///////////////////
	void SetImage(SGD::HTexture image)  {  m_hImage = image; }
	void SetMinVelocity(SGD::Vector minVelocity)  {  m_vtMinVelocity = minVelocity; }
	void SetMaxVelocity(SGD::Vector maxVelocity)  {  m_vtMaxVelocity = maxVelocity; }
	void SetMinAcceleration(SGD::Vector minAcceleration)  {  m_vtMinAcceleration = minAcceleration; }
	void SetMaxAcceleration(SGD::Vector maxAcceleration)  {  m_vtMaxAcceleration = maxAcceleration; }
	void SetMinGravity(SGD::Vector minGravity)  {  m_vtMinGravity = minGravity; }
	void SetMaxGravity(SGD::Vector maxGravity)  {  m_vtMaxGravity = maxGravity; }
	void SetStartColor(SGD::Color color)  {  m_cStartColor = color; }
	void SetEndColor(SGD::Color color)  {  m_cEndColor = color; }
	void SetPosition(SGD::Point position)  {  m_ptPosition = position; }
	void SetSize(SGD::Size size)  {  m_szSize = size; }
	void SetMaxLifeSpan(float LifeSpan)  { m_fMaxLifeSpan = LifeSpan; }
	void SetMinLifeSpan(float Lifespan)  {  m_fMinLifeSpan = Lifespan; }
	void SetScale(SGD::Size Scale)  {  m_szScale = Scale; }
	void SetMinScaleRate(float minScaleRate)  {  m_fMinScaleRate = minScaleRate; }
	void SetMaxScaleRate(float maxScaleRate)  {  m_fMaxScaleRate = maxScaleRate; }
	void SetMaxParticles(unsigned int maxParticles)  {  m_unMaxParticles = maxParticles; }
	void SetLooping(bool looping)  { m_bLooping = looping; }
	void SetMaxSize(SGD::Size _Size) { m_szMaxParticleSize = _Size; }
	void SetMinSize(SGD::Size _Size) { m_szMinParticleSize = _Size; }
	void SetAlphaStart(float _AlphaStart) { m_fAlphaStart = _AlphaStart; }
	void SetAlphaFade(float _AlphaFade) { m_fAlphaFade = _AlphaFade; }
	void SetGravity(float _Gravity) { m_fGravity = _Gravity; }
	void SetRotation(float _Rotation) { m_fRotation = _Rotation; }
	void SetColorChange(float _ColorChange) { m_fColorChange = _ColorChange; }
	void SetEmitterShape(int _EmitterShape) { m_iEmitterShape = _EmitterShape; }
	void SetSpinSpeed(int _SpinSpeed){ m_iSpinSpeed = _SpinSpeed; }
	void SetRadius(int _Radius){ m_iRadius = _Radius; }
	void PinEdges(bool _PinEdges){ m_bPinEdges = _PinEdges; }
	
	
private:
	std::random_device device;
	SGD::HTexture m_hImage = SGD::INVALID_HANDLE;
	SGD::Vector m_vtMinVelocity = { 0, 0 };
	SGD::Vector m_vtMaxVelocity = { 0, 0 };
	SGD::Vector m_vtMinAcceleration = { 0, 0 };
	SGD::Vector m_vtMaxAcceleration = { 0, 0 };
	SGD::Vector m_vtMinGravity = { 0, 0 };
	SGD::Vector m_vtMaxGravity = { 0, 0 };
	SGD::Color m_cStartColor;
	SGD::Color m_cEndColor;
	SGD::Point m_ptPosition = { 0, 0 };
	SGD::Point m_EndPoint = { 0, 0 };
	SGD::Size m_szSize = { 0, 0 };
	SGD::Size m_szMaxParticleSize = { 0, 0 };
	SGD::Size m_szMinParticleSize = { 0, 0 };
	int m_iEmitterShape;
	int m_iSpinSpeed;
	int m_iRadius;
	bool m_bPinEdges;
	float m_fColorChange = 0.0f;
	float m_fRotation = 0.0f;
	float m_fMaxLifeSpan = 0.0f;
	float m_fMinLifeSpan = 0.0f;
	float m_fGravity = 0.0f;
	SGD::Size m_szScale = { 0, 0 };
	float m_fAlphaStart = 0.0f;
	float m_fAlphaFade = 0.0f;
	float m_fMinScaleRate = 0.0f;
	float m_fMaxScaleRate = 0.0f;
	unsigned int m_unMaxParticles = 10;
	bool m_bLooping = false;
	//Timers
	float m_fSpinTimer = 0.0f;

	std::vector<Particle> m_vecParticles;
	//CHecks
	bool m_bStarted = false;

};


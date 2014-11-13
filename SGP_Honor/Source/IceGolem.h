#pragma once
#include "Enemy.h"
#include "../SGD Wrappers/SGD_Listener.h"

class IceGolem :
	public Enemy, public SGD::Listener
{
public:
	IceGolem();
	~IceGolem();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	void BasicCollision(const IEntity* pOther) override;
	void RightRampCollision(const IEntity* pOther);
	void LeftRampCollision(const IEntity* pOther);

	////////////////////////////////////////////////
	////////////// Listener Interface //////////////
	void HandleEvent(const SGD::Event* pEvent);

	/////////////////////////////////////////////////
	/////////////////Mutators//////////////////////
	void SetSmashTimer(float _timer) { m_fSmashTimer = _timer; }
	void SetDirection(int _direction) { m_nDirection = _direction; }

private:

	float m_fSmashTimer = 1.65f;
	int m_nDirection = 0;
	float m_fRotation = 0.0f;
	float m_fAggroRange = 200.0f;
	bool m_bInRange = false;
	bool m_bPlayedAudio = false;
	float m_fDistance = 0.0f;

	SGD::HAudio m_hSmashSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hDeathSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hHitSound = SGD::INVALID_HANDLE;

};


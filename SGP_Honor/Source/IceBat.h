#pragma once
#include "Enemy.h"
#include "../SGD Wrappers/SGD_Listener.h"
class IceBat :
	public Enemy, public SGD::Listener
{
public:
	IceBat();
	virtual ~IceBat();

	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	void BasicCollision(const IEntity* pOther) override;
	void RightRampCollision(const IEntity* pOther);
	void LeftRampCollision(const IEntity* pOther);

	void SetDirection(int _direction) { m_nDirection = _direction; }


	////////////////////////////////////////////////
	////////////// Listener Interface //////////////
	void HandleEvent(const SGD::Event* pEvent);

private:



	int m_nDirection = 0;

	float m_fAggroRange = 200.0f;
	bool m_bInRange = false;
	bool m_bPlayedAudio = false;
	float m_fDistance = 0.0f;
	SGD::HAudio m_hDeathSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hHitSound = SGD::INVALID_HANDLE;
};


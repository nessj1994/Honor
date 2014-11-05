#pragma once
#include "Enemy.h"
class IceTurtle :
	public Enemy
{
public:
	IceTurtle();
	virtual ~IceTurtle();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;


	/////////////////////////////////////////////////
	/////////////////Mutators//////////////////////
	void SetFireTimer(float _timer) { m_fFireTimer = _timer; }
	void SetDirection(int _direction) { m_nDirection = _direction; }

private:

	float m_fFireTimer = 1.65f;
	int m_nDirection = 0;
	float m_fRotation = 0.0f;

	SGD::HAudio m_hShootSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hDeathSound = SGD::INVALID_HANDLE;
	SGD::HAudio m_hHitSound = SGD::INVALID_HANDLE;

};


#pragma once
#include "Unit.h"
class Turret :
	public Unit
{
public:
	Turret();
	~Turret();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;
	

	/////////////////////////////////////////////////
	/////////////////Mutators//////////////////////
	void SetFireTimer(float _timer) { m_fFireTimer = _timer; m_fShootTimer = _timer; }
	void SetShootDirection(int _dir) { m_nDirection = _dir; }

private:

	float m_fFireTimer = 0.0f;
	float m_fShootTimer = 0.0f;
	int m_nDirection = 0;
	float m_fRotation = 0.0f;

	SGD::HAudio m_hShootSound = SGD::INVALID_HANDLE;
};



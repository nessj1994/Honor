#pragma once

#include "Enemy.h"

#include "../SGD Wrappers/SGD_Listener.h"

class BullEnemy : public Enemy, SGD::Listener
{
public:

	enum BullState { BS_IDLE, BS_WALKING, BS_RUNNING, BS_DEATH };

	/////////////////////////////
	// Ctor/Dtor
	BullEnemy();
	~BullEnemy();

	/////////////////////////////
	// Main interface
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_BULL_ENEMY; }
	virtual void HandleCollision(const IEntity* pOther) override;
	void HandleEvent(const SGD::Event* pEvent);

	bool GetAttacking();

private:
	/////////////////////////////
	// Member fields
	float m_fTurnTimer = 0.0f;
	float m_fRunSpeed = 256.0f;
	float m_fChangeTimer = 0.0f;
	float m_fDeathTimer = 0.0f;
	bool m_bDying = false;
	bool m_bPlayAudio = false;
	BullState m_bsCurrState;

	SGD::HAudio m_hWalking = SGD::INVALID_HANDLE;
	SGD::HAudio m_hRunning = SGD::INVALID_HANDLE;
	SGD::HAudio m_hRoar1 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hRoar2 = SGD::INVALID_HANDLE;

	unsigned int m_unPrevFrame = 0;

};
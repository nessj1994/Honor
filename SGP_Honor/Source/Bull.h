#pragma once
#include "Boss.h"

#include "../SGD Wrappers/SGD_Listener.h"

// Forward declarations
class Emitter;

class Bull :
	public Boss, SGD::Listener
{
public:
	Bull();
	virtual ~Bull();

	enum BullState { BS_WALKING, BS_CHARGING, BS_RUNNING, BS_STOMPING, BS_RETURNING, BS_DEATH, BS_SLOWING, BS_STUNNED };

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual int GetType(void) const override { return ENT_BOSS_BULL; }
	void HandleCollision(const IEntity* pOther) override;

	////////////////////////////////////////////////
	////////////// Listener Interface //////////////
	void HandleEvent(const SGD::Event* pEvent);

	bool GetAttacking() const;
	void ResetBull();

private:
	////////////////////////////////////////////////
	////////////// Member fields //////////////
	bool m_bCanCharge = false;
	bool m_bWillChargeRight = true;
	bool m_bWillChargeLeft = true;
	bool m_bDead = false;
	bool m_bRenderFire = false;
	bool m_bAudioPlayed = false;
	bool m_bTouchingPlayer = false;

	float m_fStunSpeed = 0.0f;
	char m_fAlphaFade = 0;

	float m_fTurnTimer = 0.0f;
	float m_fChargeTimer = 0.0f;
	float m_fRunTimer = 0.0f;
	float m_fWalkTimer = 0.0f;
	float m_fSlowTimer = 0.0f;
	float m_fStunTimer = 0.0f;
	float m_fDeathTimer = 0.0f;

	BullState m_bsCurrState;

	Emitter * m_eFire1;
	Emitter * m_eFire2;

	SGD::HAudio m_hVictory = SGD::INVALID_HANDLE;
	SGD::HAudio m_hWalking = SGD::INVALID_HANDLE;
	SGD::HAudio m_hRunning = SGD::INVALID_HANDLE;
	SGD::HAudio m_hRoar1 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hRoar2 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hRoar3 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hWallHit = SGD::INVALID_HANDLE;
	SGD::HAudio m_hReadyCharge = SGD::INVALID_HANDLE;

	unsigned int m_unPrevFrame = 0;
	
};


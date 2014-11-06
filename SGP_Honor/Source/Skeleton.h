#pragma once

#include "Enemy.h"

#include "../SGD Wrappers/SGD_Listener.h"

class Skeleton : public Enemy, SGD::Listener
{
public:

	enum SkeletonState { SS_IDLE, SS_THROWING, SS_DEATH };

	/////////////////////////////
	// Ctor/Dtor
	Skeleton();
	~Skeleton();

	/////////////////////////////
	// Main interface
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_SKELETON; }
	virtual void HandleCollision(const IEntity* pOther) override;
	void HandleEvent(const SGD::Event* pEvent);

	//bool GetAttacking();

private:
	/////////////////////////////
	// Member fields
	//float m_fTurnTimer = 0.0f;
	//float m_fRunSpeed = 256.0f;
	//float m_fChangeTimer = 0.0f;
	//float m_fDeathTimer = 0.0f;
	//bool m_bDying = false;
	//BullState m_bsCurrState;
	float m_fShootTimer = 0.0f;
	SkeletonState m_ssCurrState;
	float m_fDeathTimer = 0.0f;
	bool m_bDying = false;
	bool m_bThrowing = false;
	bool m_bCanThrow = true;
	bool m_bPlayAudio = false;

	SGD::HAudio m_hThrow = SGD::INVALID_HANDLE;
	SGD::HAudio m_hDeath = SGD::INVALID_HANDLE;

	unsigned int m_unPrevFrame = 0;
};
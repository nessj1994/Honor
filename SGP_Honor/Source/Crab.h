#pragma once
#include "Boss.h"
#include "../SGD Wrappers/SGD_Listener.h"

class Crab : public Boss, SGD::Listener
{
public:
	Crab();
	virtual ~Crab();

	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual int GetType(void) const override { return ENT_BOSS_CRAB; }

	void BasicCollision(const IEntity* pOther) override;
	void HandleCollision(const IEntity* pOther) override;

	void HandleEvent(const SGD::Event* pEvent);

private:
	enum states{idle, slamming, swipping, bubbles, hurt};
	float leftSlamTimer = 0.0f;
	float rightSlamTimer = 0.0f;
	float bubbleTimer = 0.0f;
	float bubbleSpawn = 0.0f;
	SGD::Point m_ptLeftArm;
	SGD::Size m_szLeftArm;
	SGD::Point m_ptRightArm;
	SGD::Size m_szRightArm;
};


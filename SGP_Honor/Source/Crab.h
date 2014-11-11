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
	virtual SGD::Rectangle GetRect(void) const override;

private:
	enum states{idle, slamming, swipping, bubbles, hurt};
	float leftSlamCD = 0.0f;
	float rightSlamCD = 0.0f;
	float bubbleTimer = 0.0f;
	float bubbleCD = 0.0f;
	float bubbleSpawn = 0.0f;
	float leftSlamTimer = 0.0f;
	float rightSlamTimer = 0.0f;
	float swipeTimer = 0.0f;
	float swipeCD = 0.0f;
	float wasHitCD = 0.0f;
	float StartWaitTime = 0.0f;
	float deathTime = 0.0f;

	float roarTimer = 60.0f;

	bool castedBubbles = false;
	bool castedLeftSlam = false;
	bool castedRightSlam = false;
	bool LeftSlamOnCD = false;
	bool RightSlamOnCD = false;
	bool castedSwipe = false;
	bool SwipeOnCD = false;
	bool wasHit = false;

	unsigned char alpha = 255;

	// Sounds
	SGD::HAudio m_hRoar = SGD::INVALID_HANDLE;
	SGD::HAudio m_hBubble = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlam1 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlam2 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hSlam3 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hHurt = SGD::INVALID_HANDLE;
};


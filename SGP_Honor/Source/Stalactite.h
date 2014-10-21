#pragma once
#include "Entity.h"
class Stalactite :
	public Entity
{
public:
	Stalactite();
	virtual ~Stalactite();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	/////////////////////////////////////////////////
	/////////////////// Accessors //////////////////
	SGD::Point GetStartPosition(void) const { return m_ptStartPosition; }
	bool GetIsFalling(void) const { return m_bIsFalling; }
	float GetFallSpeed(void) const { return m_fFallSpeed; }
	float GetRespawnTimer(void) const { return m_fRespawnTimer; }

	///////////////////////////////////////////////
	///////////////// Mutators ///////////////////
	void SetStartPosition(SGD::Point startPos) { m_ptStartPosition = startPos; }
	void SetIsFalling(bool isFalling) { m_bIsFalling = isFalling; }
	void SetFallSpeed(float speed) { m_fFallSpeed = speed; }
	void SetRespawnTimer(float timer) { m_fRespawnTimer = timer; }


private:

	SGD::Point m_ptStartPosition;
	bool m_bIsFalling = true;
	float m_fRespawnTimer = 0.0f;
	float m_fFallSpeed = 300.0f;
};


#pragma once
#include "Entity.h"
#include "Font.h"
#include "BitmapFont.h"
#include <map>
#include "Game.h"
#include "Animation.h"
#include "AnimTimeStamp.h"

class Hawk;


class Unit :
	public Entity
{
public:
	Unit();
	virtual ~Unit();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)				override;
	virtual void Render(void)							override;

	virtual int GetType(void)							const override;
	virtual SGD::Rectangle GetRect(void)				const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	virtual void BasicCollision(const IEntity* pOther);

	//////////////////////////////////////////////
	/////////////////Accessors///////////////////
	std::string GetCurrentAnimation(void) const { return m_stCurrentAnimation; }
	bool IsFacingRight(void) const { return m_bFacingRight;}
	bool GetAlive(void) const { return m_bAlive; }
	float GetGravity(void) const { return m_fGravity; }

	bool IsDashing(void)	const { return m_bDashing; }
	bool IsIcing(void)		const { return m_bIcing; }
	bool IsHawking(void)	const { return m_bHawking; }
	bool IsBouncing(void)	const { return m_bBouncing; }
	bool GetFalling(void) const { return m_bFalling; }

	Hawk* GetHawkPtr(void) const { return m_pHawk; }



	////////////////////////////////////////////
	/////////////////Mutators//////////////////
	void SetCurrentAnimation(std::string animation) { m_stCurrentAnimation = animation; }
	void SetFacingRight(bool facingRight) { m_bFacingRight = facingRight; }
	void SetAlive(bool alive) { m_bAlive = alive; }
	void SetGravity(float _gravity) { m_fGravity = _gravity; }
	void SetIsDashing(	bool dashing) {m_bDashing = dashing;}
	void SetIsIcing(  	bool icing) {m_bIcing = icing;}
	void SetIsHawking(	bool hawking) {m_bHawking = hawking;}
	void SetIsBouncing(	bool bouncing) {m_bBouncing = bouncing;}
	void SetHawkPtr(Hawk* _hawk) { m_pHawk = _hawk; }
	void SetFalling(bool walker) { m_bFalling = walker; }


private:

	bool m_bAlive = true;
	bool m_bFalling = true;
	
	std::map<Animation, std::string> m_mAnimations;
	std::string m_stCurrentAnimation;
	
	bool m_bDashing = false;
	bool m_bIcing = false;
	bool m_bHawking = false;
	bool m_bBouncing = false;

	Hawk* m_pHawk = nullptr;


	float m_fGravity = -500.0f;
	float m_fDashTimer = 0.5f;
	float m_fHawkTimer = 0.0f;

protected:
	AnimTimeStamp m_ts;
	//Local refernce to the font
	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");
	bool m_bFacingRight = true;
};


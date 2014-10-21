#pragma once
#include "Entity.h"
#include <vector>

class MovingPlatform :
	public Entity
{
public:
	MovingPlatform();
	virtual ~MovingPlatform();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;


	/////////////////////////////////////////////////
	/////////////////Mutators//////////////////////
	void	SetVertical(bool _vertical)			{ m_bIsVertical = _vertical; }
	void	SetTurnDistance(float _distance)	{ m_fTurnDistance = _distance; }
	void	SetSpeed(float _speed)				{ m_fSpeed = _speed; }

private:
	/////////////////////////////////////////////////
	/////////////////Member fields//////////////////////
	bool		m_bIsVertical = false;
	float		m_fTurnDistance = 0.0f;
	float		m_fSpeed = 0.0f;
	float		m_fDistanceTraveled = 0.0f;
	bool		m_bReturning = false;
	std::vector<Entity*> m_vEntities;
};
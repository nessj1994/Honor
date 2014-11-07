#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class BuzzSaw :
	public Entity
{
public:
	BuzzSaw();
	virtual ~BuzzSaw();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	//////////////////////////////////////////////
	///////////////// Accessors /////////////////
	bool GetHorizontal(void) const { return m_bHorizontal; }
	bool GetMovingRight(void) const { return m_bHorizontal; }
	SGD::Point GetStartPosition(void) const { return m_ptStartPosition; }
	unsigned int GetDistance(void) const { return m_unDistance; }

	void SetHorizontal(bool horizontal){ m_bHorizontal = horizontal; }
	void SetMovingRight(bool movingRight){ m_bMovingRight = movingRight; }
	void SetStartPosition(SGD::Point startPos){ m_ptStartPosition = startPos; }
	void SetDistance(unsigned int distance) { m_unDistance = distance; }
private:

	bool m_bHorizontal = true;		//Isthe buzzsaw moving horizontal
	bool m_bMovingRight = false;	//Is the buzzsaw moving right or left
	SGD::Point m_ptStartPosition;	//Position to store for movement
	unsigned int m_unDistance = 0; //Pixels moved from the start postion on either side
	float rotation = 0.0f;
	SGD::HAudio m_hEffect = SGD::INVALID_HANDLE;
	bool m_bEffectPlay;

};


#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class Geyser :
	public Entity
{
public:
	Geyser();
	virtual ~Geyser();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	//SGD::Rectangle GetPillar(void) const { return m_rPillar; }

	void ChangePillar(int _height, float _delta);

	SGD::Point	GetOrigPosition(void) const { return m_ptOrigPos; }
	void		SetOrigPosition(SGD::Point pos)		{ m_ptOrigPos = pos; }


private:

	//SGD::Rectangle m_rPillar = { m_ptPosition, m_szSize };

	

	bool m_bSPRAYED = false;
	bool m_bRising = false;
	bool m_bApex = false;
	bool m_bFalling = false;
	bool m_bResting = false;

	float m_fApexTimer = 2.0f;
	float m_fSprayTimer = 4.0f;


	SGD::Point m_ptOrigPos = { 0, 0 };
	SGD::Size m_szOrigSize = { 0, 0 };


	SGD::HAudio m_hEffect = SGD::INVALID_HANDLE;

};


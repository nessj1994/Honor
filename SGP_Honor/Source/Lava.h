#pragma once
#include "Geyser.h"
class Lava :
	public Geyser
{
public:
	Lava();
	virtual ~Lava();

	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual int GetType(void) const {return ENT_LAVA;}


	void ChangePillar(int _height, float _delta);


	SGD::Point	GetOrigPosition(void) const { return m_ptOrigPos; }
	void		SetOrigPosition(SGD::Point pos)		{ m_ptOrigPos = pos; }


private:

	bool m_bRising = false;
	bool m_bApex = false;
	bool m_bFalling = false;
	bool m_bResting = false;

	float m_fApexTimer = 2.0f;

	

	SGD::Point m_ptOrigPos = { 0, 0 };
	SGD::Size m_szOrigSize = { 0, 0 };

};


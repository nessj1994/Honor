#pragma once
#include "Boss.h"

class Bull :
	public Boss
{
public:
	Bull();
	virtual ~Bull();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual int GetType(void) const override { return ENT_BOSS_BULL; }

};


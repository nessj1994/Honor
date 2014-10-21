#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class Block :
	public Entity
{
public:
	Block();
	virtual ~Block();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_BLOCK; }
	virtual void HandleCollision(const IEntity* pOther) override;


};


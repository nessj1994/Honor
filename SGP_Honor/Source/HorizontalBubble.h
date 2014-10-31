#pragma once
#include "Projectile.h"
class HorizontalBubble : public Projectile
{
public:
	HorizontalBubble();
	virtual ~HorizontalBubble();

	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_PROJ; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;
};


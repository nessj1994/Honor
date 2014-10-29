#pragma once
#include "Projectile.h"
class GravProjectile : public Projectile
{
public:
	GravProjectile();
	virtual ~GravProjectile();

	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	int GetType(void) const override { return ENT_PROJ; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

private:
	float gravity = 0.0f;
	float gravTimer = 0.0f;
};


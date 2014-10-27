#pragma once
#include "Entity.h"
class Honor :
	public Entity
{
public:
	Honor();
	virtual ~Honor();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return ENT_HONOR; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	bool GetIsCollected() const { return isCollected; }
	void SetHonorAmount(unsigned int amount) { m_HonorAmount = amount; }
	unsigned int GetHonorAmount() const { return m_HonorAmount; }

private:
	bool isCollected = false;
	unsigned int m_HonorAmount = 50;
};
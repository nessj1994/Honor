#pragma once
#include "Entity.h"
//#include "C:\Users\Conor\Desktop\SGP REPO SPRINT 2\honor2\SGP_Honor\Source\Entity.h"
class SwordSwing :
	public Entity
{
public:
	SwordSwing();
	virtual ~SwordSwing();

	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;
	virtual SGD::Rectangle GetRect(void)const { return SwingRect; }
	virtual void HandleCollision(const IEntity* pOther) override;
	virtual int GetType(void) const { return ENT_SWORD; }

	void SetRect(SGD::Rectangle _rect) { SwingRect = _rect; }


private:

	SGD::Rectangle SwingRect = { 0, 0, 0, 0 };

};


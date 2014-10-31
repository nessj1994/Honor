#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Geometry.h"

class Player;

class Activator :
	public Entity
{
public:
	Activator(bool isPressure);
	virtual ~Activator();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override;
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	/////////////////////////////////////////////////
	/////////////////// Accessors //////////////////
	bool isOn(void) const { return m_bIsOn; }
	bool isPressure(void) const { return m_bPressurePlate; }
	unsigned int GetKeyID(void) const { return m_unKeyID; }

	///////////////////////////////////////////////
	/////////////////// Mutators /////////////////
	void SetOn(bool isOn) { m_bIsOn = isOn; }
	void SetKeyID(unsigned int ID) { m_unKeyID = ID; }
	void SetPlayer(Player* ptr) { m_pPlayer = ptr; }

private:

	Player* m_pPlayer = nullptr;

	bool m_bPressurePlate = false;
	bool m_bIsOn = false;

	float m_fSwitchTimer = 0.0f;
	unsigned int m_unKeyID = 0;

};




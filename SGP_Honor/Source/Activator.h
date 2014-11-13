#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"

class Player;

class Activator :
	public Entity, SGD::Listener
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

	////////////////////////////////////////////////
	////////////// Listener Interface //////////////
	void HandleEvent(const SGD::Event* pEvent);

	/////////////////////////////////////////////////
	/////////////////// Accessors //////////////////
	bool isOn(void) const { return m_bIsOn; }
	bool isPressure(void) const { return m_bPressurePlate; }
	unsigned int GetKeyID(void) const { return m_unKeyID; }

	///////////////////////////////////////////////
	/////////////////// Mutators /////////////////
	void SetOn(bool isOn) { m_bIsOn = isOn; }
	void SetStartOn(bool _on) { m_bStartOn = _on; }
	void SetKeyID(unsigned int ID) { m_unKeyID = ID; }
	void SetPlayer(Player* ptr) { m_pPlayer = ptr; }

private:

	Player* m_pPlayer = nullptr;

	bool m_bPressurePlate = false;
	bool m_bIsOn = false;
	bool m_bStartOn = false;

	float m_fSwitchTimer = 0.0f;
	unsigned int m_unKeyID = 0;

	SGD::HAudio m_hEffect = SGD::INVALID_HANDLE;
	SGD::HTexture m_hActivator = SGD::INVALID_HANDLE;

};




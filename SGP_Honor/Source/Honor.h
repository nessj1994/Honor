#pragma once
#include "Entity.h"

#include "../SGD Wrappers/SGD_Listener.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

// Forward declarations
class Emitter;

class Honor :
	public Entity, SGD::Listener
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
	void HandleEvent(const SGD::Event* pEvent);

	void SetEmitter();
	void PlayAudio();

	/////////////////////////////////////////////////
	/////////////////Accessors//////////////////////

	void SetHonorAmount(unsigned int amount) { m_HonorAmount = amount; }
	void SetVectorID(unsigned int _ID) { m_unVectorID = _ID; }
	void SetIsCollected(bool _collected) { m_bIsCollected = _collected; }
	void SetStartedCollected(bool _collected) { m_bStartedCollected = _collected; }

	/////////////////////////////////////////////////
	/////////////////Mutators//////////////////////

	unsigned int GetHonorAmount() const { return m_HonorAmount; }
	bool GetIsCollected() const { return m_bIsCollected; }
	unsigned int GetVectorID() const { return m_unVectorID; }

private:
	bool m_bIsCollected = false;
	bool m_bStartedCollected = false;
	unsigned int m_HonorAmount = 50;
	unsigned int m_unVectorID = 0;
	Emitter * m_eEffect;

	// Sounds
	SGD::HAudio m_hPiano1 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hPiano2 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hPiano3 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hPiano4 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hPiano5 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hPiano6 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hPiano7 = SGD::INVALID_HANDLE;

};
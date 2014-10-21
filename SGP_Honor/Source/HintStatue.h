#pragma once
#include "Entity.h"
#include <string>

class HintStatue :
	public Entity
{
public:
	HintStatue();
	virtual ~HintStatue();

	enum messageSize { MS_SMALL, MS_MEDIUM, MS_LARGE };
	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime)  override;
	virtual void Render(void) override;

	virtual int GetType(void) const override { return ENT_STATUE; }
	virtual SGD::Rectangle GetRect(void) const override;
	virtual void HandleCollision(const IEntity* pOther) override;

	void SetMessageSize(unsigned int s) { sizeOfMessage = s; }
	void SetMessageString(std::string str) { st_message = str; }

private:
	SGD::HTexture		m_hSBImage = SGD::INVALID_HANDLE;
	unsigned int sizeOfMessage;
	bool showMessage = false;
	std::string st_message;
};
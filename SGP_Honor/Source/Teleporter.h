#pragma once
#include "Entity.h"
#include <string>

class Teleporter :
	public Entity
{
public:
	Teleporter();
	virtual ~Teleporter();


	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Render(void) override;
	int GetType(void) const override { return ENT_TELEPORTER; }
	virtual void HandleCollision(const IEntity* pOther) override;

	/////////////////////////////////////////////////
	/////////////////Accessors//////////////////////
	std::string GetLevel(void) const { return m_sLevel; }
	bool GetHubTeleporter(void) const { return m_bHubTeleporter; }

	/////////////////////////////////////////////////
	/////////////////Mutators//////////////////////
	void SetLevel(std::string _level) { m_sLevel = _level; }
	void SetHubTeleporter(bool _hub) { m_bHubTeleporter = _hub; }

private:
	std::string m_sLevel;
	bool m_bHubTeleporter = false;

	SGD::HAudio m_hDoorClosed = SGD::INVALID_HANDLE;

};


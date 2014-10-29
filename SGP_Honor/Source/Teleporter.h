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

	/////////////////////////////////////////////////
	/////////////////Mutators//////////////////////
	void SetLevel(std::string _level) { m_sLevel = _level; }

private:
	std::string m_sLevel;

};


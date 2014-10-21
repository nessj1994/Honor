#pragma once

#include "../SGD Wrappers/SGD_Geometry.h" //Included for rectangles
class IEntity
{
protected:
	IEntity() = default;
	virtual ~IEntity() = default;

public:

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime) = 0;
	virtual void Render(void) = 0;

	virtual int GetType(void) const = 0;
	virtual SGD::Rectangle GetRect(void) const = 0;
	virtual void HandleCollision(const IEntity* pOther) = 0;
	
	
	///////////////////////////////////////////////
	//////////////Reference Counting//////////////
	virtual void AddRef(void) = 0;
	virtual void Release(void) = 0;

};


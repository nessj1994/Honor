#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"
#include <string>
class Frame
{
public:
	Frame();
	~Frame();

	/////////////////////////////////////////////////////////
	///////////////////////Accessors////////////////////////
	SGD::Rectangle GetSourceRect(void) const { return m_rSource; }
	SGD::Rectangle GetCollisionRect(void) const { return m_rCollision; }
	SGD::Rectangle GetAttackRect(void) const { return m_rAttack; }
	SGD::Point GetEventPt(void) const { return m_ptEvent; }
	SGD::Point GetAnchor(void) const { return m_ptAnchor; }
	float GetDuration(void) const { return m_fDuration; }
	std::string GetEvent(void) const { return m_stEvent; }

	/////////////////////////////////////////////////////////
	///////////////////////Mutators/////////////////////////
	void SetSourceRect(SGD::Rectangle rect) { m_rSource = rect; }
	void SetCollisionRect(SGD::Rectangle rect) { m_rCollision = rect; }
	void SetAttackRect(SGD::Rectangle rect) { m_rAttack = rect; }
	void SetEventPt(SGD::Point pt) { m_ptEvent = pt; }
	void SetAnchor(SGD::Point anchor) { m_ptAnchor = anchor; }
	void SetDuration(float dur) { m_fDuration = dur; }
	void SetEvent(std::string eventName) { m_stEvent = eventName; }

private:

	SGD::Rectangle m_rSource;
	SGD::Rectangle m_rCollision;
	SGD::Rectangle m_rAttack;
	SGD::Point m_ptEvent;
	SGD::Point m_ptAnchor;
	float m_fDuration;
	std::string m_stEvent;



};


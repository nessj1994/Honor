#pragma once

#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include <vector>
#include "Frame.h"
#include "AnimTimeStamp.h"

class Animation
{
public:
	Animation();
	~Animation();

	/////////////////////////////////////////////////
	/////////////////Interface//////////////////////
	virtual void Update(float elapsedTime, AnimTimeStamp& ts);
	virtual void Render(SGD::Point position, float rotation);
	virtual void Reset();


	////////////////////////////////////////////////
	/////////////////Accessors/////////////////////
	float GetSpeed(void) const { return m_fSpeed; }
	bool IsLooping(void) const { return m_bLooping; }
	SGD::HTexture GetImage() const { return m_hImage; }

	///////////////////////////////////////////////
	//////////////////Mutators////////////////////
	void SetSpeed(float speed) { m_fSpeed = speed; }
	void SetLooping(bool Looping) { m_bLooping = Looping; }
	void SetImage(SGD::HTexture img) { m_hImage = img; }
	void AddFrame(SGD::Rectangle source, SGD::Rectangle collision, SGD::Rectangle attack,
				  SGD::Point ptEv, SGD::Point anchor, float dur, std::string stEv);

	void UnloadTexture();


	std::vector<Frame> GetFrames() const { return m_vFrames; }


private:
	SGD::HTexture m_hImage = SGD::INVALID_HANDLE;

	//Animation(const Animation&) = delete;
	//Animation& operator=(const Animation&) = delete;

	std::vector<Frame> m_vFrames;

	float m_fSpeed = 0.0f;
	bool m_bLooping = false;
};


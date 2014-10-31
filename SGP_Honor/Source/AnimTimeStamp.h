#pragma once
#include <string>

class AnimTimeStamp
{
public:
	AnimTimeStamp();
	~AnimTimeStamp();

	std::string GetCurrAnimation() const { return currAnimation; }
	unsigned int GetCurrFrame() const { return m_unCurrentFrame; }
	float GetTimeWaited() const { return m_fTimeWaited; }
	float GetSpeed(void) const { return m_fSpeed; }
	bool IsPlaying() const { return playing; }

	void SetCurrAnimation(std::string str) { currAnimation = str; }
	void IncreaseCurrFrame() { m_unCurrentFrame++; }
	void DecreaseCurrFrame() { m_unCurrentFrame--; }
	void ResetCurrFrame() { m_unCurrentFrame = 0; }
	void SetTimeWaited(float time) { m_fTimeWaited = time; }
	void SetPlaying(bool p) { playing = p; }
	void SetSpeed(float speed) { m_fSpeed = speed; }

private:
	float m_fSpeed = 1.0f;
	std::string currAnimation;
	unsigned int m_unCurrentFrame = 0;
	float m_fTimeWaited = 0.0f;
	bool playing = false;
};


#pragma once

#include "Animation.h"
#include <map>

class AnimationEngine
{
public:

	//////////////////////////////////////////////////////////////////////////
	////////////////////////Singleton Accessors//////////////////////////////
	static AnimationEngine* GetInstance(void);
	static void DeleteInstance(void);

	/////////////////////////////////////////////////////////////////////////
	/////////////////////////Setup, Play, Cleanup///////////////////////////
	bool Initialize(void);
	void Update(float elapsedTime, AnimTimeStamp& ts);
	void Render(SGD::Point position, float rotation, AnimTimeStamp& ts);
	void Terminate(void);

	Animation GetAnimation(std::string anim) { return m_mAnimationMap[anim]; }


	/////////////////////////////////////////////////////////////////////////
	///////////////////////////////Interface////////////////////////////////
	void LoadAnimation(std::string filePath);


	////////////////////////////////////////////////////////////////////////
	////////////////////////////Accessors//////////////////////////////////
	//Animation& GetEmitter(std::string AnimationName) const { return new Animation(); }

private:
	static AnimationEngine* s_pInstance;

	AnimationEngine() = default;
	~AnimationEngine() = default;

	AnimationEngine(const AnimationEngine&) = delete;
	AnimationEngine& operator= (const AnimationEngine&) = delete;

	std::map<std::string, Animation> m_mAnimationMap;

};


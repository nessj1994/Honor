#pragma once

#include "Animation.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <map>

class IEntity;

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
	void Update(float elapsedTime, AnimTimeStamp& ts, IEntity* sender);
	void Render(SGD::Point position, float rotation, AnimTimeStamp& ts, bool flipped, float scale, SGD::Point camerapos, SGD::Vector rotationOffset, SGD::Color color);
	void Terminate(void);

	Animation GetAnimation(std::string anim) { return m_mAnimationMap[anim]; }
	SGD::Rectangle GetRect(const AnimTimeStamp& ts, bool facingRight, float scale, SGD::Point position);
	SGD::Rectangle GetAttackRect(const AnimTimeStamp& ts, bool facingRight, float scale, SGD::Point position);

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


#pragma once
#include "IGameState.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Handle.h"
#include <string>

// Forward declarations
class CutScene;

class CutSceneState :
	public IGameState
{
public:

	///////////////////////////////////////////////////////////
	///////////////////// Singleton Accessor /////////////////
	static CutSceneState* GetInstance(void);

	//////////////////////////////////////////////////////////////
	//////////////////// IGameState Interface ///////////////////
	virtual void Enter(void)			 override; //Load Resources
	virtual void Exit(void)				 override; //Unload Resources

	virtual bool Input(void)			 override; //Hanlde user Input
	virtual void Update(float elapsedTime)			 override; //update entities
	virtual void Render(void)			 override; //Render all entities

	//////////////////////////////////////////////////////////////
	//////////////////// Accessors ///////////////////
	//unsigned int GetNextState() const { return m_unNextState; }

	//////////////////////////////////////////////////////////////
	//////////////////// Mutators ///////////////////
	//void SetNextState(unsigned int _state) { m_unNextState = _state; }
	void SetCutScenePath(std::string _path) { m_strCutScenePath = _path; }

private:

	CutSceneState() = default;
	virtual ~CutSceneState() = default;

	CutSceneState(CutSceneState&) = delete;
	CutSceneState& operator= (CutSceneState&) = delete;


	//////////////////////////////////////////////////////////
	///////////////////// Cursor Index //////////////////////
	//unsigned int m_unNextState = 0; // 0 for gameplay, 1 for credits
	CutScene * m_pCutScene = nullptr;
	std::string m_strCutScenePath = "";

};


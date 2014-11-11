#include "CutSceneState.h"
#include "CutScene.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "GameplayState.h"
#include "CreditsState.h"

///////////////////////////////////////////////////////////
/////////////////// Singleton Accessor ///////////////////

/////////////////////////////////////////////////////////
// GetInstance
// - allocate the static global instance
// - return THE instance
CutSceneState* CutSceneState::GetInstance(void)
{
	static CutSceneState s_Instance; //This is stored ONCE in global memory
	return &s_Instance; //Return the one global reference
}

///////////////////////////////////////////////////
// Enter
void CutSceneState::Enter()
{
	// Create the cutscene
	m_pCutScene = new CutScene();
	m_pCutScene->LoadCutScene(m_strCutScenePath);
}

///////////////////////////////////////////////////
// Exit
void CutSceneState::Exit()
{
	delete m_pCutScene;
}

///////////////////////////////////////////////////
// Input
bool CutSceneState::Input()
{
	return true;
}

///////////////////////////////////////////////////
// Update
void CutSceneState::Update(float elapsedTime)
{
	// Update the cutscene
	m_pCutScene->Update(elapsedTime);

	// If the cutscene is finished, go to the next state
	if (m_pCutScene->GetFinished())
	{
		Game::GetInstance()->RemoveState();
	}
}

///////////////////////////////////////////////////
// Render
void CutSceneState::Render()
{
	m_pCutScene->Render();
}
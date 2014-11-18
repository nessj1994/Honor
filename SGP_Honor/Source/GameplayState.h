/*
|	File: GameplayState.h
|	Author: Jordan Ness
|	Purpose: The GameplayState class handles all of the 
			games entities and gets them running
|
*/

#pragma once
#include "IGameState.h"
#include "Camera.h"
#include "Animation.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "AnimTimeStamp.h"
#include <map>



////////////////////////////////////////////////////
//////////// Foward Class Declarations ////////////
class EntityManager;
class Entity;
class Level;
class Player;
class FallingBlock;
class Door;
class BossDoor;
class Activator;
class Unit;
class Hawk;
class Stalactite;
class BuzzSaw;
class Turret;
class Pendulum;
class HintStatue;
class Honor;
class Armor;
class Squid;
class Pouncer;
class Bull;
class Jellyfish;
class Crab;
class HubWorldOrb;
class Emitter;

//
//////////////////////////////////////////////////
// GameplayState class
// -runs game logic
// -SINGLETON (static allocation)
class GameplayState :
	public IGameState
{

public:

	///////////////////////////////////////////////////////////
	///////////////////// Singleton Accessor /////////////////
	static GameplayState* GetInstance(void);
	



	//////////////////////////////////////////////////////////////
	//////////////////// IGameState Interface ///////////////////
	virtual void Enter(void)						override; //Load Resources
	virtual void Exit(void)							override; //Unload Resources

	virtual bool Input(void)						override; //Handle user Input
	virtual void Update(float elapsedTime)			override; //update entities
	virtual void Render(void)						override; //Render all entities


	///////////////////////////////////////////////////////////
	/////////////////////// Accessors ////////////////////////
	Camera* GetCamera(void) const { return m_pCamera; }
	Level* GetCurrentLevel(void) const { return m_pLevel; }
	std::string GetLevelString(void) const { return m_strCurrLevel; }
	float GetBorderSize(void) const { return m_fBorderSize; }
	float GetMiniMapWidth(void) const { return m_fMiniMapWidth; }
	float GetMiniMapHeight(void) const { return m_fMiniMapHeight; }
	bool GetIsEnding() const { return ending; }

	//////////////////////////////////////////////////////////
	////////////////////// Mutators /////////////////////////
	void SetCamera(Camera* camera) { m_pCamera = camera; }
	void ResetAudio() { SGD::AudioManager::GetInstance()->PlayAudio(m_hHub); }
	void SetScreenFadeout(unsigned char _fade) { m_cScreenFade = _fade; }

	///////////////////////////////////////////////////////////
	///////////////////// Factory Methods  ////////////////////
	void CreateHonor(int x, int y, int _amount, unsigned int _index);
	void CreateActivator(int x, int y, bool _pressurePlate, bool _active, int _ID);
	void CreateLaser(int x, int y, SGD::Vector _direction, int _ID, bool _on);
	void CreateTurret(int x, int y, int _direction, float _timer);
	void CreateDoor(int _x, int _y, bool _isHorizontal, int _ID, bool _startOpen);
	void CreateMovingPlatform(int _x, int _y, bool _vertical, float _turnDistance, float _speed);
	void CreateGeyser(int _x, int _y, float _speed, float _maxHeight, int _currState);
	void CreateLava(int _x, int _y, float _speed, float _maxHeight, int _currState);
	void CreateFallingBlock(int _x, int _y);
	void CreateBlock(int _x, int _y);
	void CreatePendulum(int _x, int _y);
	void CreateBuzzsaw(int _x, int _y, bool _horizontal, unsigned int _returnDistance);
	void CreateStalactite(int _x, int _y, float _timer);
	void CreateArmor(int _x, int _y);
	void CreateFreezableGround(int _x, int _y);
	void CreateFreezableLeftRamp(int _x, int _y);
	void CreateFreezableRightRamp(int _x, int _y);
	void CreateHintStatue(int _x, int _y, std::string _message);
	void CreateTeleporter(int _x, int _y, std::string _level, bool _hub);
	void CreateEnemy(int _x, int _y, int _type);
	void CreateBoss(int _x, int _y, int _type);
	void CreateBossTeleporter(int _x, int _y, std::string _level, unsigned int _honor);

	void LoadLevel(std::string _level);
	void SetHonorVector(std::vector<bool> _value);
	void SaveHonorVector();
	void LoadHonorVector();
	bool GetHonorValue(unsigned int _index);
	unsigned int GetHonorVectorSize();
	bool GetLevelUnlocked(std::string _level);
	void UnlockLevel(std::string _level);
	void ResetHonorInRoom();
	void IncreaseHonorBeforeDeath(unsigned int _value);

	void WizardDefeated();
	void StartMusic();

private:

	////////////////////////////////////////////////////////////
	///////////////////// Singleton ///////////////////////////
	GameplayState()										= default; //Default constructor
	virtual ~GameplayState()							= default; //Default destructor

	GameplayState(const GameplayState&)					= delete; //Delete copy constructor
	GameplayState& operator= (const GameplayState&)		= delete; //Delete assignment operator


	///////////////////////////////////////////////////////////
	///////////////////// Factory Methods  ////////////////////


	Player* CreatePlayer(void);
	Entity* CreateProjectile(Entity* pOwner) const;
	Entity* CreateGravProjectile(Entity* pOwner) const;
	Entity* GameplayState::CreateHorizBubble(Entity* pOwner) const;
	Entity* GameplayState::CreateVertBubble(Entity* pOwner) const;
	Entity* CreateSpray(Entity* pOwner) const;
	Hawk* CreateHawk(Entity* pOwner) const;


	void CreateBlocks(void);
	void CreatePermFrozenTiles(void);
	void CreateTempFrozenTiles(void);
	void RenderMiniMap(void);
	

	///////////////////////////////////////////////////////////
	//////////////////// Save and Load ///////////////////////
	void SaveGame();
	bool LoadGame();
	void LoadLevelMap();

	
	//////////////////////////////////////////////////////////
	/////////////// Message Callback Funtion ////////////////
	static void MessageProc(const SGD::Message* pMsg);


	//////////////////////////////////////////////////////////
	/////////////////// Game Entities ///////////////////////

	EntityManager* m_pEntities						= nullptr;
	Camera* m_pCamera								= nullptr;
	/*Emitter* m_pEmitter								= nullptr;
	Emitter* m_pEmitter2						    = nullptr;*/
	Level* m_pLevel									= nullptr;
	Player* m_pPlayer								= nullptr;

	std::string m_strCurrLevel						= "";

	std::map<std::string, std::string> m_mLevels;
	std::map<std::string, std::vector<bool>> m_mCollectedHonor;
	std::map<std::string, bool> m_mUnlockedLevels;

	AnimTimeStamp ts;
	
	//Test Entities to be removed later
	//FallingBlock* m_pFBlock = nullptr;
	//Door* m_pDoor = nullptr;
	//BossDoor* m_pBDoor = nullptr;
	//Activator* m_pSwitch = nullptr;
	//Activator* m_pPressurePlate = nullptr;
	//Stalactite* m_pStalactite = nullptr;
	//BuzzSaw* m_pBuzzSaw = nullptr;
	//Turret* m_pTurret = nullptr;

	//Pendulum * m_pPendulum = nullptr;
	//HintStatue * m_pStatue = nullptr;
	//Honor * m_pHonor = nullptr;
	//Armor * m_pArmor = nullptr;

	/*Squid* m_pSquid = nullptr;
	Pouncer* m_pPouncer = nullptr;
	Jellyfish* m_pJellyfish = nullptr;
	Jellyfish* m_pJellyfish2 = nullptr;*/

	SGD::HAudio m_hHub = SGD::INVALID_HANDLE;
	SGD::HAudio m_hWorld1 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hWorld2 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hWorld3 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hWorld4 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hWorld5 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hFinalBoss = SGD::INVALID_HANDLE;


	unsigned char m_cScreenFade = 0;
	bool m_bRenderMiniMap = false;
	float m_fBorderSize = 160.0f;
	float m_fMiniMapWidth = 0.0f;
	float m_fMiniMapHeight = 0.0f;

	bool ending = false;
	float endingTimer = 0.0f;
	unsigned char endFade = 0;
	float endFadeTimer = 0.0f;

	//Hub World Orb
	HubWorldOrb* m_pHubOrb;

	//Texture
	SGD::HTexture m_hXJUMP = SGD::INVALID_HANDLE;
	SGD::HTexture m_hOAttack = SGD::INVALID_HANDLE;
	SGD::HTexture m_hXWallJump = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTriOpenDoor = SGD::INVALID_HANDLE;
	SGD::HTexture m_hXJUMPKey = SGD::INVALID_HANDLE;
	SGD::HTexture m_hOAttackKey = SGD::INVALID_HANDLE;
	SGD::HTexture m_hXWallJumpKey = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTriOpenDoorKey = SGD::INVALID_HANDLE;
	SGD::HTexture m_hDashKey = SGD::INVALID_HANDLE;
	SGD::HTexture m_hDashCont = SGD::INVALID_HANDLE;
	SGD::HTexture m_hBubbleKey = SGD::INVALID_HANDLE;
	SGD::HTexture m_hBubbleCont = SGD::INVALID_HANDLE; 
	SGD::HTexture m_hSprayKey = SGD::INVALID_HANDLE;
	SGD::HTexture m_hSprayCont = SGD::INVALID_HANDLE;
	SGD::HTexture m_hHawkKey = SGD::INVALID_HANDLE;
	SGD::HTexture m_hHawkCont = SGD::INVALID_HANDLE;
	//FPS Count
	bool			m_bShowFPS;
	unsigned int	m_unFPS = 60;
	unsigned int	m_unFrames = 0;
	float			m_fFPSTimer;
};


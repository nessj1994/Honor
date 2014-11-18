#include "GameplayState.h"

#include <ostream>
#include <ShlObj.h>

#include "Game.h"
#include "MainMenuState.h"
#include "Emitter.h"
#include "Level.h"
#include "AnimationEngine.h"
#include "MessageID.h"
#include "DestroyEntityMessage.h"
#include "CreateProjectileMessage.h"
#include "CreateGravProjectileMessage.h"
#include "CreateHorizontalBubble.h"
#include "CreateVerticalBubble.h"
#include "CreateSprayMessage.h"
#include "CreateHawkMessage.h"
#include "CreateVomitMessage.h"
#include "CreatePoopMessage.h"
#include "ChangeLevelMessage.h"
#include "CreateStalactite.h"
#include "MovingPlatform.h"
#include "IceBat.h"
#include "CutSceneState.h"

#include <ctime>
#include "HubWorldOrb.h"

#include "Entity.h"
#include "Projectile.h"
#include "GravProjectile.h"
#include "HorizontalBubble.h"
#include "VerticalBubble.h"
#include "Player.h"
#include "EntityManager.h"
#include "Camera.h"
#include "ParticleEngine.h"
#include "Block.h"
#include "FreezeableGround.h"
#include "PauseState.h"
#include "FallingBlock.h"
#include "Door.h"
#include "BossDoor.h"
#include "Activator.h"
#include "Stalactite.h"
#include "Ice.h"
#include "Hawk.h"
#include "BuzzSaw.h"
#include "Turret.h"
#include "Stalactite.h"
#include "Geyser.h"
#include "laser.h"
#include "Lava.h"
#include "Pendulum.h"
#include "Armor.h"
#include "Honor.h"
#include "HintStatue.h"
#include "Squid.h"
#include "Pouncer.h"
#include "Jellyfish.h"
#include "MutantBat.h"
#include "Teleporter.h"
#include "BullEnemy.h"
#include "Bull.h"
#include "MutantMan.h"
#include "Crab.h"
#include "SwordSwing.h"
#include "Wizard.h"
#include "WizardDash.h"
#include "WizardHawk.h"
#include "Yeti.h"
#include "Skeleton.h"
#include "Vomit.h"
#include "Poop.h"
#include "Caveman.h"
#include "IceTurtle.h"
#include "IceGolem.h"
#include "CreditsState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../TinyXML/tinyxml.h"


#include <cassert>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

///////////////////////////////////////////////////////////
///////////////////// Singleton Accessor /////////////////

/////////////////////////////////////////////////////////
// GetInstance
// - allocate the static global instance
// - return THE instance
GameplayState* GameplayState::GetInstance(void)
{
	static GameplayState s_Instance; //This is stored ONCE in global memory
	return &s_Instance;
}




//////////////////////////////////////////////////////////////
//////////////////// IGameState Interface ///////////////////



///////////////////////////////////////////////////
// Enter
// - Reset the game
// - Load the resources
// - set up entities
void GameplayState::Enter(void) //Load Resources
{
	// Calculate mini map dimensions
	m_fMiniMapWidth = Game::GetInstance()->GetScreenWidth() - (m_fBorderSize * 2);
	m_fMiniMapHeight = Game::GetInstance()->GetScreenHeight() - (m_fBorderSize * 2);

	//Create Local references to the SGD Wrappers
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	//Set the background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 0, 0, 0 }); //Black

	//Allocate the Entity Manager
	m_pEntities = new EntityManager;

	//Initialize the Event and Message Managers
	SGD::EventManager::GetInstance()->Initialize();
	SGD::MessageManager::GetInstance()->Initialize(&MessageProc);


	//Load Audio
	m_hWorld1 = pAudio->LoadAudio(L"Assets/Audio/spanish.xwm");
	m_hWorld2 = pAudio->LoadAudio(L"Assets/Audio/cave.xwm");
	m_hWorld3 = pAudio->LoadAudio(L"Assets/Audio/mountain.xwm");
	m_hWorld4 = pAudio->LoadAudio(L"Assets/Audio/Beach.xwm");
	m_hWorld5 = pAudio->LoadAudio(L"Assets/Audio/World5.xwm");
	m_hFinalBoss = pAudio->LoadAudio(L"Assets/Audio/FinalBoss.xwm");



	//Create player with factory method
	m_pPlayer = CreatePlayer();

	Camera::GetInstance()->SetPlayer(m_pPlayer);

	m_pEntities->AddEntity(m_pPlayer, Entity::ENT_PLAYER);

	// Load in map for the levels and start the first level
	LoadLevelMap();
	bool oldGame = LoadGame();

	//LoadLevel("Level4_1");
	m_pPlayer->SetHasBounce(true);
	m_pPlayer->SetHasDash(true);
	m_pPlayer->SetHasHawk(true);
	m_pPlayer->SetHasIce(true);

	//LoadLevel("HubLevel");

	// Special case for tutorial
	if (Game::GetInstance()->GetSelectedNumber() == 4)
	{
		LoadLevel("Level0_2");
	}
	else
	{
		if (oldGame)
		{
			LoadLevel("HubLevel");
		}
		else
		{
			LoadLevel("Level0_1");
			CutSceneState::GetInstance()->SetCutScenePath("Assets/CutScenes/Intro.xml");
			Game::GetInstance()->AddState(CutSceneState::GetInstance());
		}
	}
	
	//LoadLevel("Level2_2");

	LoadLevel("Level5_2");

	// LoadLevel("HubLevel");

	// ("HubLevel");


	m_pHubOrb = new HubWorldOrb();
	//Turorial Images
	m_hOAttack = pGraphics->LoadTexture("Assets/graphics/HonorO.png");
	m_hXJUMP = pGraphics->LoadTexture("Assets/graphics/HonorX.png");
	m_hXWallJump = pGraphics->LoadTexture("Assets/graphics/HonorWall.png");
	m_hTriOpenDoor = pGraphics->LoadTexture("Assets/graphics/HonorTriangle.png");

}


/////////////////////////////////////////////////
// Exit
// - Deallocate all entities
// - unload all resources
void GameplayState::Exit(void)
{

	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hOAttack);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hXJUMP);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hXWallJump);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hTriOpenDoor);

	// Save collected honor
	m_pLevel->Exit();
	//SaveHonorVector();

	//Save the game
	SaveGame();

	if (m_pEntities != nullptr)
	{
		//Deallocate the Entity Manager
		m_pEntities->RemoveAll();
		delete m_pEntities;
		m_pEntities = nullptr;
	}

	//delete m_pBDoor;
	//delete m_pDoor;
	//delete m_pSwitch;
	//delete m_pPressurePlate;
	//delete m_pBuzzSaw;
	//delete m_pTurret;
	//delete m_pStalactite;

	if (m_pPlayer != nullptr)
	{
		m_pPlayer->Release();

	}

	if (m_pHubOrb != nullptr)
	{
		delete m_pHubOrb;
	}
	//if (m_pStatue != nullptr)
	//	m_pStatue->Release();

	//if (m_pHonor != nullptr)
	//	m_pHonor->Release();

	//if (m_pArmor != nullptr)
	//	m_pArmor->Release();

	//if (m_pPendulum != nullptr)
	//	m_pPendulum->Release();

	//delete m_pSquid;

	//
	//delete m_pPouncer;

	//delete m_pJellyfish;

	//delete m_pJellyfish2;
	//Create local references to the SGD Wrappers

	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();
	ParticleEngine::GetInstance()->Terminate();
	ParticleEngine::GetInstance()->DeleteInstance();



	//Unload Assets
	//Level
	delete m_pLevel;
	m_pLevel = nullptr;


	AnimationEngine::GetInstance()->Terminate();
	AnimationEngine::GetInstance()->DeleteInstance();

	//Graphics


	//Audio
	pAudio->StopAudio(m_hWorld1);
	pAudio->UnloadAudio(m_hWorld1);
	pAudio->StopAudio(m_hWorld2);
	pAudio->UnloadAudio(m_hWorld2);
	pAudio->StopAudio(m_hWorld3);
	pAudio->UnloadAudio(m_hWorld3);
	pAudio->StopAudio(m_hWorld4);
	pAudio->UnloadAudio(m_hWorld4);
	pAudio->StopAudio(m_hWorld5);
	pAudio->UnloadAudio(m_hWorld5);
	pAudio->StopAudio(m_hFinalBoss);
	pAudio->UnloadAudio(m_hFinalBoss);

	//Terminate and deallocate the SGD Wrappers
	//event manager
	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::GetInstance()->DeleteInstance();

	//message manager
	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::GetInstance()->DeleteInstance();
}


/////////////////////////////////////////////////
// Input
// - Handle the user's input
bool GameplayState::Input(void) //Hanlde user Input
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	//Change between windowed and full screen modes
	if (pInput->IsKeyDown(SGD::Key::Alt) && pInput->IsKeyReleased(SGD::Key::Enter))
	{
		SGD::GraphicsManager::GetInstance()->Resize({ Game::GetInstance()->GetScreenWidth(),
													Game::GetInstance()->GetScreenHeight() }, !(Game::GetInstance()->GetWindowed()));
		Game::GetInstance()->SetWindowed(!(Game::GetInstance()->GetWindowed()));
	}

	//DOOR TEST This will be removed later
	//if(pInput->IsKeyPressed(SGD::Key::X))
	//{
	//	m_pDoor->SetOpen(!(m_pDoor->GetOpen()));
	//}
	if (pInput->IsKeyPressed(SGD::Key::H))
	{
		//m_pPlayer->SetHonorCollected(m_pPlayer->GetHonorCollected() + 50);
		////Gained Honor event for certain doors
		//SGD::Event* pATEvent = new SGD::Event("HONOR_GAINED", nullptr, m_pPlayer);
		//SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
		//pATEvent = nullptr;

	}

	// Temporary test for level changing
	if (pInput->IsKeyPressed(SGD::Key::P))
	{
		LoadLevel("Level2_3");
		m_bShowFPS ? m_bShowFPS = false : m_bShowFPS = true;
	}
	if (pInput->IsKeyPressed(SGD::Key::O) && (m_strCurrLevel != "Level0_1" && m_strCurrLevel != "Level0_2"))
	{
		LoadLevel("HubLevel");
	}	/*if(pInput->IsKeyPressed(SGD::Key::L))
	{
	LoadLevel("Level4_1");
	}	if(pInput->IsKeyPressed(SGD::Key::I))
	{
	LoadLevel("Level5_1");
	}	if(pInput->IsKeyPressed(SGD::Key::M))
	{
	LoadLevel("Level5_5");
	}
	*/
	/*if (pInput->IsKeyPressed(SGD::Key::L))
	{
		WizardDefeated();
	}*/

	if (pInput->IsKeyPressed(SGD::Key::Escape)
		|| pInput->IsButtonPressed(0, 7 /*Button start on xbox controller*/) )//|| /*For Arcade Input*/pInput->IsKeyPressed(SGD::Key::MouseRight))
	{
		Game::GetInstance()->AddState(PauseState::GetInstance());
		//pAudio->StopAudio(m_hBGM);
	}

	/*if (pInput->IsKeyDown(SGD::Key::Alt) && pInput->IsKeyPressed(SGD::Key::Tab))
	{
	Game::GetInstance()->AddState(PauseState::GetInstance());
	pAudio->StopAudio(m_hBGM);
	}*/

	return true;

}

//////////////////////////////////////////////
// Update
// - Update all game entities
void GameplayState::Update(float elapsedTime)
{
	//if (m_pHonor->GetIsCollected() == true)
	//	m_pEntities->RemoveEntity(m_pHonor);

	//if (m_pArmor->GetIsCollected() == true)
	//	m_pEntities->RemoveEntity(m_pArmor);

	//	m_pCamera->Update(elapsedTime);

	//m_pEmitter2->Update(elapsedTime);
	float x = elapsedTime;

	// Toggle for mini map
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::M) && ending == false)
		//|| /*For Arcade Input*/SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::MouseLeft))
	{
		m_bRenderMiniMap = !m_bRenderMiniMap;
	}

	m_pEntities->UpdateAll(elapsedTime);
	Camera::GetInstance()->Update(elapsedTime);

	m_pEntities->CheckWorldCollision(Entity::ENT_PLAYER);


	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_HONOR);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_BLOCK);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_PERM_FREEZE);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_TEMP_FREEZE);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_DOOR);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_BOSS_DOOR);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_SWITCH);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_BUZZSAW);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_PROJ);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_GEYSER);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_LASER);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_LAVA);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_MOVING_PLATFORM);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_TELEPORTER);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_ENEMY);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_JELLYFISH);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_BOSS_BULL);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_STATUE);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_PENDULUM);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_POUNCER);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_BULL_ENEMY);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_POOP);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_STALACTITE);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_DOOR);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_BOSS_YETI);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_ICE_GOLEM);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_WIZARD_DASH);
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_WIZARD_HAWK);


	m_pEntities->CheckCollisions(Entity::ENT_ENEMY, Entity::ENT_SWORD);
	m_pEntities->CheckCollisions(Entity::ENT_ICE_TURTLE, Entity::ENT_SWORD);
	m_pEntities->CheckCollisions(Entity::ENT_ICE_GOLEM, Entity::ENT_SWORD);

	m_pEntities->CheckCollisions(Entity::ENT_PRESSURE_PLATE, Entity::ENT_SWORD);

	m_pEntities->CheckCollisions(Entity::ENT_SWITCH, Entity::ENT_SWORD);
	m_pEntities->CheckCollisions(Entity::ENT_BOSS_BULL, Entity::ENT_DOOR);
	m_pEntities->CheckCollisions(Entity::ENT_BOSS_CAVEMAN, Entity::ENT_LASER);



	m_pEntities->CheckCollisions(Entity::ENT_TEMP_FREEZE, Entity::ENT_SPRAY);
	m_pEntities->CheckCollisions(Entity::ENT_GEYSER, Entity::ENT_SPRAY);
	m_pEntities->CheckCollisions(Entity::ENT_SPRAY, Entity::ENT_TEMP_FREEZE);


	m_pEntities->CheckCollisions(Entity::ENT_PROJ, Entity::ENT_BLOCK);
	m_pEntities->CheckCollisions(Entity::ENT_FALLING_BLOCK, Entity::ENT_BLOCK);
	m_pEntities->CheckCollisions(Entity::ENT_FALLING_BLOCK, Entity::ENT_PLAYER);
	m_pEntities->CheckCollisions(Entity::ENT_HAWK, Entity::ENT_STALACTITE);
	m_pEntities->CheckCollisions(Entity::ENT_HAWK, Entity::ENT_SWITCH);
	m_pEntities->CheckCollisions(Entity::ENT_HAWK, Entity::ENT_GEYSER);
	m_pEntities->CheckCollisions(Entity::ENT_HAWK, Entity::ENT_BOSS_WIZARD);
	m_pEntities->CheckCollisions(Entity::ENT_HAWK, Entity::ENT_DEATH);
	m_pEntities->CheckCollisions(Entity::ENT_HAWK, Entity::ENT_DOOR);


	m_pEntities->CheckCollisions(Entity::ENT_BOSS_CRAB, Entity::ENT_LASER);
	m_pEntities->CheckCollisions(Entity::ENT_DOOR, Entity::ENT_LASER);
	m_pEntities->CheckCollisions(Entity::ENT_BULL_ENEMY, Entity::ENT_LASER);

	m_pEntities->CheckCollisions(Entity::ENT_BOSS_CRAB, Entity::ENT_PLAYER);
	m_pEntities->CheckCollisions(Entity::ENT_BOSS_YETI, Entity::ENT_PLAYER);
	m_pEntities->CheckCollisions(Entity::ENT_BOSS_YETI, Entity::ENT_PERM_FREEZE);
	m_pEntities->CheckCollisions(Entity::ENT_BOSS_YETI, Entity::ENT_TEMP_FREEZE);
	m_pEntities->CheckCollisions(Entity::ENT_POUNCER, Entity::ENT_DOOR);
	m_pEntities->CheckCollisions(Entity::ENT_PROJ, Entity::ENT_DOOR);
	m_pEntities->CheckCollisions(Entity::ENT_POUNCER, Entity::ENT_LEFT_RAMP);
	m_pEntities->CheckCollisions(Entity::ENT_POUNCER, Entity::ENT_RIGHT_RAMP);
	m_pEntities->CheckCollisions(Entity::ENT_POUNCER, Entity::ENT_DEATH);
	//FLocking Collisions
	m_pEntities->CheckCollisions(Entity::ENT_MUTANT_MAN, Entity::ENT_DOOR);
	m_pEntities->CheckCollisions(Entity::ENT_MUTANT_BIRD, Entity::ENT_MUTANT_BIRD);
	m_pEntities->CheckCollisions(Entity::ENT_MUTANT_MAN, Entity::ENT_MUTANT_MAN);
	m_pEntities->CheckCollisions(Entity::ENT_ENEMY, Entity::ENT_ENEMY);



	//if (m_pArmor != nullptr)
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_ARMOR);

	//if (m_pHonor != nullptr)
	//m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_HONOR);

	m_pEntities->CheckWorldCollision(Entity::ENT_FALLING_BLOCK);
	m_pEntities->CheckWorldCollision(Entity::ENT_PROJ);
	m_pEntities->CheckWorldCollision(Entity::ENT_HAWK);
	m_pEntities->CheckWorldCollision(Entity::ENT_STALACTITE);
	m_pEntities->CheckWorldCollision(Entity::ENT_LASER);
	m_pEntities->CheckWorldCollision(Entity::ENT_BOSS_BULL);
	m_pEntities->CheckWorldCollision(Entity::ENT_MUTANT_MAN);
	m_pEntities->CheckWorldCollision(Entity::ENT_BOSS_YETI);
	m_pEntities->CheckWorldCollision(Entity::ENT_BOSS_CAVEMAN);

	m_pEntities->CheckWorldCollision(Entity::ENT_VOMIT);
	m_pEntities->CheckWorldCollision(Entity::ENT_POOP);
	m_pEntities->CheckWorldCollision(Entity::ENT_ENEMY);
	m_pEntities->CheckWorldCollision(Entity::ENT_BULL_ENEMY);
	m_pEntities->CheckWorldCollision(Entity::ENT_SKELETON);
	m_pEntities->CheckWorldCollision(Entity::ENT_ICE_GOLEM);

	m_pEntities->CheckWorldCollision(Entity::ENT_POUNCER);
	m_pEntities->CheckWorldCollision(Entity::ENT_MUTANT_BIRD);
	m_pEntities->CheckWorldCollision(Entity::ENT_JELLYFISH);
	m_pEntities->CheckWorldCollision(Entity::ENT_SPRAY);
	m_pEntities->CheckWorldEvent(Entity::ENT_PLAYER);
	m_pEntities->CheckWorldEvent(Entity::ENT_BOSS_BULL);
	m_pEntities->CheckWorldEvent(Entity::ENT_BULL_ENEMY);
	m_pEntities->CheckWorldEvent(Entity::ENT_BOSS_CAVEMAN);
	m_pEntities->CheckWorldEvent(Entity::ENT_BOSS_YETI);

	//Entities WHich SLow the Player
	m_pEntities->CheckCollisions(Entity::ENT_PLAYER, Entity::ENT_VOMIT);

	//Update The Hubworld Orb
	if (m_strCurrLevel == "HubLevel")
	{
		m_pHubOrb->Update(elapsedTime, m_pPlayer->GetHonorCollected(), { (float)GetCurrentLevel()->GetLevelWidth() / 2, (float)GetCurrentLevel()->GetLevelHeight() / 2 });
	}

	//Process messages and events
	SGD::EventManager::GetInstance()->Update();
	SGD::MessageManager::GetInstance()->Update();

	if (ending == true)
	{
		endingTimer += elapsedTime;
		if (endingTimer >= 5.0f)
		{
			endFadeTimer += elapsedTime;
			if (endFadeTimer < 3.0f)
			{
				endFade += (unsigned char)(159 * elapsedTime);
				SetScreenFadeout(endFade);
			}
			else
			{
				ending = false;
				endFade = 0;
				endFadeTimer = 0.0f;
				endingTimer = 0.0f;
				SetScreenFadeout(0);
				Game::GetInstance()->AddState(CreditsState::GetInstance());
			}
		}
	}
	// Increase the FPS timer
	m_fFPSTimer += elapsedTime;
	m_unFrames++;

	if (m_fFPSTimer >= 1.0f)		// 1 second refresh rate
	{
		m_unFPS = m_unFrames;
		m_unFrames = 0;
		m_fFPSTimer = 0.0f;
	}
}

/////////////////////////////////////////////
// Render
// - Render all game entities
void GameplayState::Render(void)
{
	// Render the FPS
	SGD::OStringStream output;
	if (m_bShowFPS)
	{
		output << "FPS: " << m_unFPS;
	}
	/*if (ending == false)
	{*/

	m_pLevel->Render();
	m_pLevel->RenderImageLayer(true);

	//Render Images for tutorial 
	if (m_strCurrLevel == "Level0_1" || m_strCurrLevel == "Level0_2")
	{
		Camera::GetInstance()->DrawTexture({ 600, 300 }, 0, m_hXJUMP, false, 1, {}, {});
		Camera::GetInstance()->DrawTexture({ 1759, 300 }, 0, m_hXJUMP, false, 1, {}, {});
		Camera::GetInstance()->DrawTexture({ 2720, 200 }, 0, m_hXWallJump, false, 1, {}, {});
		Camera::GetInstance()->DrawTexture({ 3803, 60 }, 0, m_hOAttack, false, .5, {}, {});
		Camera::GetInstance()->DrawTexture({ 4180, 250 }, 0, m_hTriOpenDoor, false, 1, {}, {});
	}
	//Camera::GetInstance()->DrawTexture({ 270, 400 }, {}, SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/images.jpg"), false);
	m_pEntities->RenderAll();
	m_pLevel->RenderImageLayer(false);

	//Render the Hub world Orb
	if (m_strCurrLevel == "HubLevel")
	{
		m_pHubOrb->Render();
	}

	// Draw the mini map
	if (m_bRenderMiniMap && ending == false)
	{
		RenderMiniMap();
	}


	
	//
	// Draw a fading rectangle
	SGD::Rectangle rect = SGD::Rectangle(0, 0, Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight());
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { m_cScreenFade, 0, 0, 0 }, { 0, 0, 0, 0 }, 0);
	SGD::GraphicsManager::GetInstance()->DrawString(output.str().c_str(), { 5, 5 });

}

//Static Message callback function
void GameplayState::MessageProc(const SGD::Message* pMsg)
{

	//Show warning when a message ID enumerator is not handled
#pragma warning (push)
#pragma warning (1 : 4061)

	//What type of message is this
	switch (pMsg->GetMessageID())
	{
		case MessageID::MSG_CREATE_STALACTITE:
		{
			//Downcast to the real message type
			const CreateStalactiteMessage* pCreateMsg =
				dynamic_cast<const CreateStalactiteMessage*>(pMsg);

			//Make sure the message isnt a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CREATE_STALACTITE is not actually a CreateSTALACTITEMessage");
			Stalactite* Temp = new Stalactite();
			Temp->SetPosition(pCreateMsg->GetOwner()->GetPosition());
			Temp->SetSize({ 64, 64 });
			Temp->SetFallSpeed(1000);
			GetInstance()->m_pEntities->AddEntity(Temp, Entity::ENT_STALACTITE);

			Temp->Release();
			Temp = nullptr;
			break;
		}
		case MessageID::MSG_CREATE_VOMIT:
		{
			//Downcast to the real message type
			const CreateVomitMessage* pCreateMsg =
				dynamic_cast<const CreateVomitMessage*>(pMsg);

			//Make sure the message isnt a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CREATE_VOMIT is not actually a CreateVomitMessage");
			pCreateMsg->GetOwner()->GetPosition();
			Vomit* Temp = new Vomit(pCreateMsg->GetOwner()->GetPosition());

			GetInstance()->m_pEntities->AddEntity(Temp, Entity::ENT_VOMIT);

			Temp->Release();
			Temp = nullptr;
			break;
		}
		case MessageID::MSG_CREATE_POOP:
		{
			//Downcast to the real message type
			const CreatePoopMessage* pCreateMsg =
				dynamic_cast<const CreatePoopMessage*>(pMsg);

			//Make sure the message isnt a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CREATE_POOP is not actually a CreatePoopMessage");
			pCreateMsg->GetOwner()->GetPosition();
			Poop* Temp = new Poop(pCreateMsg->GetOwner()->GetPosition());

			GetInstance()->m_pEntities->AddEntity(Temp, Entity::ENT_POOP);

			Temp->Release();
			Temp = nullptr;
			break;
		}
		case MessageID::MSG_DESTROY_ENTITY:
		{
			//Downcast to the real message type
			const DestroyEntityMessage* pDestroyMsg =
				dynamic_cast<const DestroyEntityMessage*>(pMsg);

			//Make sure the message isnt a nullptr
			assert(pDestroyMsg != nullptr
				   && "GameplayState::MessageProc - MSG_DESTROY_ENTITY is not actually a DestroyEntityMessage");

			//get the message's entity
			Entity* ptr = pDestroyMsg->GetEntity();

			//Use the singleton to access the entity manager
			if (ptr != nullptr)
			{
				GameplayState::GetInstance()->m_pEntities->RemoveEntity(ptr);
			}

			break;
		}
		case MessageID::MSG_CREATE_PROJECTILE:
		{
			//Downcast to the real message type
			const CreateProjectileMessage* pCreateMsg =
				dynamic_cast<const CreateProjectileMessage*>(pMsg);

			//Make sure the message isn't a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CREATE_PROJECTILE is not actually a CreateProjectileMessage");

			//Create a local reference to the gameplaystate singleton
			GameplayState* pSelf = GameplayState::GetInstance();


			//Play the projectile's audio sound

			//Call CreateProjectile factory method sending in the messages projectile
			Entity* pProj = pSelf->CreateProjectile(pCreateMsg->GetOwner());


			if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_PLAYER)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_PROJ);
			}
			else if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_TURRET)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_PROJ);
			}
			else if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_ICE_TURTLE)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_PROJ);


			}

			// if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_PLAYER)
			// {
			//	 pSelf->m_pEntities->AddEntity(pProj, EntityManager::BUCKET_PLAYER_PROJ);
			// }
			// else if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_ENEMY)
			// {
			//	 pSelf->m_pEntities->AddEntity(pProj, EntityManager::BUCKET_ENEMY_PROJ);
			// }

			pProj->Release();
			pProj = nullptr;


			break;
		}
		case MessageID::MSG_CREATE_GRAVPROJECTILE:
		{
			//Downcast to the real message type
			const CreateGravProjectileMessage* pCreateMsg = dynamic_cast<const CreateGravProjectileMessage*>(pMsg);

			//Make sure the message isn't a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CREATE_GRAVPROJECTILE is not actually a CreateGravProjectileMessage");

			//Create a local reference to the gameplaystate singleton
			GameplayState* pSelf = GameplayState::GetInstance();


			//Play the projectile's audio sound

			//Call CreateProjectile factory method sending in the messages projectile
			Entity* pProj = pSelf->CreateGravProjectile(pCreateMsg->GetOwner());


			if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_SQUID)
			{
				GravProjectile * temp = (GravProjectile*)pProj;
				temp->SetProjectileType(GravProjectile::INK);
			}
			else if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_SKELETON)
			{
				GravProjectile * temp = (GravProjectile*)pProj;
				temp->SetProjectileType(GravProjectile::BONE);
			}

			pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_PROJ);

			pProj->Release();
			pProj = nullptr;


			break;
		}
		case MessageID::MSG_CREATE_HORIZ_BUBBLE:
		{
			//Downcast to the real message type
			const CreateHorizontalBubble* pCreateMsg = dynamic_cast<const CreateHorizontalBubble*>(pMsg);

			//Make sure the message isn't a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CREATE_HORIZ_BUBBLE is not actually a CreateHorizontalBubbleMessage");

			//Create a local reference to the gameplaystate singleton
			GameplayState* pSelf = GameplayState::GetInstance();


			//Play the projectile's audio sound

			//Call CreateProjectile factory method sending in the messages projectile
			Entity* pProj = pSelf->CreateHorizBubble(pCreateMsg->GetOwner());


			if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_BOSS_CRAB)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_PROJ);
			}
			// if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_BOSS_WIZARD)
			// {
			//	   pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_PROJ);
			//
			// }

			pProj->Release();
			pProj = nullptr;


			break;
		}
		case MessageID::MSG_CREATE_VERT_BUBBLE:
		{
			//Downcast to the real message type
			const CreateVerticalBubble* pCreateMsg = dynamic_cast<const CreateVerticalBubble*>(pMsg);

			//Make sure the message isn't a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CREATE_VERT_BUBBLE is not actually a CreateVerticalBubble");

			//Create a local reference to the gameplaystate singleton
			GameplayState* pSelf = GameplayState::GetInstance();


			//Play the projectile's audio sound

			//Call CreateProjectile factory method sending in the messages projectile
			Entity* pProj = pSelf->CreateVertBubble(pCreateMsg->GetOwner());


			if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_BOSS_CRAB)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_PROJ);
			}

			if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_BOSS_WIZARD)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_PROJ);

			}

			pProj->Release();
			pProj = nullptr;


			break;
		}
		case MessageID::MSG_CREATE_SPRAY:
		{

			//Downcast to the real message type
			const CreateSprayMessage* pCreateMsg =
				dynamic_cast<const CreateSprayMessage*>(pMsg);

			//Make sure the message isn't a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CREATE_PROJECTILE is not actually a CreateProjectileMessage");

			//Create a local reference to the gameplaystate singleton
			GameplayState* pSelf = GameplayState::GetInstance();


			//Play the projectile's audio sound

			//Call CreateProjectile factory method sending in the messages projectile
			Entity* pProj = pSelf->CreateSpray(pCreateMsg->GetOwner());


			if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_PLAYER)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_SPRAY);
			}
			else if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_BOSS_YETI)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_SPRAY);
			}


			//pProj->Release();
			//pProj = nullptr;

			//pProj = pSelf->CreateSpray(pCreateMsg->GetOwner());

			//if(pCreateMsg->GetOwner()->GetType() == Entity::ENT_PLAYER)
			//{
			//	pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_SPRAY);
			//}
			//else if(pCreateMsg->GetOwner()->GetType() == Entity::ENT_BOSS_YETI)
			//{
			//	pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_SPRAY);
			//}
			// if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_PLAYER)
			// {
			//	 pSelf->m_pEntities->AddEntity(pProj, EntityManager::BUCKET_PLAYER_PROJ);
			// }
			// else if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_ENEMY)
			// {
			//	 pSelf->m_pEntities->AddEntity(pProj, EntityManager::BUCKET_ENEMY_PROJ);
			// }

			pProj->Release();
			pProj = nullptr;


			break;

		}
		case MessageID::MSG_CREATE_HAWK:
		{

			//Downcast to the real message type
			const CreateHawkMessage* pCreateMsg =
				dynamic_cast<const CreateHawkMessage*>(pMsg);

			//Make sure the message isn't a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CREATE_PROJECTILE is not actually a CreateProjectileMessage");

			//Create a local reference to the gameplaystate singleton
			GameplayState* pSelf = GameplayState::GetInstance();


			//Play the projectile's audio sound


			//Call CreateProjectile factory method sending in the messages projectile
			Entity* pProj = pSelf->CreateHawk(pCreateMsg->GetOwner());


			if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_PLAYER)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_HAWK);


			}
			else if (pCreateMsg->GetOwner()->GetType() == Entity::ENT_ENEMY)
			{
				pSelf->m_pEntities->AddEntity(pProj, Entity::ENT_HAWK);
			}

			pProj->Release();
			pProj = nullptr;


			break;

		}
		case MessageID::MSG_CHANGE_LEVEL:
		{
			//Downcast to the real message type
			const ChangeLevelMessage* pCreateMsg =
				dynamic_cast<const ChangeLevelMessage*>(pMsg);

			//Make sure the message isn't a nullptr
			assert(pCreateMsg != nullptr
				   && "GameplayState::MessageProc - MSG_CHANGE_LEVEL is not actually a CreateProjectileMessage");

			//Create a local reference to the gameplaystate singleton
			GameplayState* pSelf = GameplayState::GetInstance();

			// Reference to the teleporter entity
			Teleporter * teleporter = dynamic_cast<Teleporter*>(pCreateMsg->GetOwner());
			GameplayState::GetInstance()->SaveGame();
			pSelf->LoadLevel(teleporter->GetLevel());

		}
		case MessageID::MSG_UNKNOWN:
		{
			OutputDebugStringW(L"Game::MessageProc - unknown message id\n");
			break;
		}
		default:
		{

			break;
		}
	}


}


///////////////////////////////////////////////////////////
//////////////////// Factory Methods /////////////////////

//Creates a new projectile to be added to the entity manager
Entity* GameplayState::CreateProjectile(Entity* pOwner) const
{
	Projectile* proj = new Projectile();
	//if (pOwner->GetType() != Entity::ENT_BOSS_WIZARD)

	if (pOwner->GetDirection().x == 1)
		proj->SetPosition(SGD::Point(pOwner->GetRect().right, pOwner->GetPosition().y - pOwner->GetSize().height / 2));
	else if (pOwner->GetDirection().x == -1)
		proj->SetPosition(SGD::Point(pOwner->GetRect().left, pOwner->GetPosition().y + pOwner->GetSize().height / 2));
	else if (pOwner->GetDirection().y == -1)
		proj->SetPosition(SGD::Point(pOwner->GetSize().width / 2, pOwner->GetRect().top));
	else if (pOwner->GetDirection().y == 1)
		proj->SetPosition(SGD::Point(pOwner->GetSize().width / 2, pOwner->GetRect().bottom));

	if (pOwner->GetDirection().x == 1)
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x + pOwner->GetSize().width, pOwner->GetPosition().y + pOwner->GetSize().height / 2));
	else
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x, pOwner->GetPosition().y + pOwner->GetSize().height / 2));


	proj->SetSize({ 16, 16 });
	proj->SetDirection({ pOwner->GetDirection() });
	//proj->SetVelocity({ pOwner->GetDirection() });
	proj->SetOwner(pOwner);

	return proj;

}

Entity* GameplayState::CreateGravProjectile(Entity* pOwner) const
{
	GravProjectile* proj = new GravProjectile();
	if (pOwner->GetDirection().x == 1)
		proj->SetPosition(SGD::Point(pOwner->GetRect().right, pOwner->GetRect().top /*- pOwner->GetRect().ComputeHeight() / 2*/));
	else
		proj->SetPosition(SGD::Point(pOwner->GetRect().left, pOwner->GetRect().top /*- pOwner->GetRect().ComputeHeight() / 2*/));

	proj->SetDirection({ pOwner->GetDirection() });
	proj->SetOwner(pOwner);

	return proj;
}

Entity* GameplayState::CreateHorizBubble(Entity* pOwner) const
{
	/*HorizontalBubble* proj = new HorizontalBubble();
	proj->SetPosition(SGD::Point(pOwner->GetPosition().x, pOwner->GetPosition().y - 50));
	proj->SetOwner(pOwner);
	proj->SetSize({ 40, 40 });*/

	if (pOwner->GetType() == Entity::ENT_BOSS_WIZARD)
	{

		GameplayState* pSelf = GameplayState::GetInstance();

		HorizontalBubble* proj_1 = new HorizontalBubble();
		proj_1->SetPosition(SGD::Point(pOwner->GetPosition().x, pOwner->GetPosition().y + 100));
		proj_1->SetOwner(pOwner);
		proj_1->SetSize({ 40, 40 });
		proj_1->SetDirection({ -1, 1 });
		pSelf->m_pEntities->AddEntity(proj_1, Entity::ENT_PROJ);

		proj_1->Release();


		HorizontalBubble* proj_2 = new HorizontalBubble();
		proj_2->SetPosition(SGD::Point(pOwner->GetPosition().x, pOwner->GetPosition().y + 100));
		proj_2->SetOwner(pOwner);
		proj_2->SetSize({ 40, 40 });
		proj_2->SetDirection({ 0, 1 });
		pSelf->m_pEntities->AddEntity(proj_2, Entity::ENT_PROJ);

		proj_2->Release();

		HorizontalBubble* proj = new HorizontalBubble();
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x, pOwner->GetPosition().y + 100));
		proj->SetOwner(pOwner);
		proj->SetSize({ 40, 40 });
		proj->SetDirection({ 1, 1 });
		pSelf->m_pEntities->AddEntity(proj, Entity::ENT_PROJ);


		//proj->SetDirection({ -1, 1 });


		return proj;



	}
	else
	{
		HorizontalBubble* proj = new HorizontalBubble();
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x, pOwner->GetPosition().y - 50));
		proj->SetOwner(pOwner);
		proj->SetSize({ 40, 40 });

		if (rand() % 2 == 1)
			proj->SetDirection({ 1, 0 });
		else
			proj->SetDirection({ -1, 0 });

		return proj;
	}
}

Entity* GameplayState::CreateVertBubble(Entity* pOwner) const
{
	VerticalBubble* proj = new VerticalBubble();
	proj->SetPosition(SGD::Point(pOwner->GetPosition().x, pOwner->GetPosition().y - 50));
	proj->SetOwner(pOwner);
	proj->SetSize({ 40, 40 });

	if (rand() % 2 == 1)
		proj->SetDirection({ 1, 0 });
	else
		proj->SetDirection({ -1, 0 });

	return proj;
}

//Creates a new projectile to be added to the entity manager
Entity* GameplayState::CreateSpray(Entity* pOwner) const
{
	Ice* proj = new Ice;

	int range = rand() % 96 + 5;
	if (pOwner->GetDirection().x == 1)
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x + pOwner->GetSize().width + range, pOwner->GetPosition().y + pOwner->GetSize().height / 2));
	else
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x - pOwner->GetSize().width - range, pOwner->GetPosition().y + pOwner->GetSize().height / 2));

	proj->SetSize({ 4, 4 });
	proj->SetDirection({ pOwner->GetDirection().x, -1 });
	//proj->SetVelocity({ pOwner->GetDirection() });
	proj->SetOwner(pOwner);

	return proj;

}


Hawk* GameplayState::CreateHawk(Entity* pOwner) const
{
	Hawk* proj = new Hawk;
	if (pOwner->GetDirection().x == 1)
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x + pOwner->GetSize().width, pOwner->GetPosition().y + pOwner->GetSize().height / 2));
	else
		proj->SetPosition(SGD::Point(pOwner->GetPosition().x, pOwner->GetPosition().y + pOwner->GetSize().height / 2));

	proj->SetSize({ 20, 20 });
	proj->SetDirection({ 0, 0 });
	proj->SetOwner(pOwner);
	//pOwner->SetHawkPtr(proj);
	m_pPlayer->SetHawkPtr(proj);

	return proj;

}



/////////////////////////////////////////////
// Factory Methods
// - Creates character class
Player* GameplayState::CreatePlayer(void)
{
	Player* pPlayer = new Player;
	SwordSwing* pSword = new SwordSwing;

	pPlayer->SetPosition(SGD::Point(100, 100));
	pPlayer->SetSize(SGD::Size(32, 64));


	pSword->SetSize(SGD::Size(800, 80));
	pPlayer->SetSword(pSword);
	m_pEntities->AddEntity(pSword, Entity::ENT_SWORD);


	return pPlayer;



}

void GameplayState::CreateBlocks(void)
{


	for (unsigned int i = 0; i < 10; i++)
	{
		Block* pBlock_1 = new Block;

		pBlock_1->SetPosition(SGD::Point(100 + (i * 20.0f), 400.0f));
		pBlock_1->SetSize(SGD::Size(20, 20));

		m_pEntities->AddEntity(pBlock_1, Entity::ENT_BLOCK);

		pBlock_1->Release();
	}

	//for (unsigned int i = 0; i < 40; i++)
	//{
	//	Block* pBlock_2 = new Block;

	//	pBlock_2->SetPosition(SGD::Point(600, 380 - (i * 20)));
	//	pBlock_2->SetSize(SGD::Size(20, 20));

	//	m_pEntities->AddEntity(pBlock_2, Entity::ENT_BLOCK);

	//	pBlock_2->Release();
	//}

}

void GameplayState::CreatePermFrozenTiles(void)
{

	for (unsigned int i = 0; i < 10; i++)
	{
		FreezeableGround* pFreeze = new FreezeableGround;


		//// TYPE STATE WILL CHANGE ( Needs to be done through TYPE with enttiyManger) Two types for each frozen tile === It's Current state and it's Bucket type (temp or perm tile)
		pFreeze->SetType(Entity::ENT_FROZEN);

		pFreeze->SetSize(SGD::Size(20, 20));
		pFreeze->SetPosition(SGD::Point(300 + (i * 20.0f), 400.0f));
		pFreeze->SetIsFrozen(true);
		pFreeze->SetIsPerm(true);

		////// TYPE OF TILE Bucket
		m_pEntities->AddEntity(pFreeze, Entity::ENT_PERM_FREEZE);

		pFreeze->Release();
	}


}


void GameplayState::CreateTempFrozenTiles(void)
{
	for (unsigned int i = 0; i < 10; i++)
	{
		FreezeableGround* pFreeze = new FreezeableGround;

		//// TYPE STATE WILL CHANGE ( Needs to be done through TYPE with enttiyManger) Two types for each frozen tile === It's Current state and it's Bucket type (temp or perm tile)
		pFreeze->SetType(Entity::ENT_NOT_FROZEN);

		pFreeze->SetSize(SGD::Size(100, 100));
		pFreeze->SetPosition(SGD::Point(500 + (i * 20.0f), 400.0f));
		pFreeze->SetIsFrozen(false);
		pFreeze->SetIsPerm(false);

		////// TYPE OF TILE Bucket
		m_pEntities->AddEntity(pFreeze, Entity::ENT_TEMP_FREEZE);

		pFreeze->Release();
	}

}

#pragma region Entity factory methods

/////////////////////////
// CreateHonor
// -Creates an honor collectible at the given coordinates
void GameplayState::CreateHonor(int _x, int _y, int _amount, unsigned int _index)
{
	Honor * mHonor = new Honor();
	mHonor->SetPosition({ (float)_x, (float)_y });
	mHonor->SetHonorAmount(_amount);
	mHonor->SetEmitter();
	mHonor->SetVectorID(_index);
	// Check if this has been collected
	mHonor->SetIsCollected(false);
	if (_index < GetHonorVectorSize())
	{
		mHonor->SetIsCollected(m_mCollectedHonor[m_strCurrLevel][_index]);
		mHonor->SetStartedCollected(m_mCollectedHonor[m_strCurrLevel][_index]);
	}
	m_pEntities->AddEntity(mHonor, Entity::ENT_HONOR);
	mHonor->Release();
}


/////////////////////////
// CreateActivator
// -Creates an activator at the given coordinates
void GameplayState::CreateActivator(int _x, int _y, bool _isPressure, bool _currState, int _ID)
{
	Activator * pActivator = new Activator(_isPressure);
	pActivator->SetPosition({ (float)_x, (float)_y });
	pActivator->SetOn(_currState);
	pActivator->SetPlayer(m_pPlayer);
	pActivator->SetKeyID(_ID);
	pActivator->SetStartOn(_currState);
	m_pEntities->AddEntity(pActivator, Entity::ENT_SWITCH);
	pActivator->Release();
}

/////////////////////////
// CreateLaser
// -Creates a laser at the given coordinates
void GameplayState::CreateLaser(int x, int y, SGD::Vector _direction, int _ID, bool _on)
{
	Laser* m_pLaser = new Laser;
	m_pLaser->SetPosition({ (float)x, (float)y });
	m_pLaser->SetOrigPosition({ (float)x, (float)y });
	m_pLaser->SetDirection({ _direction });
	m_pLaser->SetFreq(_ID);
	m_pLaser->SetOn(_on);

	//Activator* m_pLaserSwitch = new Activator(false);
	//m_pLaserSwitch->SetPosition({ (float)_switchX, (float)_switchY });

	//m_pLaserSwitch->SetKeyID(2);

	m_pEntities->AddEntity(m_pLaser, Entity::ENT_LASER);
	//m_pEntities->AddEntity(m_pLaserSwitch, Entity::ENT_SWITCH);
	//m_pLaserSwitch->Release();
	m_pLaser->Release();
}

/////////////////////////
// CreateTurret
// -Creates a turret at the given coordinates
void GameplayState::CreateTurret(int x, int y, int _direction, float _timer)
{
	Turret * mTurret = new Turret();
	mTurret->SetPosition({ (float)x, (float)y });
	mTurret->SetFireTimer(_timer);
	mTurret->SetShootDirection(_direction);
	m_pEntities->AddEntity(mTurret, Entity::ENT_TURRET);
	mTurret->Release();
}

/////////////////////////
// CreateDoor
// -Creates a door at the given coordinates
void GameplayState::CreateDoor(int _x, int _y, bool _isHorizontal, int _ID, bool _startOpen)
{
	Door * pDoor = new Door();
	pDoor->SetPosition({ (float)_x, (float)_y });
	if (_isHorizontal)
	{
		pDoor->SetSize({ 128.0f, 32.0f });
	}
	else
	{
		pDoor->SetSize({ 32.0f, 128.0f });
	}
	pDoor->SetHorizontal(_isHorizontal);
	pDoor->SetKeyID(_ID);
	pDoor->SetStartOpen(_startOpen);
	pDoor->SetOpen(_startOpen);
	m_pEntities->AddEntity(pDoor, Entity::ENT_DOOR);
	pDoor->Release();
}

/////////////////////////
// CreateMovingPlatform
// -Creates a moving platform at the given coordinates
void GameplayState::CreateMovingPlatform(int _x, int _y, bool _vertical, float _turnDistance, float _speed)
{
	MovingPlatform * mPlatform = new MovingPlatform();
	mPlatform->SetPosition({ (float)_x, (float)_y });
	mPlatform->SetSize({ 128.0f, 32.0f });
	mPlatform->SetVertical(_vertical);
	mPlatform->SetTurnDistance(100);

	//mPlatform->SetTurnDistance(_turnDistance);
	mPlatform->SetSpeed(_speed);
	mPlatform->SetTurnDistance(_turnDistance);
	m_pEntities->AddEntity(mPlatform, Entity::ENT_MOVING_PLATFORM);
	mPlatform->Release();
}

/////////////////////////
// CreateGeyser
// -Creates a geyser at the given coordinates
void GameplayState::CreateGeyser(int _x, int _y, float _speed, float _maxHeight, int _currState)
{
	Geyser* m_pGeyser = new Geyser;
	m_pGeyser->SetPosition({ (float)_x, (float)_y });
	m_pGeyser->SetOrigPosition({ (float)_x, (float)_y });
	m_pGeyser->SetSpeed(_speed);
	m_pGeyser->SetMaxHeight(_maxHeight);
	m_pGeyser->SetState(_currState);
	m_pEntities->AddEntity(m_pGeyser, Entity::ENT_GEYSER);

	m_pGeyser->Release();
}


/////////////////////////
// CreateMovingPlatform
// -Creates a lava at the given coordinates
void GameplayState::CreateLava(int x, int y, float _speed, float _maxHeight, int _currState)
{
	Lava* m_pLava = new Lava;
	m_pLava->SetPosition({ (float)x, (float)y });
	m_pLava->SetOrigPosition({ (float)x, (float)y });
	m_pEntities->AddEntity(m_pLava, Entity::ENT_LAVA);

	m_pLava->Release();
}

/////////////////////////
// CreateFallingBlock
// -Creates a falling block at the given coordinates
void GameplayState::CreateFallingBlock(int _x, int _y)
{
	FallingBlock * fBlock = new FallingBlock();
	fBlock->SetPosition({ (float)_x, (float)_y });
	fBlock->SetStartPosition({ (float)_x, (float)_y });
	m_pEntities->AddEntity(fBlock, Entity::ENT_FALLING_BLOCK);
	fBlock->Release();
}

/////////////////////////
// CreateBlock
// -Creates a block at the given coordinates
void GameplayState::CreateBlock(int _x, int _y)
{
	Block * mBlock = new Block();
	mBlock->SetPosition({ (float)_x, (float)_y });
	m_pEntities->AddEntity(mBlock, Entity::ENT_BLOCK);
	mBlock->Release();
}

/////////////////////////
// CreatePendulum
// -Creates a pendulum at the given coordinates
void GameplayState::CreatePendulum(int _x, int _y)
{
	Pendulum * mPendulum = new Pendulum();
	mPendulum->SetPosition({ (float)_x, (float)_y });
	m_pEntities->AddEntity(mPendulum, Entity::ENT_PENDULUM);
	mPendulum->Release();
}

/////////////////////////
// CreateBuzzsaw
// -Creates a buzz saw at the given coordinates
void GameplayState::CreateBuzzsaw(int _x, int _y, bool _horizontal, unsigned int _returnDistance)
{
	BuzzSaw * mSaw = new BuzzSaw();
	mSaw->SetPosition({ (float)_x, (float)_y });
	mSaw->SetStartPosition({ (float)_x, (float)_y });
	mSaw->SetHorizontal(_horizontal);
	mSaw->SetDistance(_returnDistance);
	m_pEntities->AddEntity(mSaw, Entity::ENT_BUZZSAW);
	mSaw->Release();
}

/////////////////////////
// CreateStalactite
// -Creates a stalactite at the given coordinates
void GameplayState::CreateStalactite(int _x, int _y, float _timer)
{
	Stalactite * mStalactite = new Stalactite();
	mStalactite->SetPosition({ (float)_x, (float)_y });
	mStalactite->SetRespawnTimer(_timer);
	mStalactite->SetStartPosition({ (float)_x, (float)_y });
	m_pEntities->AddEntity(mStalactite, Entity::ENT_STALACTITE);
	mStalactite->Release();
}

/////////////////////////
// CreateArmor
// -Creates an armor at the given coordinates
void GameplayState::CreateArmor(int _x, int _y)
{
	Armor * mArmor = new Armor();
	mArmor->SetPosition({ (float)_x, (float)_y });
	m_pEntities->AddEntity(mArmor, Entity::ENT_ARMOR);
	mArmor->Release();
}

/////////////////////////
// CreateFreezableGround
// -Creates a freezable ground at the given coordinates
void GameplayState::CreateFreezableGround(int _x, int _y)
{
	FreezeableGround* pFreeze = new FreezeableGround;

	//// TYPE STATE WILL CHANGE ( Needs to be done through TYPE with enttiyManger) Two types for each frozen tile === It's Current state and it's Bucket type (temp or perm tile)
	pFreeze->SetType(Entity::ENT_NOT_FROZEN);

	pFreeze->SetSize(SGD::Size(32, 32));
	pFreeze->SetPosition(SGD::Point((float)_x, (float)_y));
	pFreeze->SetIsFrozen(false);
	pFreeze->SetIsPerm(false);

	////// TYPE OF TILE Bucket
	m_pEntities->AddEntity(pFreeze, Entity::ENT_TEMP_FREEZE);

	pFreeze->Release();
}

/////////////////////////
// CreateFreezableLeftRamp
// -Creates a freezable left ramp at the given coordinates
void GameplayState::CreateFreezableLeftRamp(int _x, int _y)
{
	// TODO
}

/////////////////////////
// CreateFreezableRightRamp
// -Creates a freezable right ramp at the given coordinates
void GameplayState::CreateFreezableRightRamp(int _x, int _y)
{
	// TODO
}

/////////////////////////
// CreateHintStatue
// -Creates a hint statue at the given coordinates
void GameplayState::CreateHintStatue(int _x, int _y, std::string _message)
{
	HintStatue * mStatue = new HintStatue();
	mStatue->SetPosition({ (float)_x, (float)_y });
	mStatue->SetMessageString(_message);
	mStatue->SetMessageSize(_message.size()); // TODO
	m_pEntities->AddEntity(mStatue, Entity::ENT_STATUE);
	mStatue->Release();
}

/////////////////////////
// CreateTeleporter
// -Creates a teleporter at the given coordinates
void GameplayState::CreateTeleporter(int _x, int _y, std::string _level, bool _hub)
{
	Teleporter * mTeleporter = new Teleporter();
	mTeleporter->SetPosition({ (float)_x, (float)_y });
	mTeleporter->SetSize({ 64.0f, 64.0f });
	mTeleporter->SetLevel(_level);
	mTeleporter->SetHubTeleporter(_hub);
	m_pEntities->AddEntity(mTeleporter, Entity::ENT_TELEPORTER);
	mTeleporter->Release();
}

/////////////////////////
// CreateEnemy
// -Creates an enemy at the given coordinates
void GameplayState::CreateEnemy(int _x, int _y, int _type)
{
	switch (_type)
	{
		case 0: // bull
		{
			BullEnemy * pBull = new BullEnemy();
			pBull->SetPosition({ (float)_x, (float)_y });
			pBull->SetOriginalPosition({ (float)_x, (float)_y });
			pBull->SetPlayer(m_pPlayer);
			m_pEntities->AddEntity(pBull, Entity::ENT_BULL_ENEMY);
			pBull->Release();
			break;
		}
		case 1: // skeleton
		{
			Skeleton * pSkeleton = new Skeleton();
			pSkeleton->SetPosition({ (float)_x, (float)_y });
			pSkeleton->SetOriginalPosition({ (float)_x, (float)_y });
			pSkeleton->SetPlayer(m_pPlayer);
			m_pEntities->AddEntity(pSkeleton, Entity::ENT_SKELETON);
			pSkeleton->Release();
			break;
		}
		case 2: // mutant man
		{
			MutantMan * pMutant = new MutantMan();
			pMutant->SetPosition({ (float)_x, (float)_y });
			pMutant->SetOriginalPosition({ (float)_x, (float)_y });
			pMutant->Begin({ (float)_x, (float)_y });
			pMutant->SetPlayer(m_pPlayer);
			m_pEntities->AddEntity(pMutant, Entity::ENT_MUTANT_MAN);
			pMutant->Release();
			break;
		}
		case 3: // mutant bird
		{
			MutantBat * pMutant = new MutantBat();
			pMutant->SetPosition({ (float)_x, (float)_y });
			pMutant->SetOriginalPosition({ (float)_x, (float)_y });
			pMutant->Begin({ (float)_x, (float)_y });
			pMutant->SetPlayer(m_pPlayer);
			m_pEntities->AddEntity(pMutant, Entity::ENT_MUTANT_BIRD);
			pMutant->Release();
			break;
		}
		case 4: // ice golem
		{

			IceGolem * pIceGolem = new IceGolem();
			pIceGolem->SetPosition({ (float)_x, (float)_y });
			pIceGolem->SetOriginalPosition({ (float)_x, (float)_y });
			pIceGolem->SetPlayer(m_pPlayer);
			pIceGolem->SetDirection(2);
			m_pEntities->AddEntity(pIceGolem, Entity::ENT_ICE_GOLEM);
			pIceGolem->Release();
			break;
		}
		case 5: // ice bat
		{
			IceBat * pIceBat = new IceBat();
			pIceBat->SetPosition({ (float)_x, (float)_y });
			pIceBat->SetOriginalPosition({ (float)_x, (float)_y });
			pIceBat->SetPlayer(m_pPlayer);
			pIceBat->SetDirection(2);
			m_pEntities->AddEntity(pIceBat, Entity::ENT_ICE_BAT);
			pIceBat->Release();
			break;
		}
		case 6: // ice turtle
		{
			IceTurtle * pIceTurtle = new IceTurtle();
			pIceTurtle->SetPosition({ (float)_x, (float)_y });
			pIceTurtle->SetOriginalPosition({ (float)_x, (float)_y });
			pIceTurtle->SetPlayer(m_pPlayer);
			m_pEntities->AddEntity(pIceTurtle, Entity::ENT_ICE_TURTLE);
			pIceTurtle->Release();
			pIceTurtle = nullptr;
			break;
		}
		case 7: // hermit crab
		{
			Pouncer * pPouncer = new Pouncer();
			pPouncer->SetPosition({ (float)_x, (float)_y });
			pPouncer->SetOriginalPosition({ (float)_x, (float)_y });
			pPouncer->SetPlayer(m_pPlayer);
			m_pEntities->AddEntity(pPouncer, Entity::ENT_POUNCER);
			pPouncer->Release();
			break;
		}
		case 8: // squid
		{
			Squid * pSquid = new Squid();
			pSquid->SetPosition({ (float)_x, (float)_y });
			pSquid->SetOriginalPosition({ (float)_x, (float)_y });
			pSquid->SetPlayer(m_pPlayer);
			m_pEntities->AddEntity(pSquid, Entity::ENT_ENEMY);
			pSquid->Release();
			break;
		}
		case 9: // jellyfish
		{
			Jellyfish * pJelly = new Jellyfish();
			pJelly->SetPosition({ (float)_x, (float)_y });
			pJelly->SetOriginalPosition({ (float)_x, (float)_y });
			pJelly->SetPlayer(m_pPlayer);
			pJelly->SetPatrol();
			m_pEntities->AddEntity(pJelly, Entity::ENT_ENEMY);
			pJelly->Release();
			break;
		}
		case 10: //Mini-Wizard (for final boss fight)
		{


		}
	}
	//case 1: // skeleton
	//{
	//			break;
	//}
	//case 2: // mutant man
	//{
	//			MutantMan * pMutant = new MutantMan();
	//			pMutant->SetPosition({ (float)_x, (float)_y });
	//			pMutant->Begin({ (float)_x, (float)_y });
	//			pMutant->SetPlayer(m_pPlayer);
	//			m_pEntities->AddEntity(pMutant, Entity::ENT_MUTANT_MAN);
	//			pMutant->Release();
	//			break;
	//}
	//case 3: // mutant bird
	//{

	//			break;
	//}
	//case 4: // ice golem
	//{
	//			break;
	//}
	//case 5: // ice bat
	//{
	//			break;
	//}
	//case 6: // ice turtle
	//{
	//			break;
	//}
	//case 7: // hermit crab
	//{
	//			Pouncer * pPouncer = new Pouncer();
	//			pPouncer->SetPosition({ (float)_x, (float)_y });
	//			pPouncer->SetPlayer(m_pPlayer);
	//			m_pEntities->AddEntity(pPouncer, Entity::ENT_POUNCER);
	//			pPouncer->Release();
	//			break;
	//}
	//case 8: // squid
	//{
	//			Squid * pSquid = new Squid();
	//			pSquid->SetPosition({ (float)_x, (float)_y });
	//			pSquid->SetPlayer(m_pPlayer);
	//			m_pEntities->AddEntity(pSquid, Entity::ENT_ENEMY);
	//			pSquid->Release();
	//			break;
	//}
	//case 9: // jellyfish
	//{
	//			Jellyfish * pJelly = new Jellyfish();
	//			pJelly->SetPosition({ (float)_x, (float)_y });
	//			pJelly->SetPlayer(m_pPlayer);
	//			pJelly->SetPatrol();
	//			m_pEntities->AddEntity(pJelly, Entity::ENT_ENEMY);
	//			pJelly->Release();
	//			break;
	//}
	//}
}

/////////////////////////
// CreateBoss
// -Creates a boss at the given coordinates
void GameplayState::CreateBoss(int _x, int _y, int _type)
{
	switch (_type)
	{
		case 0: // bull
		{
			Bull * pBull = new Bull();
			pBull->SetPosition({ (float)_x, (float)_y });
			pBull->SetStartPosition({ (float)_x, (float)_y });
			pBull->SetPlayer(m_pPlayer);
			m_pEntities->AddEntity(pBull, Entity::ENT_BOSS_BULL);
			pBull->Release();
			break;
		}
		case 1: // caveman
		{
			Caveman* Temp = new Caveman();
			Temp->SetPosition({ (float)_x, (float)_y });
			Temp->SetStartPosition({ (float)_x, (float)_y });
			Temp->SetPlayer(m_pPlayer);
			m_pEntities->AddEntity(Temp, Entity::ENT_BOSS_CAVEMAN);
			Temp->Release();
			break;
		}
		case 2: // yeti
		{

			Yeti * pYeti = new Yeti();
			pYeti->SetPosition({ (float)_x, (float)_y });
			pYeti->SetStartPosition({ (float)_x, (float)_y });
			pYeti->SetPlayer(m_pPlayer);
			m_pEntities->AddEntity(pYeti, Entity::ENT_BOSS_YETI);
			pYeti->Release();
			break;
		}
		case 3: // crab
		{
			Crab * mCrab = new Crab();
			mCrab->SetPosition({ (float)_x, (float)_y });
			m_pEntities->AddEntity(mCrab, Entity::ENT_BOSS_CRAB);
			mCrab->Release();
			break;
		}
		case 4: // wizard
		{
			WizardDash* m_pDash1 = new WizardDash;
			//	m_pDash1->SetPosition({ 100, 400 });
			m_pDash1->SetPosition({ -200, -200 });
			m_pDash1->SetFacingRight(true);


			WizardDash* m_pDash2 = new WizardDash;
			//m_pDash2->SetPosition({ 400, 400 });
			m_pDash2->SetPosition({ -200, -200 });
			m_pDash2->SetFacingRight(false);


			WizardDash* m_pDash3 = new WizardDash;
			//m_pDash3->SetPosition({ 100, 700 });
			m_pDash3->SetPosition({ -200, -200 });
			m_pDash3->SetFacingRight(true);


			WizardDash* m_pDash4 = new WizardDash;
			//m_pDash4->SetPosition({ 400, 700 });
			m_pDash4->SetPosition({ -200, -200 });
			m_pDash4->SetFacingRight(false);




			Wizard* m_pWizard = new Wizard;
			m_pWizard->SetPosition({ (float)_x, (float)_y });
			m_pWizard->SetStartPosition({ (float)_x, (float)_y });


			WizardHawk* m_pHawk1 = new WizardHawk;
			m_pHawk1->SetPosition({ -300, -300 });
			m_pHawk1->SetSize({ 1, 1 });

			WizardHawk* m_pHawk2 = new WizardHawk;
			m_pHawk2->SetPosition({ -300, -300 });
			m_pHawk2->SetSize({ 1, 1 });


			WizardHawk* m_pHawk3 = new WizardHawk;
			m_pHawk3->SetPosition({ -300, -300 });
			m_pHawk3->SetSize({ 1, 1 });


			WizardHawk* m_pHawk4 = new WizardHawk;
			m_pHawk4->SetPosition({ -300, -300 });
			m_pHawk4->SetSize({ 1, 1 });


			m_pWizard->SetPlayer(m_pPlayer);

			//Dashptrs
			m_pWizard->SetDash1(m_pDash1);
			m_pWizard->SetDash2(m_pDash2);
			m_pWizard->SetDash3(m_pDash3);
			m_pWizard->SetDash4(m_pDash4);

			//Hawkptrs
			m_pWizard->SetHawk1(m_pHawk1);
			m_pWizard->SetHawk2(m_pHawk2);
			m_pWizard->SetHawk3(m_pHawk3);
			m_pWizard->SetHawk4(m_pHawk4);


			m_pEntities->AddEntity(m_pDash1, Entity::ENT_WIZARD_DASH);
			m_pEntities->AddEntity(m_pDash2, Entity::ENT_WIZARD_DASH);
			m_pEntities->AddEntity(m_pDash3, Entity::ENT_WIZARD_DASH);
			m_pEntities->AddEntity(m_pDash4, Entity::ENT_WIZARD_DASH);

			m_pEntities->AddEntity(m_pHawk1, Entity::ENT_WIZARD_HAWK);
			m_pEntities->AddEntity(m_pHawk2, Entity::ENT_WIZARD_HAWK);
			m_pEntities->AddEntity(m_pHawk3, Entity::ENT_WIZARD_HAWK);
			m_pEntities->AddEntity(m_pHawk4, Entity::ENT_WIZARD_HAWK);

			m_pEntities->AddEntity(m_pWizard, Entity::ENT_BOSS_WIZARD);
			m_pWizard->Release();


			m_pDash1->Release();
			m_pDash2->Release();
			m_pDash3->Release();
			m_pDash4->Release();
			m_pHawk1->Release();
			m_pHawk2->Release();
			m_pHawk3->Release();
			m_pHawk4->Release();



			break;
		}
	}



}


/////////////////////////
// CreateTeleporter
// -Creates a teleporter at the given coordinates
void GameplayState::CreateBossTeleporter(int _x, int _y, std::string _level, unsigned int _honor)
{
	BossDoor * mDoor = new BossDoor();
	mDoor->SetPosition({ (float)_x, (float)_y });
	mDoor->SetSize({ 64.0f, 64.0f });
	mDoor->SetLevel(_level);
	mDoor->SetRequiredHonor(_honor);
	m_pEntities->AddEntity(mDoor, Entity::ENT_BOSS_DOOR);
	mDoor->Release();
}

#pragma endregion

void GameplayState::SaveGame()
{
	HRESULT hr;
	std::ostringstream stringstream;
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t   size;

	// Get the path to the app data folder
	hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &wszPath);

	// Convert from LPWSTR to char[]
	wcstombs_s(&size, path, MAX_PATH, wszPath, MAX_PATH);

	// Convert char types
	if (hr == S_OK)
		stringstream << path;
	std::string pathtowrite = stringstream.str();

	// Add the company and game information
	pathtowrite += "\\honor\\";

	// Create our directory
	SHCreateDirectoryEx(NULL, pathtowrite.c_str(), 0);

	// Create our save file
	pathtowrite += "\\savefile";

	if (Game::GetInstance()->GetSelectedNumber() == 1)
	{
		pathtowrite += "1.xml";
	}
	else if (Game::GetInstance()->GetSelectedNumber() == 2)
	{
		pathtowrite += "2.xml";
	}
	else if (Game::GetInstance()->GetSelectedNumber() == 3)
	{
		pathtowrite += "3.xml";
	}



	//Create the doc
	TiXmlDocument doc;

	//Create the declaraction
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	std::time_t rawtime;
	std::tm* timeinfo = new tm();
	char buffer[80];

	std::time(&rawtime);
	localtime_s(timeinfo, &rawtime);

	std::strftime(buffer, 80, "%m/%d/%y %H:%M ", timeinfo);
	std::puts(buffer);


	delete timeinfo;
	// Create the root node
	TiXmlElement* Root = new TiXmlElement("Levels");
	int totalHonor = m_pPlayer->GetHonorCollected();
	Root->SetAttribute("totalHonor", totalHonor);
	Root->SetAttribute("time", buffer);
	doc.LinkEndChild(Root);

	// Loop through each level
	typedef std::map<std::string, std::vector<bool>>::iterator it_type;
	for (it_type iter = m_mCollectedHonor.begin(); iter != m_mCollectedHonor.end(); iter++)
	{
		// Grab the key and value
		std::string key = iter->first;
		std::vector<bool> value = iter->second;

		// Create a node for the current level
		TiXmlElement* Level = new TiXmlElement("Level");
		Level->SetAttribute("name", key.c_str());
		if (m_mUnlockedLevels[key])
		{
			Level->SetAttribute("unlocked", "1");
		}
		else
		{
			Level->SetAttribute("unlocked", "0");
		}
		Root->LinkEndChild(Level);

		// Loop through each value in the data
		for (unsigned int i = 0; i < value.size(); ++i)
		{
			TiXmlElement* Honor = new TiXmlElement("Honor");
			if (value[i])
			{
				Honor->SetAttribute("collected", "1");
			}
			else
			{
				Honor->SetAttribute("collected", "0");
			}
			Level->LinkEndChild(Honor);
		}
	}
	doc.SaveFile(pathtowrite.c_str());
}

bool GameplayState::LoadGame()
{
	HRESULT hr;
	std::ostringstream stringstream;
	char path[MAX_PATH];
	LPWSTR wszPath = NULL;
	size_t   size;

	// Get the path to the app data folder
	hr = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, 0, &wszPath);

	// Convert from LPWSTR to char[]
	wcstombs_s(&size, path, MAX_PATH, wszPath, MAX_PATH);

	// Convert char types
	if (hr == S_OK)
		stringstream << path;
	std::string pathtowrite = stringstream.str();

	// Add the company and game information
	pathtowrite += "\\honor\\";

	// Create our directory
	SHCreateDirectoryEx(NULL, pathtowrite.c_str(), 0);

	// Create our save file
	pathtowrite += "\\savefile";

	if (Game::GetInstance()->GetSelectedNumber() == 1)
	{
		pathtowrite += "1.xml";
	}
	else if (Game::GetInstance()->GetSelectedNumber() == 2)
	{
		pathtowrite += "2.xml";
	}
	else if (Game::GetInstance()->GetSelectedNumber() == 3)
	{
		pathtowrite += "3.xml";
	}

	//Create the doc
	TiXmlDocument doc;

	bool check = doc.LoadFile(pathtowrite.c_str());

	TiXmlElement* pRoot = doc.RootElement();
	if (pRoot == nullptr)
	{
		m_pPlayer->SetHonorCollected(0);
		return false;

	}

	// Read in total honor collected
	int totalHonor;
	pRoot->Attribute("totalHonor", &totalHonor);
	m_pPlayer->SetHonorCollected(totalHonor);

	// Loop through each level
	TiXmlElement * pLevel = pRoot->FirstChildElement();
	while (pLevel)
	{
		// Name of this level, used for the key
		std::string name = pLevel->Attribute("name");

		// If this level has been unlocked
		int unlocked;
		pLevel->Attribute("unlocked", &unlocked);
		m_mUnlockedLevels[name] = unlocked ? true : false;

		// Loop through the vector of collected honor
		TiXmlElement * pVector = pLevel->FirstChildElement();
		while (pVector)
		{
			// Read in value
			int collected;
			pVector->Attribute("collected", &collected);
			if (collected == 1)
			{
				m_mCollectedHonor[name].push_back(true);
			}
			else
			{
				m_mCollectedHonor[name].push_back(false);
			}

			// Go to the next element
			pVector = pVector->NextSiblingElement();
		}

		// Go to the next element
		pLevel = pLevel->NextSiblingElement();
	}
	//m_pPlayer->SetPosition(SGD::Point((float)x, (float)y));
	int temp = 0;
	temp++;
	return check;
}

/////////////////////////////////////////////
// LoadLevelMap
// - Reads in data to fill up the level map
void GameplayState::LoadLevelMap()
{
	//Create the tinyxml document 
	TiXmlDocument doc;

	// Load the file
	if (doc.LoadFile("Assets/Levels/LevelGuide.xml") == false)
	{
		return;
	}

	// Grab a reference to the first element
	TiXmlElement* pLevel = doc.RootElement();
	if (pLevel == nullptr)
	{
		return;
	}

	// Loop through reading each level
	while (pLevel != nullptr)
	{
		// Read in data and push into the map
		std::string key = pLevel->Attribute("key");
		std::string path = pLevel->Attribute("path");
		m_mLevels[key] = path;
		m_mCollectedHonor[key];
		m_mUnlockedLevels[key] = false;

		// Move to the next node
		pLevel = pLevel->NextSiblingElement();
	}

	// Unlock certain levels right away
	m_mUnlockedLevels["HubLevel"] = true;
	m_mUnlockedLevels["Level0_1"] = true;
	m_mUnlockedLevels["World1Level"] = true;
	m_mUnlockedLevels["World2Level"] = true;
	m_mUnlockedLevels["World3Level"] = true;
	m_mUnlockedLevels["World4Level"] = true;
	m_mUnlockedLevels["World5Level"] = true;
	m_mUnlockedLevels["Level1_1"] = true;
	m_mUnlockedLevels["Level2_4"] = true;

}

/////////////////////////////////////////////
// LoadLevel
// - Loads the level at the path for the given key
void GameplayState::LoadLevel(std::string _level)
{
	// Clear screen for loading
	SGD::Rectangle rect = SGD::Rectangle(0, 0, Game::GetInstance()->GetScreenWidth(), Game::GetInstance()->GetScreenHeight());
	SGD::GraphicsManager::GetInstance()->DrawRectangle(rect, { 255, 0, 0, 0 }, { 0, 0, 0, 0 }, 0);

	//Local refernce to the font
	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");
	font.DrawString("LOADING", 250, 200, 2.0f, { 255, 0, 0 });

	SGD::GraphicsManager::GetInstance()->Update();

	// Clear the old entities
	m_pEntities->RemoveAll();

	// Add the player back in
	m_pEntities->AddEntity(m_pPlayer, Entity::ENT_PLAYER);

	// First unload the last level
	if (m_pLevel != nullptr)
	{
		m_pLevel->Exit();
		delete m_pLevel;
		m_pLevel = nullptr;
	}

	// Save the string
	m_strCurrLevel = _level;

	// Create a new level and load the correct file
	m_pLevel = new Level();
	m_pLevel->LoadLevel(m_mLevels[_level].c_str());
	m_pLevel->Startup();

	// Set the players position
	m_pPlayer->SetPosition({ (float)m_pLevel->GetPlayerX(), (float)m_pLevel->GetPlayerY() });
	m_pPlayer->SetStartPosition({ (float)m_pLevel->GetPlayerX(), (float)m_pLevel->GetPlayerY() });
	m_pPlayer->SetVelocity({ 0.0f, 0.0f });



	//Play Music
	if (_level == "HubLevel")
	{
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld1))
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hWorld1);
		}
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld2))
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hWorld2);
		}
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld3))
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hWorld3);
		}
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld4))
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hWorld4);
		}
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld5))
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hWorld5);
		}
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hFinalBoss))
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hFinalBoss);
		}
	}
	if (_level == "Level1_1" || _level == "Level1_2" || _level == "Level1_3" || _level == "Level1_5")
	{
		//if(SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hHub))
		//{
		//	SGD::AudioManager::GetInstance()->StopAudio(m_hHub);
		//}


		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld1))
			SGD::AudioManager::GetInstance()->PlayAudio(m_hWorld1, true);



	}
	else if (_level == "Level2_1" || _level == "Level2_2" || _level == "Level2_3" || _level == "Level2_4" || _level == "Level2_5")
	{
		//if(SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hHub))
		//{
		//	SGD::AudioManager::GetInstance()->StopAudio(m_hHub);
		//}
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld1))
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hWorld1);
		}


		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld2))
			SGD::AudioManager::GetInstance()->PlayAudio(m_hWorld2, true);
	}
	else if (_level == "Level3_1" || _level == "Level3_2" || _level == "Level3_3" || _level == "Level3_4" || _level == "Level3_5")
	{
		//{
		//	SGD::AudioManager::GetInstance()->StopAudio(m_hHub);
		//}
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld2))
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hWorld2);
		}


		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld3))
			SGD::AudioManager::GetInstance()->PlayAudio(m_hWorld3, true);
	}
	else if (_level == "Level4_1" || _level == "Level4_2" || _level == "Level4_3" || _level == "Level4_4" || _level == "Level4_5")
	{
		//{
		//	SGD::AudioManager::GetInstance()->StopAudio(m_hHub);
		//}
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld3))
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hWorld3);
		}


		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld4))
			SGD::AudioManager::GetInstance()->PlayAudio(m_hWorld4, true);
	}
	else if (_level == "Level5_1" || _level == "Level5_2" || _level == "Level5_3" || _level == "Level5_4")
	{
		//{
		//	SGD::AudioManager::GetInstance()->StopAudio(m_hHub);
		//}
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld4))
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hWorld4);
		}


		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld5))
			SGD::AudioManager::GetInstance()->PlayAudio(m_hWorld5, true);
	}
	else if (_level == "Level5_5")
	{
		//{
		//	SGD::AudioManager::GetInstance()->StopAudio(m_hHub);
		//}
		if (SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hWorld5))
		{
			SGD::AudioManager::GetInstance()->StopAudio(m_hWorld5);
		}


		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hFinalBoss))
			SGD::AudioManager::GetInstance()->PlayAudio(m_hFinalBoss, true);
	}

	// TODO anything else to reset the player
}

/////////////////////////////////////////////
// SetHonorVector
// -Sets the honor vector in the honor map to the given value
void GameplayState::SetHonorVector(std::vector<bool> _value)
{
	m_mCollectedHonor[m_strCurrLevel] = _value;
}

/////////////////////////////////////////////
// SaveHonorVector
// -Saves the data in the honor vector to a file
void GameplayState::SaveHonorVector()
{
	// Setting up the xml file
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	// Create the root node
	TiXmlElement* Root = new TiXmlElement("Levels");
	int totalHonor = m_pPlayer->GetHonorCollected();
	Root->SetAttribute("totalHonor", totalHonor);
	doc.LinkEndChild(Root);

	// Loop through each level
	typedef std::map<std::string, std::vector<bool>>::iterator it_type;
	for (it_type iter = m_mCollectedHonor.begin(); iter != m_mCollectedHonor.end(); iter++)
	{
		// Grab the key and value
		std::string key = iter->first;
		std::vector<bool> value = iter->second;

		// Create a node for the current level
		TiXmlElement* Level = new TiXmlElement("Level");
		Level->SetAttribute("name", key.c_str());
		if (m_mUnlockedLevels[key])
		{
			Level->SetAttribute("unlocked", "1");
		}
		else
		{
			Level->SetAttribute("unlocked", "0");
		}
		Root->LinkEndChild(Level);

		// Loop through each value in the data
		for (unsigned int i = 0; i < value.size(); ++i)
		{
			TiXmlElement* Honor = new TiXmlElement("Honor");
			if (value[i])
			{
				Honor->SetAttribute("collected", "1");
			}
			else
			{
				Honor->SetAttribute("collected", "0");
			}
			Level->LinkEndChild(Honor);
		}
	}

	// Close the file and finish
	doc.SaveFile("Assets/Levels/CollectedHonor.xml");
}

/////////////////////////////////////////////
// LoadHonorVector
// -Loads the data in the honor vector file
void GameplayState::LoadHonorVector()
{
	//Create the tinyxml document 
	TiXmlDocument doc;

	// Load the file
	if (doc.LoadFile("Assets/Levels/CollectedHonor.xml") == false)
	{
		return;
	}

	// Grab reference to the root
	TiXmlElement* pRoot = doc.RootElement();
	if (pRoot == nullptr)
	{
		return;
	}

	// Read in total honor collected
	int totalHonor;
	pRoot->Attribute("totalHonor", &totalHonor);
	m_pPlayer->SetHonorCollected(totalHonor);

	// Loop through each level
	TiXmlElement * pLevel = pRoot->FirstChildElement();
	while (pLevel)
	{
		// Name of this level, used for the key
		std::string name = pLevel->Attribute("name");

		// If this level has been unlocked
		int unlocked;
		pLevel->Attribute("unlocked", &unlocked);
		m_mUnlockedLevels[name] = unlocked ? true : false;

		// Loop through the vector of collected honor
		TiXmlElement * pVector = pLevel->FirstChildElement();
		while (pVector)
		{
			// Read in value
			int collected;
			pVector->Attribute("collected", &collected);
			if (collected == 1)
			{
				m_mCollectedHonor[name].push_back(true);
			}
			else
			{
				m_mCollectedHonor[name].push_back(false);
			}

			// Go to the next element
			pVector = pVector->NextSiblingElement();
		}

		// Go to the next element
		pLevel = pLevel->NextSiblingElement();
	}
}

bool GameplayState::GetHonorValue(unsigned int _index)
{
	if (_index < m_mCollectedHonor[m_strCurrLevel].size())
	{
		return m_mCollectedHonor[m_strCurrLevel][_index];
	}
	return false;
}

/////////////////////////////////////////////
// GetHonorVectorSize
// -Loads the data in the honor vector file
unsigned int GameplayState::GetHonorVectorSize()
{
	return m_mCollectedHonor[m_strCurrLevel].size();
}

/////////////////////////////////////////////
// RenderMiniMap
// -Renders the mini map
void GameplayState::RenderMiniMap()
{
	// Reference to the graphics manager
	SGD::GraphicsManager * pGraphics = SGD::GraphicsManager::GetInstance();

	// Draw the backdrop
	SGD::Rectangle backDrop = SGD::Rectangle(m_fBorderSize, m_fBorderSize,
											 Game::GetInstance()->GetScreenWidth() - m_fBorderSize,
											 Game::GetInstance()->GetScreenHeight() - m_fBorderSize);
	pGraphics->DrawRectangle(backDrop, { 100, 200, 200, 200 }, { 0, 0, 0 }, 0);

	// Render the terrain onto the minimap
	m_pLevel->RenderMiniMap();

	// Render the entities onto the minimap
	m_pEntities->RenderMiniMap();
}

/////////////////////////////////////////////
// RenderMiniMap
// -Renders the mini map
bool GameplayState::GetLevelUnlocked(std::string _level)
{
	return m_mUnlockedLevels[_level];
}

/////////////////////////////////////////////
// RenderMiniMap
// -Renders the mini map
void GameplayState::UnlockLevel(std::string _level)
{
	m_mUnlockedLevels[_level] = true;
}

/////////////////////////////////////////////
// ResetHonorInRoom
// -When the player dies, reset how much honor he collected
void GameplayState::ResetHonorInRoom()
{
	unsigned int dif = m_pPlayer->GetHonorCollected() - m_pLevel->GetHonorBeforeDeath();
	m_pPlayer->SetHonorCollected(dif);
	m_pLevel->SetHonorBeforeDeath(0);
}

/////////////////////////////////////////////
// IncreaseHonorBeforeDeath
// -Add the given amount to honor before death
void GameplayState::IncreaseHonorBeforeDeath(unsigned int _value)
{
	m_pLevel->SetHonorBeforeDeath(m_pLevel->GetHonorBeforeDeath() + _value);
}


void GameplayState::WizardDefeated()
{
	LoadLevel("HubLevel");
	ending = true;
	m_pPlayer->SetPosition({ -100, -100 });
	Camera::GetInstance()->SetCameraCap(6);
}
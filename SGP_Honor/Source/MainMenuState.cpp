#include "MainMenuState.h"
#include <ShlObj.h>
#include <ostream>
#include <string>

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../TinyXML/tinyxml.h"
#include "Game.h"
#include "GameplayState.h"
#include "InstructionsState.h"
#include "CreditsState.h"
#include "OptionsState.h"
#include "ProfileState.h"
#include "Font.h"
#include "BitmapFont.h"
#include "ParticleEngine.h"
#include "Emitter.h"


///////////////////////////////////////////////////////////
/////////////////// Singleton Accessor ///////////////////

/////////////////////////////////////////////////////////
// GetInstance
// - allocate the static global instance
// - return THE instance
MainMenuState* MainMenuState::GetInstance(void)
{
	static MainMenuState s_Instance; //This is stored ONCE in global memory
	return &s_Instance; //Return the one global reference
}




//////////////////////////////////////////////////////////////
//////////////////// IGameState Interface ///////////////////



///////////////////////////////////////////////////
// Enter
// - Reset the game
// - Load the resources
// - set up entities
void MainMenuState::Enter(void) //Load Resources
{
	//Set the background color
	SGD::GraphicsManager::GetInstance()->SetClearColor({ 50, 50, 50, 50 });	// dark gray
	m_nCursor = 0;
	m_rSword.top = 250.0f;

	int nMusicVol = 50;
	int nEffectsVol = 50;

	TiXmlDocument doc;

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
	pathtowrite += "\\Options.xml";

	doc.LoadFile(pathtowrite.c_str());

	TiXmlElement* pRoot = doc.RootElement();

	if (pRoot != nullptr)
	{

		TiXmlElement* pOption = pRoot->FirstChildElement("option");

		pOption->Attribute("music_volume", &nMusicVol);

		pOption = pOption->NextSiblingElement();

		pOption->Attribute("sfx_volume", &nEffectsVol);
	}
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	rMouse = SGD::Rectangle({ pInput->GetMousePosition().x, pInput->GetMousePosition().y, pInput->GetMousePosition().x + 1, pInput->GetMousePosition().y + 1 });
	rLast = rMouse;

	m_hSelection = SGD::AudioManager::GetInstance()->LoadAudio("assets/audio/selection.wav");
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music, nMusicVol);
	SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects, nEffectsVol);
	m_hBackground = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/Honor_Castle.png");
	m_hSword = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/SwordButton.png");
	m_hButton = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/Honor_Buttons.png");

	m_fstickYOff = SGD::InputManager::GetInstance()->GetLeftJoystick(0).y;

	//Emitters Ini
	m_emSelect = ParticleEngine::GetInstance()->LoadEmitter("assets/particles/MainSelect.xml", "MainSelect", { 0, 0 });
	m_emTitle = ParticleEngine::GetInstance()->LoadEmitter("assets/particles/TitleMain.xml", "Title", { 220, 100 });

	// Music
	m_hMusic = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/MenuMusic.xwm");
	if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hMusic))
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hMusic, true);
	}
}


/////////////////////////////////////////////////
// Exit
// - Deallocate all entities
// - unload all resources
void MainMenuState::Exit(void)
{
	delete m_emSelect;
	delete m_emTitle;
	ParticleEngine::GetInstance()->Terminate();
	ParticleEngine::GetInstance()->DeleteInstance();
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hSword);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hButton);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBackground);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hSelection);
	SGD::AudioManager::GetInstance()->StopAudio(m_hMusic);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hMusic);
}


/////////////////////////////////////////////////
// Input
// - Handle the user's input
bool MainMenuState::Input(void) //Hanlde user Input
{


	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	rLast = rMouse;

	rMouse = SGD::Rectangle({ pInput->GetMousePosition().x, pInput->GetMousePosition().y, pInput->GetMousePosition().x + 1, pInput->GetMousePosition().y + 1 });




	if((pInput->IsAnyKeyPressed() || isAnyButtonPressed() ) && !pInput->IsKeyPressed(SGD::Key::MouseLeft))
	{
		m_bMouse = false;
	}
	else if(rMouse != rLast)
	{
		m_bMouse = true;
	}

	if(m_fInputTimer > .05f)
	{
		m_fstickYOff = SGD::InputManager::GetInstance()->GetLeftJoystick(0).y;
		m_fInputTimer = 0;
	}

	m_fInputTimer += .0025f;

	//Move the cursor down
	if(pInput->IsKeyPressed(SGD::Key::Down)
		|| pInput->IsDPadPressed(0, SGD::DPad::Down) || m_fstickYOff > 0)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hSelection);
		if(m_fstickYOff > 0)
		{
			m_fstickYOff = 0;
		}
		m_nCursor += 1;
		m_rSword.top += 70;
		if(m_rSword.top > 530.0f)
		{
			m_rSword.top = 530.0f;
		}

		if(m_nCursor > 4)
		{
			m_nCursor = 4;
		}
	}
	//Move the cursor up
	else if(pInput->IsKeyPressed(SGD::Key::Up)
		|| pInput->IsDPadPressed(0, SGD::DPad::Up) || m_fstickYOff < 0)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hSelection);

		if(m_fstickYOff < 0)
		{
			m_fstickYOff = 0;
		}
		m_nCursor -= 1;
		m_rSword.top -= 70;

		if(m_rSword.top < 250.0f)
		{
			m_rSword.top = 250.0f;
		}

		if(m_nCursor < 0)
		{
			m_nCursor = 0;
		}


	}

	if(m_bMouse)
	{
		if(m_rPlay.IsIntersecting(rMouse))
		{
			m_nCursor = 0;

			m_rSword.top = m_rPlay.top + 10;
			if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
			{
				Game::GetInstance()->AddState(ProfileState::GetInstance());

			}
		}
		if(m_rOptions.IsIntersecting(rMouse))
		{
			m_nCursor = 1;
			m_rSword.top = m_rOptions.top + 10;

			if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
			{
				StopAudio();

				Game::GetInstance()->AddState(OptionsState::GetInstance());

			}
		}
		if(m_rInstructions.IsIntersecting(rMouse))
		{
			m_nCursor = 2;
			m_rSword.top = m_rInstructions.top + 10;

			if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
			{
				//change state to instructions state
				Game::GetInstance()->SetSelectedProfile(4);
				Game::GetInstance()->AddState(GameplayState::GetInstance());

			}
		}
		if(m_rCredits.IsIntersecting(rMouse))
		{
			m_nCursor = 3;
			m_rSword.top = m_rCredits.top + 10;

			if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
			{
				Game::GetInstance()->AddState(CreditsState::GetInstance());

			}
		}
		if(m_rExit.IsIntersecting(rMouse))
		{
			m_nCursor = 4;
			m_rSword.top = m_rExit.top + 10;

			if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
			{
				return false;

			}
		}
	}


	//Change between windowed and full screen modes
	if(pInput->IsKeyDown(SGD::Key::Alt) && pInput->IsKeyReleased(SGD::Key::Enter))
	{
		SGD::GraphicsManager::GetInstance()->Resize({ Game::GetInstance()->GetScreenWidth(),
			Game::GetInstance()->GetScreenHeight() }, !(Game::GetInstance()->GetWindowed()));
		Game::GetInstance()->SetWindowed(!(Game::GetInstance()->GetWindowed()));
	}
	//Select the current menu option
	else if((pInput->IsKeyPressed(SGD::Key::Enter)
		&& !(pInput->IsKeyDown(SGD::Key::Alt)))
		|| pInput->IsButtonPressed(0, 0/*button A on xbox controller*/))
	{
		if(m_nCursor == 0)
		{
			//Changestate to gameplay state
			Game::GetInstance()->AddState(ProfileState::GetInstance());
		}
		else if(m_nCursor == 1)
		{
			StopAudio();
			//Change State to options state
			Game::GetInstance()->AddState(OptionsState::GetInstance());
		}
		else if(m_nCursor == 2)
		{
			//change state to instructions state
			Game::GetInstance()->SetSelectedProfile(4);
			Game::GetInstance()->AddState(GameplayState::GetInstance());
		}
		else if(m_nCursor == 3)
		{
			//Change state to the credits state
			Game::GetInstance()->AddState(CreditsState::GetInstance());
		}
		else if(m_nCursor == 4)
		{
			//Exit the game properly
			return false;
		}
	}


	//Set to exit if escape is pressed
	if(pInput->IsKeyPressed(SGD::Key::Escape)
		|| pInput->IsButtonPressed(0, 1 /*Button B on xbox controller*/))
	{
		m_rSword.top = 530.0f;
		m_nCursor = 4;
	}

	return true;
}

//////////////////////////////////////////////
// Update
// - Update all game entities
void MainMenuState::Update(float elapsedTime)
{
	m_emSelect->Update(elapsedTime);
	m_emTitle->Update(elapsedTime);
	//Do Nothing
}

/////////////////////////////////////////////
// Render
// - Render all game entities
void MainMenuState::Render(void)
{
	//Create a local reference to the input manager for ease of use
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	float fWidth = Game::GetInstance()->GetScreenWidth();
	//Local refernce to the font
	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");

	//Draw the background
	pGraphics->DrawTexture(m_hBackground, { 0, 0 }, 0.0f, {}, {}, { 1.6f, 1.2f });

	//Emitter Renders
	m_emSelect->Render();
	m_emTitle->Render();

	//Draw the title
	font.DrawString("HONOR", 220, 100, 3, SGD::Color{ 255, 255, 130, 0 });


	pGraphics->DrawTexture(m_hSword, { m_rSword.left, m_rSword.top }, 0.0f, {}, {}, { 1.4f, 1.4f });



	//Change the highlighted options color to red dependent on the cursor
	if(m_nCursor == 0)
	{
		pGraphics->DrawRectangle(m_rPlay, { 255, 255, 255, 255 }, {}, {});
		//pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rPlay.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });

		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 240 }, 0.0f, {}, { 255, 255, 255, 255 });



		font.DrawString("Play", (int)((fWidth - (4 * 19)) / 2), 250, 1, SGD::Color{ 255, 255, 165, 0 });

	}
	else
	{
		pGraphics->DrawRectangle(m_rPlay, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 240 }, 0.0f, {}, { 255, 255, 255, 255 });

		//pGraphics->DrawTexture(m_hSword, { m_rPlay.left - 50, m_rPlay.top }, 0.0f, {}, {}, { 1.4f, 1.4f });

		font.DrawString("Play", (int)((fWidth - (4 * 19)) / 2), 250, 1, SGD::Color{ 255, 255, 165, 0 });
	}


	if(m_nCursor == 1)
	{
		pGraphics->DrawRectangle(m_rOptions, { 255, 255, 255, 255 }, {}, {});
		//pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rOptions.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 310 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Options", (int)((fWidth - (7 * 15)) / 2), 320, 1, SGD::Color{ 255, 255, 165, 0 });
	}
	else
	{
		pGraphics->DrawRectangle(m_rOptions, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 310 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Options", (int)((fWidth - (7 * 15)) / 2), 320, 1, SGD::Color{ 255, 255, 165, 0 });

	}


	if(m_nCursor == 2)
	{
		pGraphics->DrawRectangle(m_rInstructions, { 255, 255, 255, 255 }, {}, {});
		//pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rInstructions.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 380 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Tutorial", (int)((fWidth - (12 * 14)) / 1.85f), 390, 1, SGD::Color{ 255, 255, 165, 0 });
	}
	else
	{
		pGraphics->DrawRectangle(m_rInstructions, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 380 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Tutorial", (int)((fWidth - (12 * 14.0f)) / 1.85), 390, 1, SGD::Color{ 255, 255, 165, 0 });

	}


	if(m_nCursor == 3)
	{
		pGraphics->DrawRectangle(m_rCredits, { 255, 255, 255, 255 }, {}, {});
		//pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rCredits.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 450 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Credits", (int)((fWidth - (7 * 14)) / 2), 460, 1, SGD::Color{ 255, 255, 165, 0 });
	}
	else
	{
		pGraphics->DrawRectangle(m_rCredits, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 450 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Credits", (int)((fWidth - (7 * 14)) / 2), 460, 1, SGD::Color{ 255, 255, 165, 0 });

	}


	if(m_nCursor == 4)
	{
		pGraphics->DrawRectangle(m_rExit, { 255, 255, 255, 255 }, {}, {});
		//pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rExit.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 520 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Exit", (int)((fWidth - (4 * 18)) / 2), 530, 1, SGD::Color{ 255, 255, 165, 0 });
	}
	else
	{
		pGraphics->DrawRectangle(m_rExit, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 520 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Exit", (int)((fWidth - (4 * 18)) / 2), 530, 1, SGD::Color{ 255, 255, 165, 0 });

		//font.DrawString("Exit", 450, 450, 1, SGD::Color{ 255, 0, 0, 255 });
	}



}

bool MainMenuState::isAnyButtonPressed()
{

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	//Check for controller input
	if(pInput->IsButtonPressed(0, 0) ||
		pInput->IsButtonPressed(0, 1) ||
		pInput->IsButtonPressed(0, 2) ||
		pInput->IsButtonPressed(0, 3) ||
		pInput->IsButtonPressed(0, 4) ||
		pInput->IsButtonPressed(0, 5) ||
		pInput->IsButtonPressed(0, 6) ||
		pInput->IsButtonPressed(0, 7) ||
		pInput->IsButtonPressed(0, 8) ||
		pInput->IsButtonPressed(0, 9) ||
		pInput->IsButtonPressed(0, 10) ||
		pInput->IsButtonPressed(0, 11) ||
		pInput->IsDPadPressed(0, SGD::DPad::Up) ||
		pInput->IsDPadPressed(0, SGD::DPad::Left) ||
		pInput->IsDPadPressed(0, SGD::DPad::Right) ||
		pInput->IsDPadPressed(0, SGD::DPad::Down) ||
		pInput->GetLeftJoystick(0).x < -0.2 ||
		pInput->GetLeftJoystick(0).x > 0.2 ||
		pInput->GetRightJoystick(0).x < -0.2 ||
		pInput->GetRightJoystick(0).x > 0.2 ||
		pInput->GetLeftJoystick(0).y < -0.2 ||
		pInput->GetLeftJoystick(0).y > 0.2 ||
		pInput->GetRightJoystick(0).y < -0.2 ||
		pInput->GetRightJoystick(0).y > 0.2 ||
		pInput->GetTrigger(0) > 0 ||
		pInput->GetTrigger(0) < 0
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

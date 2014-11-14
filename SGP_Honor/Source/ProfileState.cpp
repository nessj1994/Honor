#include "ProfileState.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "CutSceneState.h"
#include <map>
#include <ShlObj.h>
#include "../TinyXML/tinyxml.h"
#include <sstream>
#include "Game.h"
#include "GameplayState.h"
#include "Font.h"
#include "BitmapFont.h"
#include "Profile.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
///////////////////////////////////////////////////////////
///////////////////// Singleton Accessor /////////////////

/////////////////////////////////////////////////////////
// GetInstance
// - allocate the static global instance
// - return THE instance
ProfileState* ProfileState::GetInstance(void)
{
	static ProfileState s_Instance; //This is stored ONCE in global memory
	return &s_Instance;
}




//////////////////////////////////////////////////////////////
//////////////////// IGameState Interface ///////////////////



///////////////////////////////////////////////////
// Enter
// - Reset the game
// - Load the resources
// - set up entities
void ProfileState::Enter(void) //Load Resources
{
	m_hSelection = SGD::AudioManager::GetInstance()->LoadAudio("assets/audio/selection.wav");

	m_hBackground = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/Honor_Castle.png");
	m_hSword = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/SwordButton.png");
	m_hButton = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/Honor_Buttons.png");
	m_hEsc = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/esc.png");
	m_hCircle = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/circle.png");

	LoadProfile(Game::GetInstance()->GetProfile(1));
	LoadProfile(Game::GetInstance()->GetProfile(2));
	LoadProfile(Game::GetInstance()->GetProfile(3));


}


/////////////////////////////////////////////////
// Exit
// - Deallocate all entities
// - unload all resources
void ProfileState::Exit(void)
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hSelection);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hSword);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hButton);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBackground);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hEsc);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hCircle);
}


/////////////////////////////////////////////////
// Input
// - Handle the user's input
bool ProfileState::Input(void) //Hanlde user Input
{



	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	SGD::Rectangle rMouse = SGD::Rectangle({ pInput->GetMousePosition().x, pInput->GetMousePosition().y, pInput->GetMousePosition().x + 1, pInput->GetMousePosition().y + 1 });

	//move option selection cursor right
	if(pInput->IsKeyPressed(SGD::Key::Right) || pInput->IsDPadPressed(0, SGD::DPad::Right))
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hSelection);
		m_nOptionCursor += 1;
		if(m_nOptionCursor > 1)
		{
			m_nOptionCursor = 1;
		}
	}
	if(pInput->IsKeyPressed(SGD::Key::Left) || pInput->IsDPadPressed(0, SGD::DPad::Left))
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hSelection);

		m_nOptionCursor -= 1;
		if(m_nOptionCursor < 0)
		{
			m_nOptionCursor = 0;
		}
	}

	//Move the cursor down
	if(pInput->IsKeyPressed(SGD::Key::Down)
		|| pInput->IsDPadPressed(0, SGD::DPad::Down))
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hSelection);

		m_nCursor += 1;
		m_rSword.top += 70;
		if(m_rSword.top > 280.0f)
		{
			m_rSword.top = 280.0f;
		}

		if(m_nCursor > 2)
		{
			m_nCursor = 2;
		}
	}
	//Move the cursor up
	else if(pInput->IsKeyPressed(SGD::Key::Up)
		|| pInput->IsDPadPressed(0, SGD::DPad::Up))
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hSelection);

		m_nCursor -= 1;
		m_rSword.top -= 70;

		if(m_nCursor < 0)
		{
			m_nCursor = 0;
		}

		if(m_rSword.top < 140.0f)
		{
			m_rSword.top = 140.0f;
		}




	}

	if(m_rProfile1.IsIntersecting(rMouse))
	{
		m_nCursor = 0;
		Game::GetInstance()->SetSelectedProfile(1);

		m_rSword.top = m_rProfile1.top + 10;
		if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{

			Game::GetInstance()->AddState(GameplayState::GetInstance());

		}
	}
	if(m_rProfile2.IsIntersecting(rMouse))
	{
		m_nCursor = 1;
		m_rSword.top = m_rProfile2.top + 10;
		Game::GetInstance()->SetSelectedProfile(2);

		if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{
			Game::GetInstance()->AddState(GameplayState::GetInstance());

			//Game::GetInstance()->AddState(OptionsState::GetInstance());

		}
	}
	if(m_rProfile3.IsIntersecting(rMouse))
	{
		Game::GetInstance()->SetSelectedProfile(3);

		m_nCursor = 2;
		m_rSword.top = m_rProfile3.top + 10;

		if(pInput->IsKeyPressed(SGD::Key::MouseLeft))
		{
			Game::GetInstance()->AddState(GameplayState::GetInstance());

			//Game::GetInstance()->AddState(InstructionsState::GetInstance());

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
			Game::GetInstance()->SetSelectedProfile(1);
			//Game::GetInstance()->AddState(GameplayState::GetInstance());

			// Go into the opening cutscene if it is a new profile

		}
		else if(m_nCursor == 1)
		{
			Game::GetInstance()->SetSelectedProfile(2);

			//Game::GetInstance()->AddState(GameplayState::GetInstance());

			//Change State to options state
			//Game::GetInstance()->AddState(OptionsState::GetInstance());
		}
		else if(m_nCursor == 2)
		{
			Game::GetInstance()->SetSelectedProfile(3);

			//Game::GetInstance()->AddState(GameplayState::GetInstance());

			//change state to instructions state
			//Game::GetInstance()->AddState(InstructionsState::GetInstance());
		}
		else if(m_nCursor == 3)
		{
			Game::GetInstance()->RemoveState();
		}

		if(m_nOptionCursor == 0)
		{
			Game::GetInstance()->SetSelectedProfile(m_nCursor + 1);
			Game::GetInstance()->AddState(GameplayState::GetInstance());
			CutSceneState::GetInstance()->SetCutScenePath("Assets/CutScenes/Intro.xml");
			Game::GetInstance()->AddState(CutSceneState::GetInstance());

		}
		else if(m_nOptionCursor == 1)
		{

			DeleteProfile(Game::GetInstance()->GetProfile(Game::GetInstance()->GetSelectedNumber()));

			LoadProfile(Game::GetInstance()->GetProfile(1));
			LoadProfile(Game::GetInstance()->GetProfile(2));
			LoadProfile(Game::GetInstance()->GetProfile(3));
		}

	}

	//Set to exit if escape is pressed
	if(pInput->IsKeyPressed(SGD::Key::Escape)
		|| pInput->IsButtonPressed(0, 1 /*Button B on xbox controller*/))
	{
		Game::GetInstance()->RemoveState();
	}

	return true;
}

//////////////////////////////////////////////
// Update
// - Update all game entities
void ProfileState::Update(float elapsedTime)
{

}

/////////////////////////////////////////////
// Render
// - Render all game entities
void ProfileState::Render(void)
{

	//Create a local reference to the input manager for ease of use
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	float fWidth = Game::GetInstance()->GetScreenWidth();
	//Local refernce to the font
	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");

	//Draw the background
	pGraphics->DrawTexture(m_hBackground, { 0, 0 }, 0.0f, {}, {}, { 1.6f, 1.2f });

	//Draw the title
	font.DrawString("HONOR", 220, 10, 3, SGD::Color{ 255, 255, 130, 0 });


	pGraphics->DrawTexture(m_hSword, { m_rSword.left, m_rSword.top }, 0.0f, {}, {}, { 1.4f, 1.4f });



	//Change the highlighted options color to red dependent on the cursor
	if(m_nCursor == 0)
	{
		pGraphics->DrawRectangle(m_rProfile1, { 255, 255, 255, 255 }, {}, {});
		//pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rPlay.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });

		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 4, Game::GetInstance()->GetScreenHeight() / 2 - 170 }, 0.0f, {}, { 255, 255, 255, 255 });

		std::string time;
		time = Game::GetInstance()->GetProfile(m_nCursor + 1)->GetTime();


		font.DrawString("Profile 1", (int)((fWidth - (4 * 19)) / 3.65f), (int)Game::GetInstance()->GetScreenHeight() / 2 - 160, 1, SGD::Color{ 255, 255, 165, 0 });

		std::ostringstream stringstream;
		stringstream << Game::GetInstance()->GetProfile(m_nCursor + 1)->GetCollectedHonor();

		font.DrawString("Current Level: " + Game::GetInstance()->GetProfile(m_nCursor + 1)->GetCurrentLevel(), (int)(m_rProfile1.left + 300) - 30, 220, 1, { 255, 255, 130, 0 });
		font.DrawString("Collected Honor: " + stringstream.str(), (int)(m_rProfile1.left + 300) - 30, 252, 1, { 255, 255, 130, 0 });
		font.DrawString("Time Created:" + time, (int)(m_rProfile1.left + 300) - 30, 284, 1, { 255, 255, 130, 0 });

		//font.DrawString(Game::GetInstance()->GetProfile(m_nCursor + 1)->GetCurrentLevel().c_str(), m_rProfile1.left + 400, 220, 1, { 255, 255, 0, 0 });


	}
	else
	{
		pGraphics->DrawRectangle(m_rProfile1, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 4, Game::GetInstance()->GetScreenHeight() / 2 - 170 }, 0.0f, {}, { 255, 255, 255, 255 });


		//pGraphics->DrawTexture(m_hSword, { m_rPlay.left - 50, m_rPlay.top }, 0.0f, {}, {}, { 1.4f, 1.4f });

		font.DrawString("Profile 1", (int)((fWidth - (4 * 19)) / 3.65f), (int)Game::GetInstance()->GetScreenHeight() / 2 - 160, 1, SGD::Color{ 255, 255, 165, 0 });
	}


	if(m_nCursor == 1)
	{
		pGraphics->DrawRectangle(m_rProfile2, { 255, 255, 255, 255 }, {}, {});
		//pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rOptions.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 4, Game::GetInstance()->GetScreenHeight() / 2 - 100 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Profile 2", (int)((fWidth - (4 * 19)) / 3.65f), (int)Game::GetInstance()->GetScreenHeight() / 2 - 90, 1, SGD::Color{ 255, 255, 165, 0 });
		std::string time = Game::GetInstance()->GetProfile(m_nCursor + 1)->GetTime();


		std::ostringstream stringstream;
		stringstream << Game::GetInstance()->GetProfile(m_nCursor + 1)->GetCollectedHonor();
		font.DrawString("Current Level: " + Game::GetInstance()->GetProfile(m_nCursor + 1)->GetCurrentLevel(), (int)(m_rProfile1.left + 300) - 30, 220, 1, { 255, 255, 130, 0 });
		font.DrawString("Collected Honor: " + stringstream.str(), (int)(m_rProfile1.left + 300) - 30, 252, 1, { 255, 255, 130, 0 });
		font.DrawString("Time Created:" + time, (int)(m_rProfile1.left + 300) - 30, 284, 1, { 255, 255, 130, 0 });

	}
	else
	{
		pGraphics->DrawRectangle(m_rProfile2, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 4, Game::GetInstance()->GetScreenHeight() / 2 - 100 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Profile 2", (int)((fWidth - (4 * 19)) / 3.65f), (int)Game::GetInstance()->GetScreenHeight() / 2 - 90, 1, SGD::Color{ 255, 255, 165, 0 });

	}


	if(m_nCursor == 2)
	{

		std::string time = Game::GetInstance()->GetProfile(m_nCursor + 1)->GetTime();
		pGraphics->DrawRectangle(m_rProfile3, { 255, 255, 255, 255 }, {}, {});
		//pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rInstructions.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 4, Game::GetInstance()->GetScreenHeight() / 2 - 30 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Profile 3", (int)((fWidth - (4 * 19)) / 3.65f), (int)Game::GetInstance()->GetScreenHeight() / 2 - 20, 1, SGD::Color{ 255, 255, 165, 0 });

		std::ostringstream stringstream;
		stringstream << Game::GetInstance()->GetProfile(m_nCursor + 1)->GetCollectedHonor();
		font.DrawString("Current Level: " + Game::GetInstance()->GetProfile(m_nCursor + 1)->GetCurrentLevel(), (int)(m_rProfile1.left + 300) - 30, 220, 1, { 255, 255, 130, 0 });
		font.DrawString("Collected Honor: " + stringstream.str(), (int)(m_rProfile1.left + 300) - 30, 252, 1, { 255, 255, 130, 0 });
		font.DrawString("Time Created:" + time, (int)(m_rProfile1.left + 300) - 30, 284, 1, { 255, 255, 130, 0 });

	}
	else
	{
		pGraphics->DrawRectangle(m_rProfile3, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 4, Game::GetInstance()->GetScreenHeight() / 2 - 30 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Profile 3", (int)((fWidth - (4 * 19)) / 3.65f), (int)Game::GetInstance()->GetScreenHeight() / 2 - 20, 1, SGD::Color{ 255, 255, 165, 0 });

	}

	if(m_nOptionCursor == 0)
	{
		pGraphics->DrawRectangle(m_rSelect, { 255, 255, 255, 255 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { Game::GetInstance()->GetScreenWidth() / 2 - 290, Game::GetInstance()->GetScreenHeight() / 2 + 160 }, 0.0f, {}, { 255, 255, 255, 255 });
		font.DrawString("Continue", (int)((fWidth - (4 * 19)) / 4.25f), (int)Game::GetInstance()->GetScreenHeight() / 2 + 170, 1, SGD::Color{ 255, 255, 165, 0 });

	}
	else
	{
		pGraphics->DrawRectangle(m_rSelect, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { Game::GetInstance()->GetScreenWidth() / 2 - 290, Game::GetInstance()->GetScreenHeight() / 2 + 160 }, 0.0f, {}, { 255, 255, 255, 255 });
		font.DrawString("Continue", (int)((fWidth - (4 * 19)) / 4.25f), (int)Game::GetInstance()->GetScreenHeight() / 2 + 170, 1, SGD::Color{ 255, 255, 165, 0 });

	}
	if(m_nOptionCursor == 1)
	{
		pGraphics->DrawRectangle(m_rDelete, { 255, 255, 255, 255 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { Game::GetInstance()->GetScreenWidth() / 2 + 50, Game::GetInstance()->GetScreenHeight() / 2 + 160 }, 0.0f, {}, { 255, 255, 255, 255 });
		font.DrawString("Delete", (int)(Game::GetInstance()->GetScreenWidth() / 2 + 135), (int)Game::GetInstance()->GetScreenHeight() / 2 + 170, 1, SGD::Color{ 255, 255, 165, 0 });

	}
	else
	{
		pGraphics->DrawRectangle(m_rDelete, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { Game::GetInstance()->GetScreenWidth() / 2 + 50, Game::GetInstance()->GetScreenHeight() / 2 + 160 }, 0.0f, {}, { 255, 255, 255, 255 });
		font.DrawString("Delete", (int)(Game::GetInstance()->GetScreenWidth() / 2 + 135), (int)Game::GetInstance()->GetScreenHeight() / 2 + 170, 1, SGD::Color{ 255, 255, 165, 0 });

	}

	font.DrawString("Press ", 30, Game::GetInstance()->GetScreenHeight() - 50, 1.0f, { 255, 130, 0 });
	pGraphics->DrawTexture(m_hEsc, { 120, Game::GetInstance()->GetScreenHeight() - 40 }, 0.0f, {}, {}, { 0.5f, 0.5f });
	font.DrawString("or ", 162, Game::GetInstance()->GetScreenHeight() - 50, 1.0f, { 255, 130, 0 });
	pGraphics->DrawTexture(m_hCircle, { 200, Game::GetInstance()->GetScreenHeight() - 40 }, 0.0f, {}, {}, { 1.0f, 1.0f });
	font.DrawString("to go back.", 245, Game::GetInstance()->GetScreenHeight() - 50, 1.0f, { 255, 130, 0 });



}



void ProfileState::LoadProfile(Profile* profile)
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
	if(hr == S_OK)
		stringstream << path;
	std::string pathtowrite = stringstream.str();

	// Add the company and game information
	pathtowrite += "\\honor\\";

	// Create our directory
	SHCreateDirectoryEx(NULL, pathtowrite.c_str(), 0);


	// Create our save file
	pathtowrite += "\\savefile";

	if(profile == Game::GetInstance()->GetProfile(1))
	{
		pathtowrite += "1.xml";
	}
	else if(profile == Game::GetInstance()->GetProfile(2))
	{
		pathtowrite += "2.xml";
	}
	else if(profile == Game::GetInstance()->GetProfile(3))
	{
		pathtowrite += "3.xml";
	}

	//Create the doc
	TiXmlDocument doc;

	doc.LoadFile(pathtowrite.c_str());


	std::string time;

	TiXmlElement* pRoot = doc.RootElement();

	if(pRoot == nullptr)
	{
		profile->SetHonorCollected(0);
		profile->SetCurrentLevel("Level0_1");
		time = "N/A";
		profile->SetTime(time);

		return;
	}

	// Read in total honor collected
	int totalHonor;
	pRoot->Attribute("totalHonor", &totalHonor);
	profile->SetHonorCollected(totalHonor);



	if(pRoot->Attribute("time"))
	{
		time = pRoot->Attribute("time");
	}
	else
	{
		time = "N/A";
	}
	profile->SetTime(time);

	// Loop through each level
	TiXmlElement * pLevel = pRoot->FirstChildElement();

	std::string CurrentLevel;

	while(pLevel)
	{
		// Name of this level, used for the key
		std::string name = pLevel->Attribute("name");

		// If this level has been unlocked
		int unlocked;
		pLevel->Attribute("unlocked", &unlocked);


		if(unlocked ? true : false)
		{
			profile->SetCurrentLevel(name);
		}


		// Go to the next element
		pLevel = pLevel->NextSiblingElement();
	}
}

void ProfileState::DeleteProfile(Profile* profile)
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
	if(hr == S_OK)
		stringstream << path;
	std::string pathtowrite = stringstream.str();

	// Add the company and game information
	pathtowrite += "\\honor\\";

	// Create our directory
	SHCreateDirectoryEx(NULL, pathtowrite.c_str(), 0);


	// Create our save file
	pathtowrite += "\\savefile";

	if(profile == Game::GetInstance()->GetProfile(1))
	{
		pathtowrite += "1.xml";
	}
	else if(profile == Game::GetInstance()->GetProfile(2))
	{
		pathtowrite += "2.xml";
	}
	else if(profile == Game::GetInstance()->GetProfile(3))
	{
		pathtowrite += "3.xml";
	}

	remove(pathtowrite.c_str());

}
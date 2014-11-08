#include "OptionsState.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Color.h"
#include "../SGD Wrappers/SGD_String.h"

#include "MainMenuState.h"
#include "GameplayState.h"
#include "Game.h"
#include "../TinyXML/tinyxml.h"
#include "BitmapFont.h"
#include "Font.h"

///////////////////////////////////////////////////////////
///////////////////// Singleton Accessor /////////////////

/////////////////////////////////////////////////////////
// GetInstance
// - allocate the static global instance
// - return THE instance
OptionsState* OptionsState::GetInstance(void)
{
	static OptionsState s_Instance; //This is stored ONCE in global memory
	return &s_Instance;
}




//////////////////////////////////////////////////////////////
//////////////////// IGameState Interface ///////////////////



///////////////////////////////////////////////////
// Enter
// - Reset the game
// - Load the resources
// - set up entities
void OptionsState::Enter(void) //Load Resources
{
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	m_hBGM = pAudio->LoadAudio(L"Assets/Audio/HonorBGM.xwm");

	m_bFullScreen = !Game::GetInstance()->GetWindowed();
	pAudio->PlayAudio(m_hBGM);
}


/////////////////////////////////////////////////
// Exit
// - Deallocate all entities
// - unload all resources
void OptionsState::Exit(void)
{

	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	int nMusicVol = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
	int nEffectsVol = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);
	//Create the doc
	TiXmlDocument doc;

	//Create the declaraction
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	//Create the root element
	TiXmlElement* rootElement = new TiXmlElement("options");
	doc.LinkEndChild(rootElement);

	TiXmlElement* element = new TiXmlElement("option");
	rootElement->LinkEndChild(element);
	element->SetAttribute("music_volume", nMusicVol);

	TiXmlElement* element2 = new TiXmlElement("option");
	rootElement->LinkEndChild(element2);
	element2->SetAttribute("sfx_volume", nEffectsVol);

	doc.SaveFile("Assets/Options.xml");


	pAudio->StopAudio(m_hBGM);
	pAudio->UnloadAudio(m_hBGM);
}


/////////////////////////////////////////////////
// Input
// - Handle the user's input
bool OptionsState::Input(void) //Hanlde user Input
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	//Change between windowed and full screen modes
	if(pInput->IsKeyDown(SGD::Key::Alt) && pInput->IsKeyReleased(SGD::Key::Enter))
	{
		SGD::GraphicsManager::GetInstance()->Resize({ Game::GetInstance()->GetScreenWidth(),
			Game::GetInstance()->GetScreenHeight() }, !(Game::GetInstance()->GetWindowed()));
		Game::GetInstance()->SetWindowed(!(Game::GetInstance()->GetWindowed()));
	}



	if(pInput->IsKeyPressed(SGD::Key::Escape)
		|| pInput->IsButtonPressed(0, 1 /*Button B on xbox controller*/))
	{
		Game::GetInstance()->RemoveState();
	}


	if(pInput->IsKeyPressed(SGD::Key::Down)
		|| pInput->IsDPadPressed(0, SGD::DPad::Down))
	{
		m_unCursor += 1;

		if(m_unCursor > 2)
		{
			m_unCursor = 2;
		}

	}
	else if(pInput->IsKeyPressed(SGD::Key::Up)
		|| pInput->IsDPadPressed(0, SGD::DPad::Up))
	{
		m_unCursor -= 1;

		if(m_unCursor < 0)
		{
			m_unCursor = 0;
		}
	}


	//Adjust volume for music 
	if(m_unCursor == 0 /*Music volume*/ && (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Right) || SGD::InputManager::GetInstance()->IsDPadPressed(0, SGD::DPad::Right)))
	{
		if(SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music) < 100)
		{
			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music,
				SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music) + 5);
		}

	}
	else if(m_unCursor == 0 /*Music volume*/ && (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Left) || SGD::InputManager::GetInstance()->IsDPadPressed(0, SGD::DPad::Left)))
	{
		if(SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music) > 0)
		{
			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music,
				SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music) - 5);



		}


	}

	//Adjust volume for SFX 
	if(m_unCursor == 1/*SFX volume*/ && (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Right) || SGD::InputManager::GetInstance()->IsDPadPressed(0, SGD::DPad::Right)))
	{
		if(SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects) < 100)
		{
			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects,
				SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects) + 5);
		}

	}
	else if(m_unCursor == 1 /*SFX volume*/ && (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Left) || SGD::InputManager::GetInstance()->IsDPadPressed(0, SGD::DPad::Left)))
	{
		if(SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects) > 0)
		{
			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects,
				SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects) - 5);



		}


	}

	if(m_unCursor == 2 && (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter) || SGD::InputManager::GetInstance()->IsButtonPressed(0, 0)))
	{
		m_bFullScreen = !m_bFullScreen;

		SGD::GraphicsManager::GetInstance()->Resize({ Game::GetInstance()->GetScreenWidth(),
			Game::GetInstance()->GetScreenHeight() }, !m_bFullScreen);
		Game::GetInstance()->SetWindowed(m_bFullScreen);
	}



	return true;
}

//////////////////////////////////////////////
// Update
// - Update all game entities
void OptionsState::Update(float elapsedTime)
{



}

/////////////////////////////////////////////
// Render
// - Render all game entities
void OptionsState::Render(void)
{
	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");

	int nMusicVol = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
	int nEffectsVol = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);

	SGD::OStringStream ossMus;
	SGD::OStringStream ossSFX;


	ossMus << nMusicVol;
	ossSFX << nEffectsVol;




	if(m_unCursor == 0)
	{
		font.DrawString("Music Volume:", 450, 250, 1, SGD::Color{ 255, 255, 0, 0 });
		font.DrawString(ossMus.str().c_str(), 700, 250, 1, { 255, 255, 0, 0 });
	}
	else
	{
		font.DrawString("Music Volume:", 450, 250, 1, SGD::Color{ 255, 0, 255, 0 });
		font.DrawString(ossMus.str().c_str(), 700, 250, 1, { 255, 0, 255, 0 });
	}

	if(m_unCursor == 1)
	{
		font.DrawString("Effects Volume:", 450, 282, 1, SGD::Color{ 255, 255, 0, 0 });
		font.DrawString(ossSFX.str().c_str(), 700, 282, 1, { 255, 255, 0, 0 });
	}
	else
	{
		font.DrawString("Effects Volume:", 450, 282, 1, SGD::Color{ 255, 0, 255, 0 });
		font.DrawString(ossSFX.str().c_str(), 700, 282, 1, { 255, 0, 255, 0 });
	}

	if(m_unCursor == 2 && m_bFullScreen)
	{
		font.DrawString("Full Screen:", 450, 314, 1, SGD::Color{ 255, 255, 0, 0 });
		font.DrawString(("ON"), 700, 314, 1, SGD::Color{ 255, 255, 0, 0 });
	}
	else if(m_unCursor != 2 && m_bFullScreen)
	{
		font.DrawString("Full Screen:", 450, 314, 1, SGD::Color{ 255, 0, 255, 0 });
		font.DrawString(("ON"), 700, 314, 1, SGD::Color{ 255, 0, 255, 0 });
	}
	else if(m_unCursor == 2 && !m_bFullScreen)
	{
		font.DrawString("Full Screen:", 450, 314, 1, SGD::Color{ 255, 255, 0, 0 });
		font.DrawString(("OFF"), 700, 314, 1, SGD::Color{ 255, 255, 0, 0 });
	}
	else if(m_unCursor != 2 && !m_bFullScreen)
	{
		font.DrawString("Full Screen:", 450, 314, 1, SGD::Color{ 255, 0, 255, 0 });
		font.DrawString(("OFF"), 700, 314, 1, SGD::Color{ 255, 0, 255, 0 });
	}

}
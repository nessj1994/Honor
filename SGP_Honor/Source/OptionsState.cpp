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
#include "ParticleEngine.h"
#include "Emitter.h"

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
	m_emBackgroundEffect = ParticleEngine::GetInstance()->LoadEmitter("assets/particles/MainSelect.xml", "MainSelect", { 0, 0 });
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance();

	m_hSelection = SGD::AudioManager::GetInstance()->LoadAudio("assets/audio/selection.wav");

	m_hBGM = pAudio->LoadAudio(L"Assets/Audio/HonorBGM.xwm");
	m_hBackground = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/Honor_Castle.png");
	m_hSword = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/SwordButton.png");
	m_hButton = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/Honor_Buttons.png");
	m_hEsc = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/esc.png");
	m_hCircle = SGD::GraphicsManager::GetInstance()->LoadTexture("assets/graphics/circle.png");

	//m_bFullScreen = !Game::GetInstance()->GetWindowed();
	pAudio->PlayAudio(m_hBGM);

	m_fstickYOff = SGD::InputManager::GetInstance()->GetLeftJoystick(0).y;

	m_emTitle = ParticleEngine::GetInstance()->LoadEmitter("assets/particles/TitleMain.xml", "Title", { 220, -100 });
}


/////////////////////////////////////////////////
// Exit
// - Deallocate all entities
// - unload all resources
void OptionsState::Exit(void)
{
	delete m_emBackgroundEffect;
	delete m_emTitle;
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
	pAudio->StopAudio(m_hSelection);
	pAudio->UnloadAudio(m_hSelection);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hSword);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hButton);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBackground);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hEsc);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hCircle);
	
}


/////////////////////////////////////////////////
// Input
// - Handle the user's input
bool OptionsState::Input(void) //Hanlde user Input
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (m_fInputTimer > .05f)
	{
		m_fstickXoff = SGD::InputManager::GetInstance()->GetLeftJoystick(0).x;
		if (m_fstickXoff == 0)
		{
			m_fstickYOff = SGD::InputManager::GetInstance()->GetLeftJoystick(0).y;
		}		
		m_fInputTimer = 0;
	}

	m_fInputTimer += .0025f;


	//Change between windowed and full screen modes
	if(pInput->IsKeyDown(SGD::Key::Alt) && pInput->IsKeyReleased(SGD::Key::Enter))
	{
		SGD::GraphicsManager::GetInstance()->Resize({ Game::GetInstance()->GetScreenWidth(),
			Game::GetInstance()->GetScreenHeight() }, !(Game::GetInstance()->GetWindowed()));
		Game::GetInstance()->SetWindowed(!(Game::GetInstance()->GetWindowed()));

		m_bFullScreen = !m_bFullScreen;
	}



	if(pInput->IsKeyPressed(SGD::Key::Escape)
		|| pInput->IsButtonPressed(0, 1 /*Button B on xbox controller*/))
	{
		Game::GetInstance()->RemoveState();
	}


	if(pInput->IsKeyPressed(SGD::Key::Down)
		|| pInput->IsDPadPressed(0, SGD::DPad::Down) || m_fstickYOff > 0)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hSelection);

		if (m_fstickYOff > 0)
		{
			m_fstickYOff = 0;
		}
		m_unCursor += 1;

		if(m_unCursor > 2)
		{
			m_unCursor = 2;
		}

	}
	else if(pInput->IsKeyPressed(SGD::Key::Up)
		|| pInput->IsDPadPressed(0, SGD::DPad::Up) || m_fstickYOff < 0)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hSelection);

		if (m_fstickYOff < 0)
		{
			m_fstickYOff = 0;
		}
		m_unCursor -= 1;

		if(m_unCursor < 0)
		{
			m_unCursor = 0;
		}
	}


	//Adjust volume for music 
	if (m_unCursor == 0 /*Music volume*/ && (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Right) || SGD::InputManager::GetInstance()->IsDPadPressed(0, SGD::DPad::Right) || m_fstickXoff > 0))
	{
		if(SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music) < 100)
		{
			m_fstickXoff = 0;
			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music,
				SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music) + 5);
		}

	}
	else if (m_unCursor == 0 /*Music volume*/ && (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Left) || SGD::InputManager::GetInstance()->IsDPadPressed(0, SGD::DPad::Left) || m_fstickXoff < 0))
	{
		if(SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music) > 0)
		{
			m_fstickXoff = 0;
			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::Music,
				SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music) - 5);
		}


	}

	//Adjust volume for SFX 
	if (m_unCursor == 1/*SFX volume*/ && (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Right) || SGD::InputManager::GetInstance()->IsDPadPressed(0, SGD::DPad::Right) || m_fstickXoff > 0))
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hSelection);

		if(SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects) < 100)
		{
			m_fstickXoff = 0;
			SGD::AudioManager::GetInstance()->SetMasterVolume(SGD::AudioGroup::SoundEffects,
				SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects) + 5);
		}

	}
	else if (m_unCursor == 1 /*SFX volume*/ && (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Left) || SGD::InputManager::GetInstance()->IsDPadPressed(0, SGD::DPad::Left) || m_fstickXoff < 0))
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hSelection);

		if(SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects) > 0)
		{
			m_fstickXoff = 0;
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
	if(m_unCursor == 0)
	{
		if(!(SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBGM)))
		SGD::AudioManager::GetInstance()->PlayAudio(m_hBGM);
	}
	else if(m_unCursor == 1)
	{
		SGD::AudioManager::GetInstance()->StopAudio(m_hBGM);
		
	}
	else
	{
		SGD::AudioManager::GetInstance()->StopAudio(m_hBGM);

	}

	m_emBackgroundEffect->Update(elapsedTime);
	m_emTitle->Update(elapsedTime);
}

/////////////////////////////////////////////
// Render
// - Render all game entities
void OptionsState::Render(void)
{
	
	//Create a local reference to the input manager for ease of use
	SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");

	float fWidth = Game::GetInstance()->GetScreenWidth();


	//Draw the background
	pGraphics->DrawTexture(m_hBackground, { 0, 0 }, 0.0f, {}, {}, { 1.6f, 1.2f });

	m_emBackgroundEffect->Render();
	m_emTitle->Render({ 220, 10 });
	//Draw the title
	font.DrawString("HONOR", 220, 10, 3, SGD::Color{ 255, 255, 130, 0 });


	//pGraphics->DrawTexture(m_hSword, { m_rSword.left, m_rSword.top }, 0.0f, {}, {}, { 1.4f, 1.4f });




	int nMusicVol = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::Music);
	int nEffectsVol = SGD::AudioManager::GetInstance()->GetMasterVolume(SGD::AudioGroup::SoundEffects);

	SGD::OStringStream ossMus;
	SGD::OStringStream ossSFX;


	ossMus << nMusicVol;
	ossSFX << nEffectsVol;

	if(m_unCursor == 0)
	{
		pGraphics->DrawRectangle(m_rPlay, { 255, 255, 255, 255 }, {}, {});
		pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rPlay.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });

		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 240 }, 0.0f, {}, { 255, 255, 255, 255 });



		font.DrawString("Music Volume:", (int)((fWidth - (4 * 19)) / 2.5), 255, .8f, SGD::Color{ 255, 255, 130, 0 });
		font.DrawString(ossMus.str().c_str(), (int)((fWidth - (4*19)) / 1.5), 255, .8f, { 255, 255, 130, 0 });



	}
	else
	{
		pGraphics->DrawRectangle(m_rPlay, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 240 }, 0.0f, {}, { 255, 255, 255, 255 });

		//pGraphics->DrawTexture(m_hSword, { m_rPlay.left - 50, m_rPlay.top }, 0.0f, {}, {}, { 1.4f, 1.4f });

		font.DrawString("Music Volume:", (int)((fWidth - (4 * 19)) / 2.5), 255, .8f, SGD::Color{ 255, 255, 130, 0 });
		font.DrawString(ossMus.str().c_str(), (int)((fWidth - (4 * 19)) / 1.5), 255, .8f, { 255, 255, 130, 0 });
	}


	if(m_unCursor == 1)
	{
		pGraphics->DrawRectangle(m_rOptions, { 255, 255, 255, 255 }, {}, {});
		pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rOptions.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 310 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Effects Volume:", (int)((fWidth - (4 * 19)) / 2.5), 325, .8f, SGD::Color{ 255, 255, 130, 0 });
		font.DrawString(ossSFX.str().c_str(), (int)((fWidth - (4 * 19)) / 1.5), 325, .8f, { 255, 255, 130, 0 });
	}
	else
	{
		pGraphics->DrawRectangle(m_rOptions, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 310 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Effects Volume:", (int)((fWidth - (4 * 19)) / 2.5), 325, .8f, SGD::Color{ 255, 255, 130, 0 });
		font.DrawString(ossSFX.str().c_str(), (int)((fWidth - (4 * 19)) / 1.5), 325, .8f, { 255, 255, 130, 0 });
	}


	if(m_unCursor == 2 && m_bFullScreen)
	{
		pGraphics->DrawRectangle(m_rInstructions, { 255, 255, 255, 255 }, {}, {});
		pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rInstructions.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 380 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Full Screen:", (int)((fWidth - (4 * 19)) / 2.5), 395, .8f, SGD::Color{ 255, 255, 130, 0 });
		font.DrawString(("ON"), (int)((fWidth - (4 * 19)) / 1.65), 395, .8f, SGD::Color{ 255, 255, 130, 0 });


	}
	else if(m_unCursor != 2 && m_bFullScreen)
	{
		pGraphics->DrawRectangle(m_rInstructions, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 380 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Full Screen:", (int)((fWidth - (4 * 19)) / 2.5), 395, .8f, SGD::Color{ 255, 255, 130, 0 });
		font.DrawString(("ON"), (int)((fWidth - (4 * 19)) / 1.65), 395, .8f, SGD::Color{ 255, 255, 130, 0 });



	}
	else if(m_unCursor == 2 && !m_bFullScreen)
	{
		pGraphics->DrawTexture(m_hSword, { (fWidth - 256) / 2 - 164, m_rInstructions.top + 10 }, 0.0f, {}, {}, { 1.4f, 1.4f });


		pGraphics->DrawRectangle(m_rInstructions, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 380 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Full Screen:", (int)((fWidth - (4 * 19)) / 2.5), 395, .8f, SGD::Color{ 255, 255, 130, 0 });
		font.DrawString(("OFF"), (int)((fWidth - (4 * 19)) / 1.65), 395, .8f, SGD::Color{ 255, 255, 130, 0 });


	}
	else if(m_unCursor != 2 && !m_bFullScreen)
	{
		pGraphics->DrawRectangle(m_rInstructions, { 255, 255, 255, 30 }, {}, {});
		pGraphics->DrawTexture(m_hButton, { (fWidth - (256)) / 2, 380 }, 0.0f, {}, { 255, 255, 255, 255 });

		font.DrawString("Full Screen:", (int)((fWidth - (4 * 19)) / 2.5), 395, .8f, SGD::Color{ 255, 255, 130, 0 });
		font.DrawString(("OFF"), (int)((fWidth - (4 * 19)) / 1.65), 395, .8f, SGD::Color{ 255, 255, 130, 0 });

	}
	
	font.DrawString("Press ", 30, (int)(Game::GetInstance()->GetScreenHeight() - 50), 1.0f, { 255, 130, 0 });
	pGraphics->DrawTexture(m_hEsc, { 120, Game::GetInstance()->GetScreenHeight() - 40 }, 0.0f, {}, {}, { 0.5f, 0.5f });
	font.DrawString("or ", 162, (int)(Game::GetInstance()->GetScreenHeight() - 50), 1.0f, { 255, 130, 0 });
	pGraphics->DrawTexture(m_hCircle, { 200, Game::GetInstance()->GetScreenHeight() - 40 }, 0.0f, {}, {}, { 1.0f, 1.0f });
	font.DrawString("to go back.", 245, (int)(Game::GetInstance()->GetScreenHeight() - 50), 1.0f, { 255, 130, 0 });
}
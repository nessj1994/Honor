#include "Player.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "GameplayState.h"
#include "DestroyEntityMessage.h"
#include "CreateProjectileMessage.h"
#include "CreateSprayMessage.h"
#include "CreateHawkMessage.h"
#include "LevelCollider.h"
#include "Hawk.h"
#include "Emitter.h"
#include "ParticleEngine.h"
#include "AnimationEngine.h"
#include "Font.h"
#include "BitmapFont.h"
#include "Game.h"
#include "Bull.h"
#include "BullEnemy.h"
#include "Vomit.h"
#include "SwordSwing.h"

#include <Windows.h>
#include "Dash.h"
#include "Bounce.h"
#include "Camera.h"
#include "Honor.h"
#include "Jellyfish.h"


#define JOYSTICK_DEADZONE  0.6f
#define TEXT_TIME_LENGTH   1.0f

Player::Player() : Listener(this)
{
	Listener::RegisterForEvent("KILL_PLAYER");
	Listener::RegisterForEvent("BOUNCE_VERTICAL");
	Listener::RegisterForEvent("BOUNCE_HORIZONTAL");
	Listener::RegisterForEvent("ADD_SPRAY");

	//Screen events
	Listener::RegisterForEvent("Screen2x2");
	Listener::RegisterForEvent("Screen2x3");
	Listener::RegisterForEvent("Screen2x4");
	Listener::RegisterForEvent("Screen3x3");
	Listener::RegisterForEvent("Screen2x1.5");
	Listener::RegisterForEvent("Screen3x1.5");
	Listener::RegisterForEvent("Screen1.5x2");
	Listener::RegisterForEvent("Screen1.5x3");
	Listener::RegisterForEvent("Screen3x1");
	Listener::RegisterForEvent("Screen3x1.5");
	Listener::RegisterForEvent("Screen3x2");



	Listener::RegisterForEvent("FINALBOSS");
	Listener::RegisterForEvent("FOURTHBOSS");
	Listener::RegisterForEvent("BossLevel");
	Listener::RegisterForEvent("GainedHawk");





	SetDirection({ 1, 0 });
	m_pDash = new Dash();
	m_pBounce = new Bounce();
	//HUD
	m_hHonorParticleHUD = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/HonorPiece.png");
	m_hBubbleCircle = SGD::GraphicsManager::GetInstance()->LoadTexture("Assets/graphics/bubble.png");
	//Emitters
	m_emHonor = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/SilverHonor.xml", "SilverHonor", { (32), (32) });
	m_emFeatherExplosion = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/FeatherExplosion.xml", "FeatherExplosion", { -1000, -1000 });
	m_emHawkReturn = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/HawkReturn.xml", "HawkReturn", { -1000, -1000 });
	//
	AnimationEngine::GetInstance()->LoadAnimation("Assets/PlayerAnimations.xml");
	m_ts.SetCurrAnimation("Idle");
	AnimationEngine::GetInstance()->LoadAnimation("Assets/ArmoredPlayerAnimations.xml");

	//Load Sounds
	m_hIceEffect = SGD::AudioManager::GetInstance()->LoadAudio("Assets/Audio/IceSpray.wav");
	m_hBounceEffect = SGD::AudioManager::GetInstance()->LoadAudio("assets/audio/BounceEffect.wav");
	m_hJellyfishEffect = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/JellyfishBounce.wav");
	m_hGainAbility = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/GainAbility.wav");
	m_hScream = SGD::AudioManager::GetInstance()->LoadAudio(L"Assets/Audio/PlayerScream.wav");

}


Player::~Player()
{
	delete m_pDash;
	delete m_pBounce;
	delete m_emHonor;
	delete m_emFeatherExplosion;
	delete m_emHawkReturn;
	delete m_pSword;
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hHonorParticleHUD);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBubbleCircle);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hIceEffect);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hBounceEffect);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hJellyfishEffect);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hGainAbility);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hScream);

}

/////////////////////////////////////////////////
/////////////////Interface///////////////////////
void Player::Update(float elapsedTime)
{
	if (GameplayState::GetInstance()->GetIsEnding() == false)
	{
		//Emitter Updates
		m_emHonor->Update(elapsedTime);
		m_emFeatherExplosion->Update(elapsedTime);
		m_emHawkReturn->Update(elapsedTime);
		//

		if (HasBounce() == true)
		{
			if (m_fTextTimer <= TEXT_TIME_LENGTH)
			{
				m_fTextTimer += elapsedTime;
			}
		}

		//if (m_pSword != nullptr)
		//{
		//	m_pSword->Update(elapsedTime);
		//}
		SGD::InputManager* pInput = SGD::InputManager::GetInstance();
		if (m_bDead)
		{
			UpdateDeath(elapsedTime);
		}
		else
		{
			UpdateArmor(elapsedTime);
			UpdateEmitters(elapsedTime);
			UpdateTimers(elapsedTime);
			//Snared Factors done do anything if Snared
			if (m_bSnared)
			{
				UpdateSnared(elapsedTime);
				return;
			}

			float leftStickXOff = pInput->GetLeftJoystick(0).x;
			float leftStickYOff = pInput->GetLeftJoystick(0).y;

			bool leftClamped = false;
			static int stickFrame = 1;
			bool controller = pInput->IsControllerConnected(0);

			if (leftStickXOff > -JOYSTICK_DEADZONE && leftStickXOff < JOYSTICK_DEADZONE)
			{
				leftStickXOff = 0;
				leftClamped = true;
				stickFrame = 5;
			}

			SetIsBouncing(false);
			UpdatePlayerSwing(elapsedTime);

			//////// NEED TO UPDATE WITH CONTROLLER ( JORDAN )
			//if (GetIsInputStuck() == false)
			//	m_fInputTimer = 0;


			/////////////////////////////////////////////////
			/////////////////Controls////////////////////////

			if (IsDashing() == false)///////////////Dash check begins
			{

				/////////////////////////////////////////////////
				////////////////////Jump/////////////////////////

				UpdateJump(elapsedTime);


				//	if(GetIsFalling() == false
				//		&& GetIsJumping() == false)
				if (m_unCurrentState == RESTING_STATE
					|| m_unCurrentState == LANDING_STATE
					|| (m_unCurrentState == JUMPING_STATE && m_bSlowed == true))
				{
					/////////////////////////////////////////////////
					/////////////////Friction////////////////////////

					UpdateFriction(elapsedTime, leftClamped);
				}


				UpdateBounce(elapsedTime);


				/////////////////////////////////////////////////
				/////////////////Movement////////////////////////
				//reset currframe to 0 & set the animation playing to true
				if (IsDashing() == false && !GetStunned())
				{
					UpdateMovement(elapsedTime, stickFrame, leftClamped, leftStickXOff);
				}

				//m_fShotTimer += elapsedTime;


				UpdateDash(elapsedTime);





				/////////////////////////////////////////////////
				///////////////////Shoot/////////////////////////

				UpdateHawk(elapsedTime);

				/////////////////////////////////////////////////
				///////////////////Spray/////////////////////////

				UpdateSpray(elapsedTime);

				/////////////////////////////////////////////////
				//////////////Constant Updates///////////////////

				UpdateConstants(elapsedTime);

			}

			UpdateVelocity(elapsedTime);


			SetIsInputStuck(false);

			is_Left_Coll = false;
			is_Right_Coll = false;

			//ASSESS RANGE event for those checking to see range of player
			SGD::Event* pATEvent = new SGD::Event("ASSESS_PLAYER_RANGE", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;


			Unit::Update(elapsedTime);
			AnimationEngine::GetInstance()->Update(elapsedTime, m_ts, this);

			SetGravity(-3000);
		}
	}
}

void Player::Render(void)
{
	if (GameplayState::GetInstance()->GetIsEnding() == false)
	{
		//Emitter Renders
		if ((IsBouncing() || !GetBounce()->GetEMBubbles()->Done()) && HasBounce())
		{
			GetBounce()->GetEMBubbles()->SetPosition(m_ptPosition);
			GetBounce()->GetEMBubbles()->Render();
		}
		if ((IsDashing() || !GetDash()->GetEMDash()->Done()) && HasDash())
		{
			GetDash()->GetEMDash()->SetPosition(m_ptPosition);
			GetDash()->GetEMDash()->Render();
		}
		if ((m_bHawkExplode || !m_emFeatherExplosion->Done()) && HasHawk())
		{
			m_emFeatherExplosion->Render();
		}
		if (m_bReturningHawk || !m_emHawkReturn->Done())
		{
			m_emHawkReturn->Render();
		}

		//SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

		////Camera::GetInstance()->Draw(SGD::Rectangle(10, 300, 20, 320), SGD::Color::Color(255, 0, 0, 255));


		// * Camera::GetInstance()->GetZoomScale().width
		// * Camera::GetInstance()->GetZoomScale().height

		/*Camera::GetInstance()->Draw(SGD::Rectangle(
			(m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x),
			(m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y),
			(m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width),
			(m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height)),
			SGD::Color::Color(255, 255, 0, 0));*/


		/*Camera::GetInstance()->Draw(SGD::Rectangle(m_pSword->GetRect().left, m_pSword->GetRect().top, m_pSword->GetRect().right, m_pSword->GetRect().bottom),
			SGD::Color::Color(255, 255, 255, 0));*/

		Camera::GetInstance()->DrawAnimation({ m_ptPosition.x + 16, m_ptPosition.y + 60 }, 0, m_ts, !IsFacingRight(), 1.0f);
		//Camera::GetInstance()->Draw(SGD::Rectangle(swingRect.left, swingRect.top, swingRect.right, swingRect.bottom),
		//	SGD::Color::Color(255, 255, 255, 0));

		//Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, !IsFacingRight(), 1.0f);

		// Draw gui for amount of honor
		SGD::OStringStream output;
		//Render Honor emitter in world
		m_emHonor->RenderINworld();
		//Render Honor image
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hHonorParticleHUD, { 25, 34 });

		output << ": " << m_unHonorCollected;
		//Local refernce to the font
		Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");


		//Draw the title

		font.DrawString(output.str().c_str(), 60, 25, 1, SGD::Color{ 255, 255, 0, 0 });

		if (HasBounce() == true)
		{
			if (m_fTextTimer <= TEXT_TIME_LENGTH)
			{
				Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");
				font.DrawString("YOU GAINED THE BUBBLE ABILITY", (int)(m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x),
					(int)(m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y - 100), 1, SGD::Color{ 255, 255, 0, 0 });
			}
		}

		if (m_bDead)
		{
			// Draw a fading rectangle
			unsigned char alpha = (char)(((0.5f - m_fDeathTimer) / 0.5f) * 255.0f);
			GameplayState::GetInstance()->SetScreenFadeout(alpha);
		}
		if (IsBouncing())
		{
			Camera::GetInstance()->DrawTexture({ m_ptPosition.x - 70, m_ptPosition.y - 50 }, 0, m_hBubbleCircle, false, 5, {}, {});
		}
	}
}

SGD::Rectangle Player::GetRect(void) const
{
	return{ m_ptPosition, m_szSize };
}

void Player::HandleCollision(const IEntity* pOther)
{
	float leftStickXOff = SGD::InputManager::GetInstance()->GetLeftJoystick(0).x;
	
	m_bSlowed = false;
	if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::W))
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hBounceEffect);

	}

	Unit::HandleCollision(pOther);
	if (pOther->GetType() == ENT_DOOR)
	{
		m_bSliding = false;
		SetFriction(13000.0f);
		BasicCollision(pOther);
	}

	if (pOther->GetType() == ENT_JELLYFISH)
	{
		m_bSliding = false;

		JellyfishCollision(pOther);
	}

	if (pOther->GetType() == Entity::ENT_HONOR)
	{
		const Honor* honor = dynamic_cast<const Honor*>(pOther);
		if (!honor->GetIsCollected())
		{
			IncreaseHonorCollected(honor->GetHonorAmount());
		}
	}

	if (pOther->GetType() == Entity::ENT_ARMOR)
	{
		m_bHasArmor = true;
		if (m_ts.GetCurrAnimation() == "Idle")
		{
			m_ts.SetCurrAnimation("Armor Player Idle");
			m_ts.SetPlaying(true);
			m_ts.ResetCurrFrame();
		}
		else if (m_ts.GetCurrAnimation() == "Jump")
		{
			m_ts.SetCurrAnimation("Armor Player Jump");
			m_ts.SetPlaying(true);
			m_ts.ResetCurrFrame();
		}
		else if (m_ts.GetCurrAnimation() == "Walking")
		{
			m_ts.SetCurrAnimation("Armor Player Walking");
			m_ts.SetPlaying(true);
			m_ts.ResetCurrFrame();
		}
		else if (m_ts.GetCurrAnimation() == "dashing")
		{
			m_ts.SetCurrAnimation("Armor Player dashing");
			m_ts.SetPlaying(true);
			m_ts.ResetCurrFrame();
		}
	}


	if (pOther->GetType() == Entity::ENT_SOLID_WALL)
	{
		m_bSliding = false;

		is_Platform = true;
		BasicCollision(pOther);
		SetFriction(13000.0f);

		//SGD::GraphicsManager::GetInstance()->DrawString("ZERO", { 100, 300 }, { 255, 255, 0, 0 });

	}
	if (pOther->GetType() == Entity::ENT_LEFT_RAMP)
	{
		m_bSliding = false;

		LeftRampCollision(pOther);
		SetFriction(13000.0f);
		is_Ramp = true;
	}

	if (pOther->GetType() == Entity::ENT_ICE_LEFT_RAMP)
	{
		LeftRampCollision(pOther);
		is_Ramp = true;
		SetFriction(0.1f);

		//if (GetVelocity().x > 0 && m_bFacingRight == false)
		//{
		//	m_vtVelocity.x -= 50;
		//}
		//else if (GetVelocity().x < 0 && m_bFacingRight == true)
		//{
		//	m_vtVelocity.x += 50;
		//}
		if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Q) || SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::E))
			SetVelocity({ GetVelocity().x * 1.56f, GetVelocity().y });



	}
	if (pOther->GetType() == Entity::ENT_RIGHT_RAMP)
	{
		m_bSliding = false;

		RightRampCollision(pOther);
		SetFriction(13000.0f);
		is_Ramp = true;
	}
	if (pOther->GetType() == Entity::ENT_ICE_RIGHT_RAMP)
	{
		RightRampCollision(pOther);
		is_Ramp = true;
		SetFriction(0.1f);


		//if (GetVelocity().x > 0 && m_bFacingRight == false)
		//{
		//	m_vtVelocity.x -= 50;
		//}
		//else if (GetVelocity().x < 0 && m_bFacingRight == true)
		//{
		//	m_vtVelocity.x += 50;
		//}
		if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Q) || SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::E))
			SetVelocity({ GetVelocity().x * 1.56f, GetVelocity().y });


	}



	if (pOther->GetType() == Entity::ENT_MOVING_PLATFORM)
	{
		m_bSliding = false;

		is_Platform = true;
		BasicCollision(pOther);
		SetFriction(13000.0f);
	}

	if (pOther->GetType() == Entity::ENT_BLOCK)
	{
		m_bSliding = false;

		is_Platform = true;
		BasicCollision(pOther);
		SetFriction(13000.0f);
	}

	if (pOther->GetType() == Entity::ENT_FROZEN)
	{
		is_Platform = true;
		m_bSliding = true;
		BasicCollision(pOther);
		SetFriction(0.1f);
		if (GetVelocity().x > 0 && m_bFacingRight == false)
		{
			m_vtVelocity.x -= 65;
		}
		else if (GetVelocity().x < 0 && m_bFacingRight == true)
		{
			m_vtVelocity.x += 65;
		}
		if(SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Q) || SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::E) || (leftStickXOff > 0.2 || leftStickXOff < -0.2))
			SetVelocity(GetVelocity() * 1.06f);

	}

	if (pOther->GetType() == Entity::ENT_NOT_FROZEN)
	{
		m_bSliding = false;

		is_Platform = true;
		BasicCollision(pOther);
		SetFriction(13000.0f);
	}

	if (pOther->GetType() == Entity::ENT_DEATH)
	{
		m_bSliding = false;

		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hScream))
			SGD::AudioManager::GetInstance()->PlayAudio(m_hScream);
		//Kill the player
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}

	if (pOther->GetType() == Entity::ENT_WIZARD_DASH)
	{
		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hScream))
			SGD::AudioManager::GetInstance()->PlayAudio(m_hScream);
		//Kill the player
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}

	if (pOther->GetType() == Entity::ENT_WIZARD_HAWK)
	{
		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hScream))
			SGD::AudioManager::GetInstance()->PlayAudio(m_hScream);
		//Kill the player
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}


	if (pOther->GetType() == Entity::ENT_ICE)
	{
		m_bSliding = true;
		BasicCollision(pOther);
		SetFriction(5.0f);
		if (GetVelocity().x > 0 && m_bFacingRight == false)
		{
			m_vtVelocity.x -= 65;
		}
		else if (GetVelocity().x < 0 && m_bFacingRight == true)
		{
			m_vtVelocity.x += 65;
		}
		if(SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Q) || SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::E) || (leftStickXOff > 0.2 || leftStickXOff < -0.2))
			SetVelocity(GetVelocity() * 1.06f);

	}


	if (pOther->GetType() == Entity::ENT_GEYSER)
	{
		m_bSliding = false;

		is_Platform = true;
		GeyserCollision(pOther);
		//BasicCollision(pOther);
		SetFriction(13000.0f);

	}

	if (pOther->GetType() == Entity::ENT_LASER)
	{
		if (IsDashing() == true)
		{

		}
		else
		{
			LaserCollision(pOther);
		}
	}

	if (pOther->GetType() == Entity::ENT_VOMIT)
	{
		if (!((Vomit*)pOther)->Finished())
		{
			SetFriction(40);
			m_bSlowed = true;
		}
	}

	if (pOther->GetType() == Entity::ENT_LAVA)
	{
		m_bSliding = false;

		//if so move back up but kill the player
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}

	if (pOther->GetType() == Entity::ENT_POOP)
	{
		m_bSliding = false;

		//if so move back up but kill the player
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}

	if (pOther->GetType() == Entity::ENT_BOSS_BULL)
	{
		// Throw the player back
		Bull * bull = (Bull*)(pOther);
		if (bull->GetAttacking())
		{
			ThrowPlayer(bull->IsFacingRight());
		}
	}

	if (pOther->GetType() == Entity::ENT_BULL_ENEMY)
	{
		// Throw the player back
		BullEnemy * bull = (BullEnemy*)(pOther);
		if (bull->GetAttacking())
		{
			ThrowPlayer(bull->GetFacingRight());
		}
	}
	if (pOther->GetType() == Entity::ENT_ICE_GOLEM)
	{
		if (((Enemy*)pOther)->GetAlive())
		{
			SGD::Event* pATEvent = new SGD::Event("KILL_PLAYER", nullptr, this);
			SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
			pATEvent = nullptr;
		}
	}

	if (pOther->GetType() == Entity::ENT_BOSS_YETI)
	{
		//SGD::Event* pATEvent = new SGD::Event("KILL_PLAYER", nullptr, this);
		//SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
		//pATEvent = nullptr;
	}

}


void Player::BasicCollision(const IEntity* pOther)
{

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	SetGravity(-3000);

	RECT rPlayer;
	rPlayer.left = (LONG)GetRect().left;
	rPlayer.top = (LONG)GetRect().top;
	rPlayer.right = (LONG)GetRect().right;
	rPlayer.bottom = (LONG)GetRect().bottom;

	//Create a rectangle for the other object
	RECT rObject;
	rObject.left = (LONG)pOther->GetRect().left;
	rObject.top = (LONG)pOther->GetRect().top;
	rObject.right = (LONG)pOther->GetRect().right;
	rObject.bottom = (LONG)pOther->GetRect().bottom;

	//Create a rectangle for the intersection
	RECT rIntersection = {};

	RECT rPlayerWall;
	rPlayerWall.left = (LONG)GetRect().left - 1;
	rPlayerWall.top = (LONG)GetRect().top;
	rPlayerWall.right = (LONG)GetRect().right + 1;
	rPlayerWall.bottom = (LONG)GetRect().bottom;

	IntersectRect(&rIntersection, &rObject, &rPlayerWall);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;

	if (nIntersectHeight > nIntersectWidth)
	{
		if (m_unCurrentState == JUMPING_STATE
			|| m_unCurrentState == FALLING_STATE)
			SetIsInputStuck(true);

	}

	IntersectRect(&rIntersection, &rPlayer, &rObject);

	nIntersectWidth = rIntersection.right - rIntersection.left;
	nIntersectHeight = rIntersection.bottom - rIntersection.top;

	//Colliding with the side of the object
	if (nIntersectHeight > nIntersectWidth)
	{
		if (rPlayer.right == rIntersection.right)
		{

			if (m_unCurrentState == RESTING_STATE
				|| m_unCurrentState == LANDING_STATE)
			{
				SetPosition({ (float)rObject.left - GetSize().width, GetPosition().y });
				SetVelocity({ 0, GetVelocity().y });
				SetDashTimer(0);
			}
			else
			{
				SetPosition({ (float)rObject.left - GetSize().width + 1, GetPosition().y });
				SetVelocity({ 0, GetVelocity().y });
				SetDashTimer(0);
			}


			if ((pInput->IsButtonDown(0, 0) == true
				|| pInput->IsKeyDown(SGD::Key::Space) == true)
				&& (m_unCurrentState != RESTING_STATE
				|| m_unCurrentState != LANDING_STATE)
				)
			{
				is_Right_Coll = true;
			}
		}
		if (rPlayer.left == rIntersection.left)
		{
			if (m_unCurrentState == RESTING_STATE
				|| m_unCurrentState == LANDING_STATE)
			{
				SetPosition({ (float)rObject.right, GetPosition().y });
				SetDashTimer(0);
				SetVelocity({ 0, GetVelocity().y });
			}
			else
			{
				SetPosition({ (float)rObject.right - 1, GetPosition().y });
				SetDashTimer(0);
				SetVelocity({ 0, GetVelocity().y });
			}



			if (

				/*(pInput->IsButtonDown(0, 0) == true
				|| pInput->IsKeyDown(SGD::Key::Space))


				&&*/ (m_unCurrentState != RESTING_STATE
				|| m_unCurrentState != LANDING_STATE)
				//&& m_fButtonTimer > 0
				)
			{
				is_Left_Coll = true;
			}

		}
	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (rPlayer.bottom == rIntersection.bottom)
		{
			if (IsBouncing() == true)
			{

				if (m_unJumpCount < 3)
					m_unJumpCount++;

				if (m_unJumpCount == 1)
					SetVelocity({ GetVelocity().x, -900.0f });
				if (m_unJumpCount == 2)
					SetVelocity({ GetVelocity().x, -1500.0f });
				if (m_unJumpCount == 3)
					SetVelocity({ GetVelocity().x, -1900.0f });


				SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height  /*- nIntersectHeight*/ });
			}

			else
			{
				SetVelocity({ GetVelocity().x, 0 });
				SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 /*- nIntersectHeight*/ });
				if (m_unCurrentState == FALLING_STATE)
				{

					m_unJumpCount = 0;

					m_fLandTimer = 0.001f;
					m_unCurrentState = LANDING_STATE;
				}

			}



			SetJumpVelCur(0);
			SetIsInputStuck(false);

			is_Left_Coll = false;
			is_Right_Coll = false;

			SGD::Event Event = { "RESET_JELLYFISH_BOUNCE", nullptr, this };
			SGD::EventManager::GetInstance()->SendEventNow(&Event);
		}
		if (rPlayer.top == rIntersection.top)
		{
			SetPosition({ GetPosition().x, (float)rObject.bottom });
			SetVelocity({ GetVelocity().x, 0 });
		}
	}

	if (IsBouncing() == false
		&& m_unCurrentState == RESTING_STATE)
	{
		m_unJumpCount = 0;
	}

}

void Player::SwingCollision(const IEntity* pOther)
{

}


void Player::LeftRampCollision(const IEntity* pOther)
{
	///////for LEFT ramp RIGHT side of player

	float tempVal = 32.0f / 32.0f;


	//SetGravity(GetGravity() * 4);
	SetGravity(0);
	SetVelocity({ GetVelocity().x, 0 });


	if (GetVelocity().x > 300)
	{
		m_vtVelocity.x = 300;

	}
	if (GetVelocity().x < -400)
	{
		m_vtVelocity.x = -400;
	}

	RECT rPlayer;
	rPlayer.left = (LONG)GetRect().left;
	rPlayer.top = (LONG)GetRect().top;
	rPlayer.right = (LONG)GetRect().right /*- 16*/;
	rPlayer.bottom = (LONG)GetRect().bottom /*- 10*/;

	////Create a rectangle for the other object
	RECT rObject;
	rObject.left = (LONG)pOther->GetRect().left;
	rObject.top = (LONG)pOther->GetRect().top;
	rObject.right = (LONG)pOther->GetRect().right;
	rObject.bottom = (LONG)pOther->GetRect().bottom;

	////Create a rectangle for the intersection
	RECT rIntersection = {};


	IntersectRect(&rIntersection, &rPlayer, &rObject);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;

	if (/*nIntersectWidth*/ /*nIntersectWidth > 1  &&*/ nIntersectWidth < 31)
	{
		//SetPosition({ GetPosition().x, (float)rObject.bottom - tempVal - GetSize().height });
		m_ptPosition.y = (float)rObject.bottom - tempVal - GetSize().height;
		m_ptPosition.y = m_ptPosition.y - (nIntersectWidth * tempVal);
	}

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	if (pInput->IsKeyDown(SGD::Key::Space) == true
		|| pInput->IsButtonDown(0, 0 /*A button on Xbox*/) == true)

	{
		m_ptPosition.y = (float)rObject.top - 64;
	}

	else
	{


		if (nIntersectWidth == 31)
		{
			//m_ptPosition.y = (float)rObject.bottom - tempVal - GetSize().height;
			//m_ptPosition.y = m_ptPosition.y - (nIntersectWidth * tempVal);

			//m_ptPosition.y = (float)rObject.top ;
			//m_ptPosition.y = m_ptPosition.y - (nIntersectWidth * tempVal);

			if (m_ptPosition.x > (float)rObject.left)
			{
				m_ptPosition.x += 1;
			}
			else
			{
				BasicCollision(pOther);
			}


		}
		else if (nIntersectWidth == 32)
		{
			if (m_ptPosition.x > (float)rObject.left)
				m_ptPosition.x -= 1;

			else
				m_ptPosition.x += 1;

			BasicCollision(pOther);
		}

	}
	if (m_ptPosition.x + m_szSize.width > rObject.right)
		SetVelocity({ m_vtVelocity.x, 1000 });

	if (m_unCurrentState == FALLING_STATE)
	{

		m_unJumpCount = 0;

		m_fLandTimer = 0.001f;
		m_unCurrentState = LANDING_STATE;
	}
}

void Player::RightRampCollision(const IEntity* pOther)
{
	///////for RIGHT ramp LEFT side of player

	float tempVal = 32.0f / 32.0f;

	//SetGravity(GetGravity() * 4);
	SetGravity(0);


	SetVelocity({ GetVelocity().x, 0 });
	if (GetVelocity().x > 300)
	{
		m_vtVelocity.x = 300;

	}
	if (GetVelocity().x < -400)
	{
		m_vtVelocity.x = -400;
	}


	//SetGravity(0);

	RECT rPlayer;
	rPlayer.left = (LONG)GetRect().left;
	rPlayer.top = (LONG)GetRect().top;
	rPlayer.right = (LONG)GetRect().right /*- 16*/;
	rPlayer.bottom = (LONG)GetRect().bottom /*- 10*/;

	//Create a rectangle for the other object
	RECT rObject;
	rObject.left = (LONG)pOther->GetRect().left;
	rObject.top = (LONG)pOther->GetRect().top;
	rObject.right = (LONG)pOther->GetRect().right;
	rObject.bottom = (LONG)pOther->GetRect().bottom;

	//Create a rectangle for the intersection
	RECT rIntersection = {};


	IntersectRect(&rIntersection, &rPlayer, &rObject);


	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;



	float tempInt = nIntersectWidth * tempVal;



	/*SGD::Rectangle rPlayer = GetRect();
	SGD::Rectangle rOther = pOther->GetRect();
	SGD::Rectangle rIntersecting = rPlayer.ComputeIntersection(rOther);*/

	//float rIntersectWidth = rIntersection.ComputeWidth();
	//float rIntersectHeight = rIntersection.ComputeHeight();



	SGD::InputManager* pInput = SGD::InputManager::GetInstance();


	if (pInput->IsKeyDown(SGD::Key::Space) == true
		|| pInput->IsButtonDown(0, 0 /*A button on Xbox*/) == true)

	{
		m_ptPosition.y = (float)rObject.top - 64;
	}
	else
	{


		if (/*nIntersectWidth*/ nIntersectWidth < 31)
		{
			tempVal = 32 / 32;
			if (m_ptPosition.x >(float)rObject.left)
			{
				m_ptPosition.y = (float)rObject.bottom - GetSize().height - tempVal;
				m_ptPosition.y = m_ptPosition.y - (nIntersectWidth * 1);
			}
		}
		//else
		//{
		//	//SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 });
		//	m_ptPosition.y -= rIntersectHeight;
		//}
		else if (nIntersectWidth == 31)
		{
			BasicCollision(pOther);
		}
		else if (nIntersectWidth == 32)
		{
			if (m_ptPosition.x > (float)rObject.left)
				m_ptPosition.x -= 1;
			else
			{
				m_ptPosition.x += 1;
			}
			BasicCollision(pOther);
		}

	}


	if (m_ptPosition.x + m_szSize.width < rObject.left)
		SetVelocity({ m_vtVelocity.x, 1000 });

	if (m_unCurrentState == FALLING_STATE)
	{

		m_unJumpCount = 0;

		m_fLandTimer = 0.001f;
		m_unCurrentState = LANDING_STATE;
	}






}

void Player::GeyserCollision(const IEntity* pOther)
{
	RECT rPlayer;
	rPlayer.left = (LONG)GetRect().left;
	rPlayer.top = (LONG)GetRect().top;
	rPlayer.right = (LONG)GetRect().right;
	rPlayer.bottom = (LONG)GetRect().bottom;

	//Create a rectangle for the other object
	RECT rObject;
	rObject.left = (LONG)pOther->GetRect().left;
	rObject.top = (LONG)pOther->GetRect().top;
	rObject.right = (LONG)pOther->GetRect().right;
	rObject.bottom = (LONG)pOther->GetRect().bottom;

	//Create a rectangle for the intersection
	RECT rIntersection = {};

	//	RECT rPlayerWall;
	//	rPlayerWall.left = (LONG)GetRect().left - 1;
	//	rPlayerWall.top = (LONG)GetRect().top;
	//	rPlayerWall.right = (LONG)GetRect().right + 1;
	//	rPlayerWall.bottom = (LONG)GetRect().bottom;
	//
	//	IntersectRect(&rIntersection, &rPlayer, &rPlayerWall);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;

	//if (nIntersectHeight > nIntersectWidth)
	//{
	//	if (GetIsFalling() == true
	//		|| GetIsJumping() == true)
	//		SetIsInputStuck(true);
	//
	//}

	IntersectRect(&rIntersection, &rPlayer, &rObject);

	nIntersectWidth = rIntersection.right - rIntersection.left;
	nIntersectHeight = rIntersection.bottom - rIntersection.top;

	//Colliding with the side of the object
	if (nIntersectHeight > nIntersectWidth)
	{
		if (rPlayer.right == rIntersection.right)
		{

			//SetPosition({ (float)rObject.left - GetSize().width + 1, GetPosition().y });
			SetVelocity({ 500 * GetDirection().x * -1, GetVelocity().y - 300 });
			SetDashTimer(0);

			is_Right_Coll = true;
		}
		if (rPlayer.left == rIntersection.left)
		{

			SetVelocity({ 500 * GetDirection().x * -1, GetVelocity().y - 300 });
			SetDashTimer(0);

			is_Left_Coll = true;


			if (m_fDashTime == 0)
			{
				m_ts.SetPlaying(true);
				m_ts.ResetCurrFrame();
				if (m_bHasArmor == false)
				{
					m_ts.SetCurrAnimation("Idle");
				}
				else
				{
					m_ts.SetCurrAnimation("Armor Player Idle");
				}

			}


		}
	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (rPlayer.bottom == rIntersection.bottom)
		{

			if (IsBouncing() == true)
			{
				//	SetVelocity({ GetVelocity().x, GetVelocity().y * -1 });
				//				SetJumpVelCur(GetJumpVelCur() * -1);
				SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height  /*- nIntersectHeight*/ });

			}

			else
			{
				SetVelocity({ 400 * GetDirection().x * -1, GetVelocity().y });
				m_unJumpCount = 0;

			}

			SetJumpVelCur(0);

			m_unCurrentState = RESTING_STATE;


			//SetIsJumping(false);
			//SetIsFalling(false);
			SetIsInputStuck(false);

			is_Left_Coll = false;
			is_Right_Coll = false;
		}
		if (rPlayer.top == rIntersection.top)
		{
			SetPosition({ GetPosition().x, (float)rObject.bottom });
			SetVelocity({ GetVelocity().x, 0 });
		}
	}
}

void Player::LaserCollision(const IEntity* pOther)
{
	RECT rPlayer;
	rPlayer.left = (LONG)GetRect().left;
	rPlayer.top = (LONG)GetRect().top;
	rPlayer.right = (LONG)GetRect().right;
	rPlayer.bottom = (LONG)GetRect().bottom;

	//Create a rectangle for the other object
	RECT rObject;
	rObject.left = (LONG)pOther->GetRect().left;
	rObject.top = (LONG)pOther->GetRect().top;
	rObject.right = (LONG)pOther->GetRect().right;
	rObject.bottom = (LONG)pOther->GetRect().bottom;

	//Create a rectangle for the intersection
	RECT rIntersection = {};


	IntersectRect(&rIntersection, &rPlayer, &rObject);


	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;

	//Colliding with the side of the object
	if (nIntersectHeight > nIntersectWidth)
	{
		if (rPlayer.right == rIntersection.right)
		{

			//if so move back up but kill the player
			SGD::Event Event = { "KILL_PLAYER", nullptr, this };
			SGD::EventManager::GetInstance()->SendEventNow(&Event);

		}
		if (rPlayer.left == rIntersection.left)
		{
			//if so move back up but kill the player
			SGD::Event Event = { "KILL_PLAYER", nullptr, this };
			SGD::EventManager::GetInstance()->SendEventNow(&Event);

		}
	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (rPlayer.bottom == rIntersection.bottom)
		{

			//if so move back up but kill the player
			SGD::Event Event = { "KILL_PLAYER", nullptr, this };
			SGD::EventManager::GetInstance()->SendEventNow(&Event);
		}
		if (rPlayer.top == rIntersection.top)
		{

			//if so move back up but kill the player
			SGD::Event Event = { "KILL_PLAYER", nullptr, this };
			SGD::EventManager::GetInstance()->SendEventNow(&Event);

		}
	}

}

void Player::JellyfishCollision(const IEntity* pOther)
{
	RECT rPlayer;
	rPlayer.left = (LONG)GetRect().left;
	rPlayer.top = (LONG)GetRect().top;
	rPlayer.right = (LONG)GetRect().right;
	rPlayer.bottom = (LONG)GetRect().bottom;

	//Create a rectangle for the other object
	RECT rObject;
	rObject.left = (LONG)pOther->GetRect().left;
	rObject.top = (LONG)pOther->GetRect().top;
	rObject.right = (LONG)pOther->GetRect().right;
	rObject.bottom = (LONG)pOther->GetRect().bottom;

	//Create a rectangle for the intersection
	RECT rIntersection = {};

	IntersectRect(&rIntersection, &rPlayer, &rObject);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;

	//Colliding with the side of the object
	if (nIntersectHeight > nIntersectWidth)
	{
		if (rPlayer.right == rIntersection.right)
		{

			SetPosition({ (float)rObject.left - GetSize().width + 1, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
		if (rPlayer.left == rIntersection.left)
		{
			SetPosition({ (float)rObject.right, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
		}
	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (rPlayer.bottom == rIntersection.bottom)
		{



			const Jellyfish* jfish = dynamic_cast<const Jellyfish*>(pOther);
			//SetVelocity({ GetVelocity().x, /*GetVelocity().y*/1500 * (-1.0f - (0.1f * jfish->GetBounceCount())) });
			SetVelocity({ 0, (-400.0f  - (400.0f * jfish->GetBounceCount())  ) });
			//	SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height /*- nIntersectHeight*/ });
			SGD::AudioManager::GetInstance()->PlayAudio(m_hJellyfishEffect);
			//SetIsFalling(false);
		}
		if (rPlayer.top == rIntersection.top)
		{
			SetPosition({ GetPosition().x, (float)rObject.bottom });
			SetVelocity({ GetVelocity().x, 0 });
		}
	}
}

////////////////////////////////////////////////
/////////////////////Methods///////////////////
void Player::CastDash()
{
	m_pDash->CastDash(this);

	m_unCurrentState = FALLING_STATE;
}

void Player::CastHawk()
{

}

void Player::CastIce()
{


}

void Player::CastBounce()
{


}

////////////////////////////////////////////////
////////////// Listener Interface //////////////
void Player::HandleEvent(const SGD::Event* pEvent)
{
	//which event

	//Set the player back to his last checkpoint 
	//This is usually back to the level start
	if (pEvent->GetEventID() == "KILL_PLAYER")
	{
		//Kill the player
		if (!m_bDead && m_fArmorTimer <= 0.0f)
		{
			KillPlayer();
		}
	}

	if (pEvent->GetEventID() == "GainedHawk")
	{
		m_bHasHawk = true;
	}
	if (pEvent->GetEventID() == "BOUNCE_VERTICAL")
	{
		SetVelocity({ GetVelocity().x, -2000 });
		SetPosition({ GetPosition().x, GetPosition().y - 1 });
	}

	if (pEvent->GetEventID() == "BOUNCE_HORIZONTAL")
	{
		m_unCurrentState = JUMPING_STATE;
		if (*(float*)pEvent->GetData() > -1.0f
			&& *(float*)pEvent->GetData() < 1.0f)
		{
			SetVelocity({ 10000.0f, -1000 });
		}
		else
			SetVelocity({ 10000.0f * (*(float*)pEvent->GetData()), -1000 });
		//SetVelocity({ 10000.0f, -1000 });

		SetPosition({ GetPosition().x, GetPosition().y - 10 });

	}

	if (pEvent->GetEventID() == "ADD_SPRAY")
	{
		m_bHasIce = true;
	}

	if (pEvent->GetEventID() == "Screen2x2")
	{
		m_fPanX = 2;
		m_fPanY = 2;
		Camera::GetInstance()->SetCameraCap(0);
	}
	if (pEvent->GetEventID() == "Screen2x3")
	{
		m_fPanX = 2;
		m_fPanY = 3;
		Camera::GetInstance()->SetCameraCap(0);

	}
	if (pEvent->GetEventID() == "Screen2x4")
	{
		m_fPanX = 5;
		m_fPanY = 2;
		Camera::GetInstance()->SetCameraCap(0);

	}

	if (pEvent->GetEventID() == "Screen3x1")
	{
		m_fPanX = 3;
		m_fPanY = 1.2f;
		Camera::GetInstance()->SetCameraCap(0);

	}



	if (pEvent->GetEventID() == "Screen3x3")
	{
		m_fPanX = 3;
		m_fPanY = 2.5f;
		Camera::GetInstance()->SetCameraCap(0);

	}
	if (pEvent->GetEventID() == "Screen2x1.5")
	{
		m_fPanX = 2;
		m_fPanY = 1.5f;
		Camera::GetInstance()->SetCameraCap(0);

	}
	if (pEvent->GetEventID() == "Screen3x1.5")
	{
		m_fPanX = 3;
		m_fPanY = 1.5f;
		Camera::GetInstance()->SetCameraCap(0);

	}

	if (pEvent->GetEventID() == "Screen3x2")
	{
		m_fPanX = 3;
		m_fPanY = 2;
		Camera::GetInstance()->SetCameraCap(0);

	}
	if (pEvent->GetEventID() == "Screen1.5x2")
	{
		m_fPanX = 1.5;
		m_fPanY = 2;
		Camera::GetInstance()->SetCameraCap(0);

	}
	if (pEvent->GetEventID() == "Screen1.5x3")
	{
		m_fPanX = 1.5;
		m_fPanY = 3;
		Camera::GetInstance()->SetCameraCap(0);

	}
	if (pEvent->GetEventID() == "FINALBOSS")
	{
		Camera::GetInstance()->SetCameraCap(0);
		//Camera::GetInstance()->SetCameraCap(5);


	}
	if (pEvent->GetEventID() == "FOURTHBOSS")
	{

		m_fPanX = 2;
		m_fPanY = 2;

		Camera::GetInstance()->SetCameraCap(4);

	}
	if (pEvent->GetEventID() == "BossLevel")
	{
		Camera::GetInstance()->SetCameraCap(2);
	}
}

void Player::KillPlayer()
{
	if (m_bHasArmor)
	{
		// Jump and remove armor
		m_bHasArmor = false;
		m_fArmorTimer = 2.0f;
		SetPosition({ m_ptPosition.x, m_ptPosition.y - 1 });
		SetVelocity({ m_vtVelocity.x, 0 });
		SetVelocity({ m_vtVelocity.x, m_vtVelocity.y - 1200 });
		m_unJumpCount = 0;
		if (m_ts.GetCurrAnimation() == "Armor Player Idle")
		{
			m_ts.SetCurrAnimation("Idle");
			m_ts.SetPlaying(true);
			m_ts.ResetCurrFrame();
		}
		else if (m_ts.GetCurrAnimation() == "Armor Player Jump")
		{
			m_ts.SetCurrAnimation("Jump");
			m_ts.SetPlaying(true);
			m_ts.ResetCurrFrame();
		}
		else if (m_ts.GetCurrAnimation() == "Armor Player Walking")
		{
			m_ts.SetCurrAnimation("Walking");
			m_ts.SetPlaying(true);
			m_ts.ResetCurrFrame();
		}
		else if (m_ts.GetCurrAnimation() == "Armor Player dashing")
		{
			m_ts.SetCurrAnimation("dashing");
			m_ts.SetPlaying(true);
			m_ts.ResetCurrFrame();
		}
	}
	else
	{
		m_fDeathTimer = 0.5f;
		m_bDead = true;
		m_unJumpCount = 0;

		if (!SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hScream))
			SGD::AudioManager::GetInstance()->PlayAudio(m_hScream);

		// TODO Add effects
		m_bSlowed = false;
	}
}

#pragma region Player update methods
void Player::UpdateDeath(float elapsedTime)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	m_fDeathTimer -= elapsedTime;
	if (m_fDeathTimer <= 0.0f)
	{
		m_bDead = false;
		m_fDeathTimer = 0.0f;
		m_ptPosition = m_ptStartPosition;
		SetVelocity({ 0.0f, 0.0f });
		GameplayState::GetInstance()->SetScreenFadeout(0);

		// Reset room
		SGD::Event* pATEvent = new SGD::Event("ResetRoom", nullptr, this);
		SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
		pATEvent = nullptr;

		// Reset honor
		GameplayState::GetInstance()->ResetHonorInRoom();
	}
}

void Player::UpdateArmor(float elapsedTime)
{
	// Update armor timer
	if (m_fArmorTimer > 0)
	{
		m_fArmorTimer -= elapsedTime;
	}
}

void Player::UpdateEmitters(float elapsedTime)
{
	//Emitter Updates
	m_pBounce->GetEMBubbles()->Update(elapsedTime);
	m_pDash->GetEMDash()->Update(elapsedTime);
	m_emHonor->Update(elapsedTime);
}

void Player::UpdateTimers(float elapsedTime)
{
	//Timers
	m_fIceTimer += elapsedTime;

	m_fJumpTimer -= elapsedTime;
	if (m_bSlowed == true)
	{
		m_fJumpTimer -= elapsedTime;

	}

	m_fLandTimer -= elapsedTime;

	m_fSwingTimer -= elapsedTime;

	if (m_fStunTimer > 0)
		m_fStunTimer -= elapsedTime;
	else if (m_bStunned)
	{
		m_fStunTimer = 0.0f;
		m_bStunned = false;

		//Kill the player
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
	}

	if (m_fSwingTimer < 0.0f)
	{
		m_fSwingTimer = 0;
		is_Swinging = false;
	}

	if (m_fJumpTimer < 0.0f)
		m_fJumpTimer = 0;

	if (m_fLandTimer < 0.0f)
		m_fLandTimer = 0;

	if (GetIsInputStuck() == false)
		m_fInputTimer = 0;


	m_fHawkTimer += elapsedTime;

	m_fShotTimer += elapsedTime;

}

void Player::UpdateFriction(float elapsedTime, bool leftClamped)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	//Left Friction
	if (GetVelocity().x < 0
		&& (pInput->IsKeyDown(SGD::Key::Q) == false
		&& leftClamped == true))
	{

		SetVelocity(SGD::Vector(GetVelocity().x + ( GetFriction() * elapsedTime  ) , GetVelocity().y));

		if (GetVelocity().x > 0
			&& GetVelocity().x < 100)
		{
			SetVelocity(SGD::Vector(0, GetVelocity().y));
		}

	}
	//Right Friction
	if (GetVelocity().x > 0
		&& (pInput->IsKeyDown(SGD::Key::E) == false
		&& leftClamped == true))
	{

		SetVelocity(SGD::Vector(GetVelocity().x - (GetFriction() * elapsedTime), GetVelocity().y));

		if (GetVelocity().x < 0
			&& GetVelocity().x > -100)
		{
			SetVelocity(SGD::Vector(0, GetVelocity().y));
		}
	}

	if (m_bSlowed)
	{
		if (m_unCurrentState != JUMPING_STATE
			/*&& m_unCurrentState != FALLING_STATE*/
			)
		{

			if (GetVelocity().x > 0)
			{
				SetVelocity(SGD::Vector(GetVelocity().x - GetFriction(), GetVelocity().y));
				if (GetVelocity().x < 0)
				{
					SetVelocity({ 0, GetVelocity().y });
				}
			}
			else
			{
				SetVelocity(SGD::Vector(GetVelocity().x + GetFriction(), GetVelocity().y));
				if (GetVelocity().x > 0)
				{
					SetVelocity({ 0, GetVelocity().y });
				}
			}

		}

		if (GetVelocity().y > 0)
		{
			SetVelocity(SGD::Vector(GetVelocity().x, GetVelocity().y - GetFriction()));
			if (GetVelocity().y < 0)
			{
				SetVelocity({ GetVelocity().x, 0 });
			}
		}
		else
		{
			SetVelocity(SGD::Vector(GetVelocity().x, GetVelocity().y + GetFriction()));
			if (GetVelocity().y > 0)
			{
				SetVelocity({ GetVelocity().x, 0 });

			}
		}

	}
}

void Player::UpdateBounce(float elapsedTime)
{
	if (HasBounce() == true)
	{
		SGD::InputManager* pInput = SGD::InputManager::GetInstance();

		//pInput->GetDPad(0);

		if (pInput->IsKeyDown(SGD::Key::W) == true
			|| pInput->IsButtonDown(0, 4 /*Left Bumper*/))
		{
			//if(!(SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hBounceEffect)))
			//{
			//}
			GetBounce()->GetEMBubbles()->Finish(false);
			SetIsBouncing(true);
		}
		else
		{
			GetBounce()->GetEMBubbles()->Finish();
			if (GetBounce()->GetEMBubbles()->Done())
			{
				//Reseting particles for the bounce since its false
				GetBounce()->GetEMBubbles()->KillParticles(m_ptPosition);
			}
		}
	}

}

void Player::UpdateMovement(float elapsedTime, int stickFrame, bool leftClamped, float leftStickXOff)
{
	if(GetIsInputStuck() == true)
		m_fInputTimer += elapsedTime;

	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	if (pInput->IsKeyPressed(SGD::Key::E) == true || pInput->IsKeyPressed(SGD::Key::Q) == true || (stickFrame == 5 && leftClamped == false)  )
	{
		stickFrame = 1;
		//m_ts.ResetCurrFrame();

		//m_ts.SetPlaying(true);
	}

	if(pInput->IsKeyPressed(SGD::Key::J))
	{
		SGD::Event* pATEvent = new SGD::Event("JUMP_TIME", nullptr, this);
		SGD::EventManager::GetInstance()->QueueEvent(pATEvent);
		pATEvent = nullptr;
	}

	//reset currframe to 0 & set the animation playing to false
	if ((pInput->IsKeyDown(SGD::Key::E) == true || pInput->IsKeyDown(SGD::Key::Q) == true) || pInput->IsKeyDown(SGD::Key::Space) == true )
	{
		leftClamped = false;
	}
	if(pInput->IsKeyReleased(SGD::Key::E) == true || pInput->IsKeyReleased(SGD::Key::Q) == true)
	{
		if(m_unCurrentState != JUMPING_STATE && m_unCurrentState != FALLING_STATE)
		{
			m_ts.SetPlaying(false);
			m_ts.ResetCurrFrame();
			if(m_bHasArmor == false)
			{
				m_ts.SetCurrAnimation("Idle");
			}
			else
			{
				m_ts.SetCurrAnimation("Armor Player Idle");
			}
			m_ts.SetPlaying(true);
		}
	}
	else if(leftClamped == true && m_unCurrentState == RESTING_STATE && is_Swinging == false)
	{
		m_ts.SetPlaying(false);
		m_ts.ResetCurrFrame();
		if(m_bHasArmor == false)
		{
			m_ts.SetCurrAnimation("Idle");
		}
		else
		{
			m_ts.SetCurrAnimation("Armor Player Idle");
		}
		m_ts.SetPlaying(true);

	}


	//Right Movement
	if(pInput->IsKeyDown(SGD::Key::E) == true
		|| leftStickXOff > JOYSTICK_DEADZONE)
	{




		if(m_fInputTimer > 0.20f
			|| GetIsInputStuck() == false)
		{
			if(GetVelocity().x < 0)
			{
				if(m_unCurrentState == RESTING_STATE
					|| m_unCurrentState == LANDING_STATE)
				{
					SetVelocity(SGD::Vector(GetVelocity().x + (5 * GetSpeed() * elapsedTime), GetVelocity().y));
				}
				else
				{
					SetVelocity(SGD::Vector(GetVelocity().x + (3 * GetSpeed() * elapsedTime), GetVelocity().y));
				}
			}
			else
			{
				if(GetIsInputStuck() == true)
				{
					SetVelocity(SGD::Vector(GetVelocity().x + (3 * GetSpeed()) * elapsedTime, GetVelocity().y));

				}
				else
					SetVelocity(SGD::Vector(GetVelocity().x + (1 * GetSpeed()) * elapsedTime, GetVelocity().y));
				//SetVelocity(SGD::Vector(1050, GetVelocity().y));
			}

			SetDirection({ 1, 0 });
		}
		if(m_unCurrentState == RESTING_STATE)
		{
			if (is_Swinging == false)
			{
				if (m_bHasArmor == false)
				{
					m_ts.SetCurrAnimation("Walking");
					m_ts.SetPlaying(true);
				}
				else
				{
					m_ts.SetCurrAnimation("Armor Player Walking");
					m_ts.SetPlaying(true);
				}
			}
		}
		SetFacingRight(true);
	}

	//Left Movement
	if(pInput->IsKeyDown(SGD::Key::Q) == true
		|| leftStickXOff < -JOYSTICK_DEADZONE)
	{


		if(m_fInputTimer > 0.20f
			|| GetIsInputStuck() == false)
		{
			if(GetVelocity().x > 0)
			{
				if(m_unCurrentState == RESTING_STATE
					|| m_unCurrentState == LANDING_STATE)
				{
					SetVelocity(SGD::Vector(GetVelocity().x - (5 * GetSpeed() * elapsedTime), GetVelocity().y));
				}
				else
				{
					SetVelocity(SGD::Vector(GetVelocity().x - (3 * GetSpeed() * elapsedTime), GetVelocity().y));
				}
			}
			else
			{
				if(GetIsInputStuck() == true)
				{
					SetVelocity(SGD::Vector(GetVelocity().x - (3 * GetSpeed()) * elapsedTime, GetVelocity().y));

				}
				else
					SetVelocity(SGD::Vector(GetVelocity().x - (1 * GetSpeed()) * elapsedTime, GetVelocity().y));
				//SetVelocity(SGD::Vector(-1050, GetVelocity().y));
			}
			SetDirection({ -1, 0 });
		}
		if(m_unCurrentState == RESTING_STATE)
		{
			if (is_Swinging == false)
			{
				if (m_bHasArmor == false)
				{
					m_ts.SetCurrAnimation("Walking");
					m_ts.SetPlaying(true);
				}
				else
				{
					m_ts.SetCurrAnimation("Armor Player Walking");
					m_ts.SetPlaying(true);
				}
			}
		}
		SetFacingRight(false);
	}
}

void Player::UpdateDash(float elapsedTime)
{
	if (m_fDashCoolTimer > 0.0f)
	{
		m_fDashCoolTimer -= elapsedTime;
	}
	if (HasDash() == true)
	{
		SGD::InputManager* pInput = SGD::InputManager::GetInstance();
		if (m_fDashCoolTimer <= 0.0f && (pInput->IsKeyPressed(SGD::Key::Tab) == true
			//if (pInput->IsKeyDown(SGD::Key::Tab) == true
			|| pInput->IsButtonPressed(0, /*3*/5 /*Right bumper on xbox controller*/)))
		{
			m_fDashCoolTimer = .5f;
			GetDash()->GetEMDash()->Finish(false);
			CastDash();
			m_ts.SetPlaying(true);
			m_ts.ResetCurrFrame();
			if (m_bHasArmor == false)
			{
				m_ts.SetCurrAnimation("dashing");
			}
			else
			{
				m_ts.SetCurrAnimation("Armor Player dashing");
			}
		}
		else
		{
			GetDash()->GetEMDash()->Finish();
			if (GetDash()->GetEMDash()->Done())
			{
				//Reset Dash Particles to the players position
				GetDash()->GetEMDash()->KillParticles(m_ptPosition);
			}
		}
		if (this->IsDashing() == false && (m_ts.GetCurrAnimation() == "dashing" || m_ts.GetCurrAnimation() == "Armor Player dashing"))
		{
			m_ts.SetPlaying(true);
			m_ts.ResetCurrFrame();
			if (m_bHasArmor == false)
			{
				m_ts.SetCurrAnimation("Idle");
			}
			else
			{
				m_ts.SetCurrAnimation("Armor Player Idle");
			}
		}
	}
}

void Player::UpdateJump(float elapsedTime)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	if (pInput->IsButtonDown(0, 0))
	{
		//SGD::GraphicsManager::GetInstance()->DrawString("PRESSED A", { 300, 300 }, { 255, 255, 0, 0 });
	}

	if (pInput->IsButtonDown(0, 0 /*A button on Xbox*/) == true)
	{
		int tempx = 0;
	}

	///////Check if on ramp first
	//RECT rObject;
	//rObject.left = (LONG)pOther->GetRect().left;
	//rObject.top = (LONG)pOther->GetRect().top;
	//rObject.right = (LONG)pOther->GetRect().right;
	//rObject.bottom = (LONG)pOther->GetRect().bottom;
	//
	////Create a rectangle for the intersection
	//RECT rIntersection = {};
	//
	//RECT rPlayerWall;
	//rPlayerWall.left = (LONG)GetRect().left - 1;
	//rPlayerWall.top = (LONG)GetRect().top;
	//rPlayerWall.right = (LONG)GetRect().right + 1;
	//rPlayerWall.bottom = (LONG)GetRect().bottom;
	//
	//IntersectRect(&rIntersection, &rObject, &rPlayerWall);
	//
	//int nIntersectWidth = rIntersection.right - rIntersection.left;
	//int nIntersectHeight = rIntersection.bottom - rIntersection.top;






	if (pInput->IsKeyDown(SGD::Key::Space) == true
		|| pInput->IsButtonDown(0, 0 /*A button on Xbox*/) == true)

	{
		m_fButtonTimer += elapsedTime;
		//if(GetIsJumping() == false)
		if (m_unCurrentState == RESTING_STATE
			|| m_unCurrentState == LANDING_STATE)
		{
			m_ts.ResetCurrFrame();
			m_ts.SetPlaying(false);
			if (m_bHasArmor == false)
			{
				m_ts.SetCurrAnimation("Jump");
			}
			else
			{
				m_ts.SetCurrAnimation("Armor Player Jump");
			}
			if (m_bSlowed == true)
			{
				m_fJumpTimer = 0.35f;

			}
			else
				m_fJumpTimer = 0.3f;
			m_unCurrentState = JUMPING_STATE;


		}

		if (m_unCurrentState == JUMPING_STATE)
		{

			if (m_fJumpTimer < 0.2f)
			{
				SetVelocity({ GetVelocity().x, GetVelocity().y + (2200 * elapsedTime) });
				if (GetVelocity().y > 0)
				{
					SetVelocity({ GetVelocity().x, 0 });

				}

			}
			else
				SetVelocity({ GetVelocity().x, -600 });

		}

		//if (m_fInputTimer <= 0.20f)
		//{
		if (GetIsInputStuck() == true)
		{

			if (is_Right_Coll == true
				&& m_fButtonTimer < 0.08)
			{
				SetJumpVelCur(GetJumpVelCur() - 2000 * elapsedTime);
				SetVelocity({ -900, -900 });
			}

			if (is_Left_Coll == true
				&& m_fButtonTimer < 0.08)
			{
				SetJumpVelCur(GetJumpVelCur() - 2000 * elapsedTime);
				SetVelocity({ 900, -900 /*GetJumpVelCur() */ });
			}
		}
		//}
	}
}

void Player::UpdateHawk(float elapsedTime)
{
	if (HasHawk() == true)
	{
		SGD::InputManager* pInput = SGD::InputManager::GetInstance();
		if (pInput->IsKeyDown(SGD::Key::R) == true
			&& m_fShotTimer > 0.25f)
		{
			m_fShotTimer = 0.0f;
			CreateProjectileMessage* pMsg = new CreateProjectileMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
		}

		float triggerOff = pInput->GetTrigger(0);
		//= pInput->GetLeftJoystick(0).x;


		if (pInput->IsKeyDown(SGD::Key::D) == true
			|| triggerOff > 0 )//|| pInput->IsButtonDown(0, 5/*5*/ /*Right bumper on xbox controller*/)/*JOYSTICK_DEADZONE*/)
		{

			if (m_bHawkCast == false
				&& m_fHawkTimer > 1.0f)
			{
				//Dont spawn another hawk until particles are done
				if (!m_bReturningHawk)
				{
					m_bHawkCast = true;
					m_bHawkExplode = false;
					CreateHawkMessage* pMsg = new CreateHawkMessage(this);
					pMsg->QueueMessage();
					pMsg = nullptr;
				}
			}
			else
			{
				if (GetHawkPtr() != nullptr)
				{
					float rightStickXOff = pInput->GetRightJoystick(0).x;
					float rightStickYOff = pInput->GetRightJoystick(0).y;


					if (pInput->IsKeyDown(SGD::Key::LeftArrow) == true)
					{

						GetHawkPtr()->SetVelocity({ -200, GetHawkPtr()->GetVelocity().y });

					}
					if (pInput->IsKeyDown(SGD::Key::RightArrow) == true)
					{
						GetHawkPtr()->SetVelocity({ 200, GetHawkPtr()->GetVelocity().y });
					}

					if (pInput->IsKeyDown(SGD::Key::UpArrow) == true)
					{
						GetHawkPtr()->SetVelocity({ GetHawkPtr()->GetVelocity().x, -300 });
					}
					if (pInput->IsKeyDown(SGD::Key::DownArrow) == true)
					{

						GetHawkPtr()->SetVelocity({ GetHawkPtr()->GetVelocity().x, 300 });


					}


					GetHawkPtr()->SetVelocity(SGD::Vector(GetHawkPtr()->GetVelocity().x + (GetHawkPtr()->GetSpeed() * rightStickXOff) * elapsedTime, GetHawkPtr()->GetVelocity().y + (GetHawkPtr()->GetSpeed() * rightStickYOff) * elapsedTime));



					// X Friction

					if(rightStickXOff == 0 && !(pInput->IsKeyDown(SGD::Key::Left) || pInput->IsKeyDown(SGD::Key::Right) ))
					{

						if (GetHawkPtr()->GetVelocity().x < 0)
						{
							GetHawkPtr()->SetVelocity(SGD::Vector(GetHawkPtr()->GetVelocity().x + (GetHawkPtr()->GetAirFriction() * 8), GetHawkPtr()->GetVelocity().y));

							if (GetHawkPtr()->GetVelocity().x > 0.007f)
							{
								GetHawkPtr()->SetVelocity(SGD::Vector(0, GetHawkPtr()->GetVelocity().y));

							}
						}
						if (GetHawkPtr()->GetVelocity().x > 0)
						{
							GetHawkPtr()->SetVelocity(SGD::Vector(GetHawkPtr()->GetVelocity().x - (GetHawkPtr()->GetAirFriction() * 8), GetHawkPtr()->GetVelocity().y));

							if (GetHawkPtr()->GetVelocity().x < -0.007f)
							{
								GetHawkPtr()->SetVelocity(SGD::Vector(0, GetHawkPtr()->GetVelocity().y));

							}
						}

					}


					// Y Friction

					if(rightStickYOff == 0 && !(pInput->IsKeyDown(SGD::Key::Up) || pInput->IsKeyDown(SGD::Key::Down)))
					{
						if (GetHawkPtr()->GetVelocity().y < 0)
						{
							//GetHawkPtr()->SetVelocity(SGD::Vector(GetHawkPtr()->GetVelocity().x, GetHawkPtr()->GetVelocity().y + GetHawkPtr()->GetAirFriction()));

							if (GetHawkPtr()->GetVelocity().y > 0.007f)
							{

							}
							GetHawkPtr()->SetVelocity(SGD::Vector(GetHawkPtr()->GetVelocity().x, 0));

						}
						if (GetHawkPtr()->GetVelocity().y > 0)
						{
							//GetHawkPtr()->SetVelocity(SGD::Vector(GetHawkPtr()->GetVelocity().x, GetHawkPtr()->GetVelocity().y - GetHawkPtr()->GetAirFriction()));

							if (GetHawkPtr()->GetVelocity().y < -0.007f)
							{


							}

							GetHawkPtr()->SetVelocity(SGD::Vector(GetHawkPtr()->GetVelocity().x, 0));

						}

					}



				}

			}

		}
		if (triggerOff <= 0
			&& pInput->IsKeyDown(SGD::Key::D) == false)
		{
			m_bHawkCast = false;

			//if HawkPtr != null set to NULL
			if (GetHawkPtr() != nullptr)
			{
				m_fHawkTimer = 0.0f;

				if (!m_bReturningHawk)
				{
					//Emitter Stuff
					m_bHawkExplode = true;
					m_emFeatherExplosion->Burst(GetHawkPtr()->GetPosition());
					m_emHawkReturn->Burst(GetHawkPtr()->GetPosition());
					m_bReturningHawk = true;
				}
				else
				{
					m_bHawkExplode = false;
				}


				

			}

		}

		if (m_bHawkCast == false)
		{
			if (GetHawkPtr() != nullptr)
			{



				DestroyEntityMessage* pMsg = new DestroyEntityMessage{ GetHawkPtr() };
				pMsg->QueueMessage();
				pMsg = nullptr;

				SetHawkPtr(nullptr);
			}
		}


		//Emitter Hawk Returning changing the position with velocity
		if (m_bReturningHawk)
		{
			SGD::Vector distance = m_ptPosition - m_emHawkReturn->GetPosition();
			if (distance.ComputeLength() > 10)
			{
				SGD::Vector VEL = { 0, 0 };
				if (m_ptPosition.x < m_emHawkReturn->GetPosition().x)
				{
					VEL.x = -600;
				}
				if (m_ptPosition.y < m_emHawkReturn->GetPosition().y)
				{
					VEL.y = -600;
				}
				if (m_ptPosition.x > m_emHawkReturn->GetPosition().x)
				{
					VEL.x = 600;
				}
				if (m_ptPosition.y > m_emHawkReturn->GetPosition().y)
				{
					VEL.y = 600;
				}
				SGD::Point TempPos = m_emHawkReturn->GetPosition();
				TempPos += VEL * elapsedTime;
				m_emHawkReturn->SetPosition(TempPos);
				m_emHawkReturn->Finish(false);
			}
			else
			{
				m_emHawkReturn->Finish();
				m_bReturningHawk = false;
			}
		}
	}

}

void Player::UpdateSpray(float elapsedTime)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	float triggerOff = pInput->GetTrigger(0);

	if ((pInput->IsKeyDown(SGD::Key::F) == true
		/*&& m_fShotTimer > 0.25f*/ || triggerOff < 0) && m_bHasIce == true)// || pInput->IsButtonDown(0, 2/*5*/ /*Right bumper on xbox controller*/))
	{
		if (!(SGD::AudioManager::GetInstance()->IsAudioPlaying(m_hIceEffect)))
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_hIceEffect);
		}
		//m_fShotTimer = 0.0f;
		if (m_fIceTimer > 1.0f)
		{
			m_fIceTimer = 0;
			CreateSprayMessage* pMsg = new CreateSprayMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
		}
	}
}

void Player::UpdateConstants(float elapsedTime)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	if (m_unCurrentState != JUMPING_STATE)
	{
		SetGravity(-3000);
		SetVelocity({ GetVelocity().x, GetVelocity().y - GetGravity() * elapsedTime });
	}

	if ((m_unCurrentState == JUMPING_STATE
		&& m_fJumpTimer == 0.0f
		|| pInput->IsKeyDown(SGD::Key::Space) == false
		&& pInput->IsButtonDown(0, 0 /*A button on Xbox*/) == false)
		&& GetVelocity().y <= 0
		&& m_unCurrentState == JUMPING_STATE
		/*&& GetIsInputStuck() == true*/)
	{

		m_unCurrentState = FALLING_STATE;

		if (GetVelocity().y < 0)
		{
			//SetVelocity({ GetVelocity().x, 0 });

		}
	}
}

void Player::UpdateVelocity(float elapsedTime)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	if (m_unCurrentState == LANDING_STATE
		&& m_fLandTimer <= 0
		&& pInput->IsKeyDown(SGD::Key::Space) == false
		&& pInput->IsButtonDown(0, 0 /*A button on Xbox*/) == false
		)
	{
		if (m_bHasArmor == false)
		{
			m_ts.SetCurrAnimation("Idle");
		}
		else
		{
			m_ts.SetCurrAnimation("Armor Player Idle");
		}
		m_unCurrentState = RESTING_STATE;
	}

	//Buffer Input for Wall Jump so you can't jump while holding the JUMP button
	if (pInput->IsKeyDown(SGD::Key::Space) == false
		&& pInput->IsButtonDown(0, 0 /*A button on Xbox*/) == false)
	{
		m_fButtonTimer = 0;
		//SGD::GraphicsManager::GetInstance()->DrawString("ZERO", { 100, 300 }, { 255, 255, 0, 0 });

	}

	if (GetVelocity().y > 1050)
	{
		SetVelocity(SGD::Vector(GetVelocity().x, 1050));
	}

	if (m_unCurrentState == RESTING_STATE
		|| m_unCurrentState == LANDING_STATE)
	{


		if(GetVelocity().x > 850 && m_bSliding == false)
		{
			if (IsDashing() == false)
				SetVelocity(SGD::Vector(850, GetVelocity().y));
		}
		if(GetVelocity().x > 1600 && m_bSliding == true)
		{
			if(IsDashing() == false)
				SetVelocity(SGD::Vector(1600, GetVelocity().y));
		}

		if(GetVelocity().x < -850 && m_bSliding == false)
		{
			if (IsDashing() == false)
				SetVelocity(SGD::Vector(-850, GetVelocity().y));
		}
		if(GetVelocity().x < -1600 && m_bSliding == true)
		{
			if (IsDashing() == false)

				SetVelocity(SGD::Vector(-1600, GetVelocity().y));
		}

		if (IsDashing() == true)
		{
			if (GetVelocity().x > 2000)
			{
				SetVelocity({ 2000, GetVelocity().y });
			}
			if (GetVelocity().x < -2000)
			{
				SetVelocity({ -2000, GetVelocity().y });
			}
		}
	}
	else
	{
		if (GetVelocity().x > 1150 && m_bSliding == false)
		{
			if (IsDashing() == false)
				SetVelocity(SGD::Vector(1150, GetVelocity().y));
		}
		if(GetVelocity().x > 1475 && m_bSliding == true)
		{
			if(IsDashing() == false)
				SetVelocity(SGD::Vector(1475, GetVelocity().y));
		}

		if (GetVelocity().x < -1150 && m_bSliding == false)
		{
			if (IsDashing() == false)
				SetVelocity(SGD::Vector(-1150, GetVelocity().y));
		}
		if(GetVelocity().x < -1475 && m_bSliding == true)
		{
			if (IsDashing() == false)

				SetVelocity(SGD::Vector(-1475, GetVelocity().y));
		}

	}

}

void Player::HawkExplode(SGD::Point _pos)
{
	if (!m_bHawkExplode)
	{
		m_bHawkExplode = true;
		m_emFeatherExplosion->Finish();
		m_emFeatherExplosion->Burst(_pos);
		m_bReturningHawk = true;
		m_emHawkReturn->Burst(_pos);
		m_emHawkReturn->Finish();
	}
}

#pragma endregion

void Player::UpdatePlayerSwing(float elapsedTime)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsKeyPressed(SGD::Key::G) == true
		|| pInput->IsButtonPressed(0, 1 /*B button on Xbox*/) == true)
	{
		if (m_fSwingTimer <= 0.0f)
		{
			m_fSwingTimer = 0.35f;
			is_Swinging = true;
			if (rand() % 2 == 0)
			{
				if (m_bHasArmor == false)
				{
					m_ts.SetCurrAnimation("Swing Attack");
					m_ts.SetPlaying(true);
					m_ts.ResetCurrFrame();
				}
				else
				{
					m_ts.SetCurrAnimation("Armor Player Swing");
					m_ts.SetPlaying(true);
					m_ts.ResetCurrFrame();
				}
			}
			else
			{
				if (m_bHasArmor == false)
				{
					m_ts.SetCurrAnimation("Slash Attack");
					m_ts.SetPlaying(true);
					m_ts.ResetCurrFrame();
				}
				else
				{
					m_ts.SetCurrAnimation("Armor Player Swipe");
					m_ts.SetPlaying(true);
					m_ts.ResetCurrFrame();
				}
			}
		}


	}

	if (m_fSwingTimer > 0.0f)
	{
		if (IsFacingRight() == true)
		{

			//SGD::Rectangle temp = GetRect();

			SGD::Rectangle temp;
			temp.left = ((GetRect().right - 16) - Camera::GetInstance()->GetCameraPos().x);
			temp.top = (GetRect().top - Camera::GetInstance()->GetCameraPos().y) - 10;
			temp.right = (temp.left) + 80;
			temp.bottom = (temp.top) + 80;

			//swingRect = temp;
			m_pSword->SetRect(temp);

		}
		else
		{

			SGD::Rectangle temp;
			temp.right = (GetRect().left + 16) - Camera::GetInstance()->GetCameraPos().x;
			temp.left = temp.right - 80;



			//temp.left = (GetRect().left + 12) - Camera::GetInstance()->GetCameraPos().x;
			temp.top = (GetRect().top - Camera::GetInstance()->GetCameraPos().y) - 10;
			//temp.right = temp.left - 60;
			temp.bottom = temp.top + 80;

			//swingRect = temp;
			m_pSword->SetRect(temp);


		}
	}
	else
	{
		swingRect = { 0, 0, 0, 0 };
		m_pSword->SetRect(swingRect);

	}


	m_pSword->Update(elapsedTime);

}

void Player::SetHasBounce(bool bounce)
{
	m_bHasBounce = bounce;
	if (bounce == true)
	{
		SGD::AudioManager::GetInstance()->PlayAudio(m_hGainAbility);
	}
}

void Player::UpdateSnared(float elapsedTime)
{
	m_fSnareTimer += elapsedTime;
	if (m_fSnareTimer > 1)
	{
		m_bSnared = false;
	}
}




#pragma endregion
////////////////////////////////
// ThrowPlayer
// -Tosses the player left or right and kills him
void Player::ThrowPlayer(bool _right)
{
	float throwSpeed = -3000;
	if (_right)
	{
		throwSpeed = 3000;
	}
	SetGravity(0);
	SetPosition({ m_ptPosition.x, m_ptPosition.y - 1 });
	SetVelocity({ throwSpeed, -1000 });
	SetStunnded(true);
	m_fStunTimer = 0.5f;
}

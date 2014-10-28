#include "Player.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "DestroyEntityMessage.h"
#include "CreateProjectileMessage.h"
#include "CreateSprayMessage.h"
#include "CreateHawkMessage.h"
#include "LevelCollider.h"
#include "Hawk.h"
#include "AnimationEngine.h"
#include "Font.h"
#include "BitmapFont.h"
#include "Game.h"

#include <Windows.h>
#include "Dash.h"
#include "Camera.h"
#include "Honor.h"
#include "Jellyfish.h"


#define JOYSTICK_DEADZONE  0.2f

Player::Player() : Listener(this)
{
	Listener::RegisterForEvent("KILL_PLAYER");
	SetDirection({ 1, 0 });
	m_pDash = new Dash;
	AnimationEngine::GetInstance()->LoadAnimation("Assets/CollisionTesting.xml");
	m_ts.SetCurrAnimation("Idle");
}


Player::~Player()
{
	delete m_pDash;
}

/////////////////////////////////////////////////
/////////////////Interface///////////////////////
void Player::Update(float elapsedTime)
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();



	m_fJumpTimer -= elapsedTime;

	if(m_fJumpTimer < 0.0f)
		m_fJumpTimer = 0;


	m_fHawkTimer += elapsedTime;


	float leftStickXOff = pInput->GetLeftJoystick(0).x;
	float leftStickYOff = pInput->GetLeftJoystick(0).y;

	bool leftClamped = false;
	static int stickFrame = 1;
	bool controller = pInput->IsControllerConnected(0);

	if(leftStickXOff > -JOYSTICK_DEADZONE && leftStickXOff < JOYSTICK_DEADZONE)
	{
		leftStickXOff = 0;
		leftClamped = true;
		stickFrame = 5;
	}

	SetIsBouncing(false);

	//////// NEED TO UPDATE WITH CONTROLLER ( JORDAN )
	if(GetIsInputStuck() == false)
		m_fInputTimer = 0;

	if(GetIsInputStuck() == true
		&& GetVelocity().y < 0)
	{
		SetVelocity({ /*GetVelocity().x*/ 0, 0 });
	}


	/////////////////////////////////////////////////
	/////////////////Controls////////////////////////

	if(IsDashing() == false)///////////////Dash check begins
	{

		if(GetIsFalling() == false
			&& GetIsJumping() == false)
		{
			/////////////////////////////////////////////////
			/////////////////Friction////////////////////////

			//Left Friction
			if(GetVelocity().x < 0
				&& (pInput->IsKeyDown(SGD::Key::Q) == false
				&& leftClamped == true))
			{

				SetVelocity(SGD::Vector(GetVelocity().x + GetFriction(), GetVelocity().y));

				if(GetVelocity().x > 0
					&& GetVelocity().x < 10)
				{
					SetVelocity(SGD::Vector(0, GetVelocity().y));
				}

			}
			//Right Friction
			if(GetVelocity().x > 0
				&& (pInput->IsKeyDown(SGD::Key::E) == false
				&& leftClamped == true))
			{

				SetVelocity(SGD::Vector(GetVelocity().x - GetFriction(), GetVelocity().y));

				if(GetVelocity().x < 0
					&& GetVelocity().x > -10)
				{
					SetVelocity(SGD::Vector(0, GetVelocity().y));
				}
			}
		}


		if(pInput->IsKeyDown(SGD::Key::W) == true)
		{
			SetIsBouncing(true);
		}


		/////////////////////////////////////////////////
		/////////////////Movement////////////////////////
		//reset currframe to 0 & set the animation playing to true
		if (IsDashing() == false)
		{
			if (pInput->IsKeyPressed(SGD::Key::E) == true || pInput->IsKeyPressed(SGD::Key::Q) == true || (stickFrame == 5 && leftClamped == false))
			{
				stickFrame = 1;
				m_ts.ResetCurrFrame();
				m_ts.SetPlaying(true);
			}
			//reset currframe to 0 & set the animation playing to false
			if ((pInput->IsKeyDown(SGD::Key::E) == true || pInput->IsKeyDown(SGD::Key::Q) == true))
			{
				leftClamped = false;
			}
			if (pInput->IsKeyReleased(SGD::Key::E) == true || pInput->IsKeyReleased(SGD::Key::Q) == true)
			{
				m_ts.SetPlaying(false);
				m_ts.ResetCurrFrame();
				m_ts.SetCurrAnimation("Idle");
			}
			else if (leftClamped == true)
			{
				m_ts.SetPlaying(false);
				m_ts.ResetCurrFrame();
				m_ts.SetCurrAnimation("Idle");
			}


			//Right Movement
			if (pInput->IsKeyDown(SGD::Key::E) == true
				|| leftStickXOff > JOYSTICK_DEADZONE)
			{

				if (GetIsInputStuck() == true)
					m_fInputTimer += elapsedTime;


				if (m_fInputTimer > 0.20f
					|| GetIsInputStuck() == false)
				{
					if (GetVelocity().x < 0)
					{
						SetVelocity(SGD::Vector(GetVelocity().x + (5 * GetSpeed() * elapsedTime), GetVelocity().y));

					}
					else
						SetVelocity(SGD::Vector(GetVelocity().x + GetSpeed() * elapsedTime, GetVelocity().y));
					SetDirection({ 1, 0 });
				}
				m_ts.SetCurrAnimation("Walking");
				SetFacingRight(true);
			}

			//Left Movement
			if (pInput->IsKeyDown(SGD::Key::Q) == true
				|| leftStickXOff < -JOYSTICK_DEADZONE)
			{
				if (GetIsInputStuck() == true)
					m_fInputTimer += elapsedTime;

				if (m_fInputTimer > 0.20f
					|| GetIsInputStuck() == false)
				{
					if (GetVelocity().x > 0)
					{
						SetVelocity(SGD::Vector(GetVelocity().x - (5 * GetSpeed() * elapsedTime), GetVelocity().y));
					}
					else
					{
						SetVelocity(SGD::Vector(GetVelocity().x - GetSpeed() * elapsedTime, GetVelocity().y));
					}
					SetDirection({ -1, 0 });
				}
				m_ts.SetCurrAnimation("Walking");
				SetFacingRight(false);
			}
		}

		m_fShotTimer += elapsedTime;


		if(pInput->IsKeyDown(SGD::Key::Tab) == true
			|| pInput->IsButtonPressed(0, 5 /*Right bumper on xbox controller*/))
		{
			CastDash();
			m_ts.SetPlaying(true);
			m_ts.ResetCurrFrame();
			m_ts.SetCurrAnimation("dashing");
		}

		/////////////////////////////////////////////////
		////////////////////Jump/////////////////////////

		if(pInput->IsButtonDown(0, 0))
		{
			SGD::GraphicsManager::GetInstance()->DrawString("PRESSED A", { 300, 300 }, { 255, 255, 0, 0 });
		}


		if(pInput->IsKeyDown(SGD::Key::Space) == true
			|| pInput->IsButtonDown(0, 0 /*A button on Xbox*/) == true)

		{
			if(GetIsJumping() == false)
			{
				m_fJumpTimer = 0.4f;

			//	SetJumpVelCur(GetJumpVelCur() - 2000 * elapsedTime);
				//SetVelocity({ GetVelocity().x, -450});
				//SetIsJumping(true);
				SetVelocity({ GetVelocity().x, -900 });


			}

			if(GetIsFalling() == false)
			{
				//SetJumpVelCur(GetJumpVelCur() - 2000 * elapsedTime);
				//SetVelocity({ GetVelocity().x, GetJumpVelCur() });
				
				if (m_fJumpTimer < 0.1f)
				{
					SetVelocity({ GetVelocity().x, GetVelocity().y + (9000 * elapsedTime) });
					if (GetVelocity().y > 0)
					{
						SetVelocity({ GetVelocity().x, 0 });

					}

				}


				SetIsJumping(true);
			}

			if(m_fInputTimer <= 0.20f)
			{
				if(GetIsInputStuck() == true)
				{

					if(/*pInput->IsKeyDown(SGD::Key::Q) == true
						&&*/ is_Right_Coll == true)
					{
						SetJumpVelCur(GetJumpVelCur() - 2000 * elapsedTime);
						SetVelocity({ -600, GetJumpVelCur() });
					}

					if(/*pInput->IsKeyDown(SGD::Key::E) == true
						&&*/ is_Left_Coll == true)
					{
						SetJumpVelCur(GetJumpVelCur() - 2000 * elapsedTime);
						SetVelocity({ 600, GetJumpVelCur() });
					}
				}
			}
		}

		/////////////////////////////////////////////////
		///////////////////Shoot/////////////////////////

		if(pInput->IsKeyDown(SGD::Key::R) == true
			&& m_fShotTimer > 0.25f)
		{
			m_fShotTimer = 0.0f;
			CreateProjectileMessage* pMsg = new CreateProjectileMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
		}


		if(pInput->IsKeyDown(SGD::Key::D) == true
			/*&& m_fShotTimer > 0.25f*/)
		{

			if(m_bHawkCast == false
				&& m_fHawkTimer > 1.0f)
			{

				m_bHawkCast = true;
				CreateHawkMessage* pMsg = new CreateHawkMessage(this);
				pMsg->QueueMessage();
				pMsg = nullptr;
			}
			else
			{
				if(GetHawkPtr() != nullptr)
				{

					if(pInput->IsKeyDown(SGD::Key::LeftArrow) == true)
					{
						GetHawkPtr()->SetVelocity(SGD::Vector(GetHawkPtr()->GetVelocity().x - GetHawkPtr()->GetSpeed() * elapsedTime, GetVelocity().y));
					}
					if(pInput->IsKeyDown(SGD::Key::RightArrow) == true)
					{
						GetHawkPtr()->SetVelocity(SGD::Vector(GetHawkPtr()->GetVelocity().x + GetHawkPtr()->GetSpeed() * elapsedTime, GetHawkPtr()->GetVelocity().y));
					}

					if(pInput->IsKeyDown(SGD::Key::UpArrow) == true)
					{
						GetHawkPtr()->SetVelocity(SGD::Vector(GetHawkPtr()->GetVelocity().x, GetHawkPtr()->GetVelocity().y - GetHawkPtr()->GetSpeed() * elapsedTime));
					}
					if(pInput->IsKeyDown(SGD::Key::DownArrow) == true)
					{
						GetHawkPtr()->SetVelocity(SGD::Vector(GetHawkPtr()->GetVelocity().x, GetHawkPtr()->GetVelocity().y + GetHawkPtr()->GetSpeed() * elapsedTime));
					}


				}

			}

		}
		if(pInput->IsKeyDown(SGD::Key::D) == false
			/*&& m_fShotTimer > 0.25f*/)
		{
			m_bHawkCast = false;

			//if HawkPtr != null set to NULL
			if(GetHawkPtr() != nullptr)
			{
				m_fHawkTimer = 0.0f;


				DestroyEntityMessage* pMsg = new DestroyEntityMessage{ GetHawkPtr() };
				pMsg->QueueMessage();
				pMsg = nullptr;

				SetHawkPtr(nullptr);
			}

		}

		/////////////////////////////////////////////////
		///////////////////Spray/////////////////////////

		if(pInput->IsKeyDown(SGD::Key::F) == true
			/*&& m_fShotTimer > 0.25f*/)
		{
			//m_fShotTimer = 0.0f;


			CreateSprayMessage* pMsg = new CreateSprayMessage(this);
			pMsg->QueueMessage();
			pMsg = nullptr;
		}




		/////////////////////////////////////////////////
		//////////////Constant Updates///////////////////

		if(GetIsFalling() == true)
		{
			SetGravity(-3000);


			SetVelocity({ GetVelocity().x, GetVelocity().y - GetGravity() * elapsedTime });
		}

		if ((m_fJumpTimer == 0.0f
			|| pInput->IsKeyDown(SGD::Key::Space) == false
			&& pInput->IsButtonDown(0, 0 /*A button on Xbox*/) == false)
			&& GetVelocity().y <= 0
			/*&& GetIsInputStuck() == true*/)
		{
			SetIsFalling(true);

			if (GetVelocity().y < 0)
			{
				SetVelocity({ GetVelocity().x, 0 });
			}

		}


	}


	if(GetVelocity().y > 1050)
	{
		SetVelocity(SGD::Vector(GetVelocity().x, 1050));
	}

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

void Player::Render(void)
{
	//SGD::GraphicsManager* pGraphics = SGD::GraphicsManager::GetInstance();

	////Camera::GetInstance()->Draw(SGD::Rectangle(10, 300, 20, 320), SGD::Color::Color(255, 0, 0, 255));

	Camera::GetInstance()->Draw(SGD::Rectangle(
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y,
		m_ptPosition.x - Camera::GetInstance()->GetCameraPos().x + GetSize().width, m_ptPosition.y - Camera::GetInstance()->GetCameraPos().y + GetSize().height),
		SGD::Color::Color(255, 255, 0, 0));

	Camera::GetInstance()->DrawAnimation(m_ptPosition, 0, m_ts, IsFacingRight());

	// Draw gui for amount of honor
	SGD::OStringStream output;
	output << "Honor: " << m_unHonorCollected;
	//Local refernce to the font
	Font font = Game::GetInstance()->GetFont()->GetFont("HonorFont_0.png");

	//Draw the title
	font.DrawString(output.str().c_str(), 32, 32, 1, SGD::Color{ 255, 255, 0, 0 });

}




SGD::Rectangle Player::GetRect(void) const
{
	return{ m_ptPosition, m_szSize };
}

void Player::HandleCollision(const IEntity* pOther)
{

	Unit::HandleCollision(pOther);
	if(pOther->GetType() == ENT_DOOR)
	{
		BasicCollision(pOther);
	}

	if (pOther->GetType() == ENT_JELLYFISH)
	{
		JellyfishCollision(pOther);
	}

	if(pOther->GetType() == ENT_BOSS_DOOR)
	{
		BasicCollision(pOther);
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
	}

	if(pOther->GetType() == Entity::ENT_SOLID_WALL)
	{
		is_Platform = true;
		BasicCollision(pOther);
		SetFriction(1.0f);
	}

	if (pOther->GetType() == Entity::ENT_MOVING_PLATFORM)
	{
		is_Platform = true;
		BasicCollision(pOther);
		SetFriction(1.0f);
	}

	if (pOther->GetType() == Entity::ENT_BLOCK)
	{
		is_Platform = true;
		BasicCollision(pOther);
		SetFriction(1.0f);
	}

	if (pOther->GetType() == Entity::ENT_FROZEN)
	{
		is_Platform = true;
		BasicCollision(pOther);
		SetFriction(0.1f);
	}

	if (pOther->GetType() == Entity::ENT_NOT_FROZEN)
	{
		is_Platform = true;
		BasicCollision(pOther);
		SetFriction(1.0f);
	}

	if(pOther->GetType() == Entity::ENT_DEATH)
	{
		//Kill the player
		m_ptPosition = m_ptStartPosition;
	}

	if(pOther->GetType() == Entity::ENT_RIGHT_RAMP)
	{
		RightRampCollision(pOther);
		SetFriction(1.0f);
		is_Ramp = true;
	}

	if(pOther->GetType() == Entity::ENT_LEFT_RAMP)
	{
		LeftRampCollision(pOther);
		SetFriction(1.0f);
		is_Ramp = true;
	}

	if(pOther->GetType() == Entity::ENT_ICE)
	{
		SetFriction(0.1f);
	}

	if(pOther->GetType() == Entity::ENT_ICE_LEFT_RAMP)
	{
		LeftRampCollision(pOther);
		is_Ramp = true;
		SetFriction(0.1f);
	}

	if(pOther->GetType() == Entity::ENT_RIGHT_RAMP)
	{
		RightRampCollision(pOther);
		SetFriction(1.0f);
		is_Ramp = true;
	}

	if (pOther->GetType() == Entity::ENT_GEYSER)
	{
		is_Platform = true;
		GeyserCollision(pOther);
		//BasicCollision(pOther);
		SetFriction(1.0f);

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

	if (pOther->GetType() == Entity::ENT_LAVA)
	{
		//if so move back up but kill the player
		SGD::Event Event = { "KILL_PLAYER", nullptr, this };
		SGD::EventManager::GetInstance()->SendEventNow(&Event);
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
		rPlayerWall.left = (LONG)GetRect().left -1;
		rPlayerWall.top = (LONG)GetRect().top;
		rPlayerWall.right = (LONG)GetRect().right + 1;
		rPlayerWall.bottom = (LONG)GetRect().bottom;
	
		IntersectRect(&rIntersection, &rObject, &rPlayerWall);

	int nIntersectWidth = rIntersection.right - rIntersection.left;
	int nIntersectHeight = rIntersection.bottom - rIntersection.top;

	if (nIntersectHeight > nIntersectWidth)
	{
		if (GetIsFalling() == true
			|| GetIsJumping() == true)
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

			SetPosition({ (float)rObject.left - GetSize().width + 1, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
			//SetDashTimer(0);

			is_Right_Coll = true;
		}
		if (rPlayer.left == rIntersection.left)
		{
			SetPosition({ (float)rObject.right, GetPosition().y });
			SetVelocity({ 0, GetVelocity().y });
			//SetDashTimer(0);

			is_Left_Coll = true;

		}
	}

	if (nIntersectWidth > nIntersectHeight)
	{
		if (rPlayer.bottom == rIntersection.bottom)
		{
			if (IsBouncing() == true)
			{
				SetVelocity({ GetVelocity().x, GetVelocity().y * -1 });
				SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height  /*- nIntersectHeight*/ });
			}

			else
			{
				SetVelocity({ GetVelocity().x, 0 });
				SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 /*- nIntersectHeight*/ });
			}


			SetJumpVelCur(0);
			SetIsJumping(false);
			SetIsFalling(false);
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

	//RECT rPlayer;
	//rPlayer.left = (LONG)GetRect().left;
	//rPlayer.top = (LONG)GetRect().top;
	//rPlayer.right = (LONG)GetRect().right;
	//rPlayer.bottom = (LONG)GetRect().bottom;

	////Create a rectangle for the other object
	//RECT rObject;
	//rObject.left = (LONG)pOther->GetRect().left;
	//rObject.top = (LONG)pOther->GetRect().top;
	//rObject.right = (LONG)pOther->GetRect().right;
	//rObject.bottom = (LONG)pOther->GetRect().bottom;

	////Create a rectangle for the intersection
	//RECT rIntersection = {};

	////	RECT rPlayerWall;
	////	rPlayerWall.left = (LONG)GetRect().left - 1;
	////	rPlayerWall.top = (LONG)GetRect().top;
	////	rPlayerWall.right = (LONG)GetRect().right + 1;
	////	rPlayerWall.bottom = (LONG)GetRect().bottom;
	////
	////	IntersectRect(&rIntersection, &rPlayer, &rPlayerWall);

	//int nIntersectWidth = rIntersection.right - rIntersection.left;
	//int nIntersectHeight = rIntersection.bottom - rIntersection.top;

	////if (nIntersectHeight > nIntersectWidth)
	////{
	////	if (GetIsFalling() == true
	////		|| GetIsJumping() == true)
	////		SetIsInputStuck(true);
	////
	////}

	//IntersectRect(&rIntersection, &rPlayer, &rObject);

	//nIntersectWidth = rIntersection.right - rIntersection.left;
	//nIntersectHeight = rIntersection.bottom - rIntersection.top;

	////Colliding with the side of the object
	//if(nIntersectHeight > nIntersectWidth)
	//{
	//	if(rPlayer.right == rIntersection.right)
	//	{

	//		SetPosition({ (float)rObject.left - GetSize().width + 1, GetPosition().y });
	//		SetVelocity({ 0, GetVelocity().y });
	//		//SetDashTimer(0);

	//		is_Right_Coll = true;
	//	}
	//	if(rPlayer.left == rIntersection.left)
	//	{
	//		SetPosition({ (float)rObject.right, GetPosition().y });
	//		SetVelocity({ 0, GetVelocity().y });
	//		//SetDashTimer(0);

	//		is_Left_Coll = true;

	//	}
	//}

	//if(nIntersectWidth > nIntersectHeight)
	//{
	//	if(rPlayer.bottom == rIntersection.bottom)
	//	{

	//		if(IsBouncing() == true)
	//		{
	//			SetVelocity({ GetVelocity().x, GetVelocity().y * -1 });
	//			//				SetJumpVelCur(GetJumpVelCur() * -1);
	//			SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height  /*- nIntersectHeight*/ });

	//		}

	//		else
	//		{
	//			SetVelocity({ GetVelocity().x, 0 });
	//			SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 /*- nIntersectHeight*/ });

	//		}

	//		SetJumpVelCur(0);
	//		SetIsJumping(false);
	//		SetIsFalling(false);
	//		SetIsInputStuck(false);

	//		is_Left_Coll = false;
	//		is_Right_Coll = false;
	//	}
	//	if(rPlayer.top == rIntersection.top)
	//	{
	//		SetPosition({ GetPosition().x, (float)rObject.bottom });
	//		SetVelocity({ GetVelocity().x, 0 });
	//	}
	//}

}

void Player::LeftRampCollision(const IEntity* pOther)
{
	///////for LEFT ramp RIGHT side of player

	float tempVal = 32.0f / 32.0f;


	//SetGravity(0);

	//RECT rPlayer;
	//rPlayer.left = (LONG)GetRect().left;
	//rPlayer.top = (LONG)GetRect().top;
	//rPlayer.right = (LONG)GetRect().right /*- 16*/;
	//rPlayer.bottom = (LONG)GetRect().bottom /*- 10*/;

	////Create a rectangle for the other object
	//RECT rObject;
	//rObject.left = (LONG)pOther->GetRect().left;
	//rObject.top = (LONG)pOther->GetRect().top;
	//rObject.right = (LONG)pOther->GetRect().right;
	//rObject.bottom = (LONG)pOther->GetRect().bottom;

	////Create a rectangle for the intersection
	//RECT rIntersection = {};


	//IntersectRect(&rIntersection, &rPlayer, &rObject);

	//int nIntersectWidth = rIntersection.right - rIntersection.left;
	//int nIntersectHeight = rIntersection.bottom - rIntersection.top;


	SGD::Rectangle rPlayer = GetRect();
	SGD::Rectangle rOther = pOther->GetRect();
	SGD::Rectangle rIntersecting = rPlayer.ComputeIntersection(rOther);

	float rIntersectWidth = rIntersecting.ComputeWidth();
	float rIntersectHeight = rIntersecting.ComputeHeight();


	//float tempInt = (/*(rObject.right - rObject.left) +*/ nIntersectWidth)* tempVal;

	//	if (is_Platform == false
	//		&& rPlayer.bottom < rObject.top)
	//	{



	if(/*nIntersectWidth*/ rIntersectWidth > 17)
	{
		//SetPosition({ GetPosition().x, (float)rObject.bottom - tempInt - GetSize().height });
		m_ptPosition.y -= rIntersectHeight;
	}
	//else
	//{
	//	//SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 });
	//	m_ptPosition.y -= rIntersectHeight;
	//}



	//	}


	SetVelocity({ 0, 0 });




}




void Player::RightRampCollision(const IEntity* pOther)
{
	///////for RIGHT ramp LEFT side of player

	float tempVal = 32.0f / 32.0f;


	//SetGravity(0);

	//RECT rPlayer;
	//rPlayer.left = (LONG)GetRect().left;
	//rPlayer.top = (LONG)GetRect().top;
	//rPlayer.right = (LONG)GetRect().right /*- 16*/;
	//rPlayer.bottom = (LONG)GetRect().bottom /*- 10*/;

	////Create a rectangle for the other object
	//RECT rObject;
	//rObject.left = (LONG)pOther->GetRect().left;
	//rObject.top = (LONG)pOther->GetRect().top;
	//rObject.right = (LONG)pOther->GetRect().right;
	//rObject.bottom = (LONG)pOther->GetRect().bottom;

	////Create a rectangle for the intersection
	//RECT rIntersection = {};


	//IntersectRect(&rIntersection, &rPlayer, &rObject);


	//int nIntersectWidth = rIntersection.right - rIntersection.left;
	//int nIntersectHeight = rIntersection.bottom - rIntersection.top;



	//float tempInt = nIntersectWidth * tempVal;



	SGD::Rectangle rPlayer = GetRect();
	SGD::Rectangle rOther = pOther->GetRect();
	SGD::Rectangle rIntersecting = rPlayer.ComputeIntersection(rOther);

	float rIntersectWidth = rIntersecting.ComputeWidth();
	float rIntersectHeight = rIntersecting.ComputeHeight();


	if(/*nIntersectWidth*/ rIntersectWidth > 17)
	{
		//SetPosition({ GetPosition().x, (float)rObject.bottom - tempInt - GetSize().height });
		m_ptPosition.y -= rIntersectHeight;
	}
	//else
	//{
	//	//SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height + 1 });
	//	m_ptPosition.y -= rIntersectHeight;
	//}






	SetVelocity({ 0, 0 });



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
			//SetDashTimer(0);

			is_Right_Coll = true;
		}
		if (rPlayer.left == rIntersection.left)
		{

			//SetPosition({ (float)rObject.right, GetPosition().y });

			SetVelocity({ 500 * GetDirection().x * -1, GetVelocity().y - 300 });

			//SetVelocity({ 0, GetVelocity().y });
			//SetDashTimer(0);

			is_Left_Coll = true;

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


			}

			SetJumpVelCur(0);
			SetIsJumping(false);
			SetIsFalling(false);
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
			SetVelocity({ GetVelocity().x, GetVelocity().y * (-1.0f - (0.1f * jfish->GetBounceCount())) });
			SetPosition({ GetPosition().x, (float)rObject.top - GetSize().height /*- nIntersectHeight*/ });
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
		if (m_bHasArmor == false)
		{
			m_ptPosition = m_ptStartPosition;
		}
		else
		{
			m_bHasArmor = false;
		}
	}
}

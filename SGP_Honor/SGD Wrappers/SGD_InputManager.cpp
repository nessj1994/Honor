/***********************************************************************\
|																		|
|	File:			SGD_InputManager.cpp  								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-03-01											|
|																		|
|	Purpose:		To detect keyboard, mouse, and gamepad input		|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#include "SGD_InputManager.h"


// Uses assert for debug breaks
#include <cassert>

// Uses Window's Keyboard & Mouse
// Uses OutputDebugString for debug text
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstring>
#include <cstdio>

// Uses std::vector for gamepads
#include <vector>

// Uses DirectInput for gamepads
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib, "dinput8.lib")


namespace SGD
{
	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//
		// GamepadInfo
		//	- stores info for the gamepad: id, axes, buttons
		struct GamepadInfo
		{
			GUID					id;					// instance id
			IDirectInputDevice8W*	pDevice;			// DirectInput device
			wchar_t*				wszName;			// device name
			bool					bHasTriggerAxis;	// uses a fifth axis for the shoulder triggers
			bool					bConnected;			// plugged in & working

			float					fLeftX;
			float					fLeftY;
			float					fRightX;
			float					fRightY;
			float					fTrigger;			// some controllers treat the trigger as a button, not an axis
			DPad					eDPad;				// enumerated bitflags
			DPad					ePreviousDPad;
			unsigned char			aButtons[ 32 ];		// buffered key state (w/ curr and prev bits)
		};
		//*************************************************************//

		
		//*************************************************************//
		// InputManager
		//	- concrete class for detecting keyboard, mouse, and gamepad input
		class InputManager : public SGD::InputManager
		{
		public:
			// SINGLETON
			static	InputManager*	GetInstance		( void );
			static	void			DeleteInstance	( void );


			virtual	bool		Initialize			( void )			override;
			virtual	bool		Update				( void )			override;
			virtual	bool		Terminate			( void )			override;


			virtual bool		IsKeyPressed		( Key key )			const	override;
			virtual bool		IsKeyDown			( Key key )			const	override;
			virtual bool		IsKeyUp				( Key key )			const	override;
			virtual bool		IsKeyReleased		( Key key )			const	override;
					
			virtual bool		IsAnyKeyPressed		( void )			const	override;
			virtual Key			GetAnyKeyPressed	( void )			const	override;
			virtual wchar_t		GetAnyCharPressed	( void )			const	override;
			virtual bool		IsAnyKeyDown		( void )			const	override;
			virtual Key			GetAnyKeyDown		( void )			const	override;
			virtual wchar_t		GetAnyCharDown		( void )			const	override;


			virtual const wchar_t*	GetKeyName		( Key key )			const	override;


			virtual Point		GetMousePosition	( void )			const	override;
			virtual bool		SetMousePosition	( Point position )			override;
			virtual Vector		GetMouseMovement	( void )			const	override;


			virtual bool		CheckForNewControllers	( void )													override;
			virtual bool		IsControllerConnected	( unsigned int controller )							const	override;
		
			virtual const wchar_t*	GetControllerName	( unsigned int controller )							const	override;

			virtual Vector		GetLeftJoystick			( unsigned int controller )							const	override;
			virtual Vector		GetRightJoystick		( unsigned int controller )							const	override;
			virtual float		GetTrigger				( unsigned int controller )							const	override;
 
			virtual DPad		GetDPad					( unsigned int controller )							const	override;
		
			virtual bool		IsDPadPressed			( unsigned int controller, DPad direction )			const	override;
			virtual bool		IsDPadDown				( unsigned int controller, DPad direction )			const	override;
			virtual bool		IsDPadUp				( unsigned int controller, DPad direction )			const	override;
			virtual bool		IsDPadReleased			( unsigned int controller, DPad direction )			const	override;
		
			virtual bool		IsButtonPressed			( unsigned int controller, unsigned int button )	const	override;
			virtual bool		IsButtonDown			( unsigned int controller, unsigned int button )	const	override;
			virtual bool		IsButtonUp				( unsigned int controller, unsigned int button )	const	override;
			virtual bool		IsButtonReleased		( unsigned int controller, unsigned int button )	const	override;


		private:
			// SINGLETON
			static	InputManager*		s_Instance;		// the ONE instance

			InputManager				( void );									// Default constructor
			virtual	~InputManager		( void )					= default;		// Destructor

			InputManager				( const InputManager& )		= delete;		// Copy constructor
			InputManager&	operator=	( const InputManager& )		= delete;		// Assignment operator


			// Wrapper Status
			enum EInputManagerStatus
			{	
				E_UNINITIALIZED,
				E_INITIALIZED,
				E_DESTROYED
			};

			EInputManagerStatus			m_eStatus			= E_UNINITIALIZED;			// wrapper initialization status

			HWND						m_hWnd				= NULL;						// window handle
			Point						m_ptMouse			= Point{0, 0};				// mouse position (relative to window)
			Vector						m_vMouseMovement	= Vector{0, 0};				// mouse movement between frames
			IDirectInput8W*				m_pDirectInput		= nullptr;					// DirectInput api
			std::vector< GamepadInfo >	m_vGamepads;									// gamepad info / states

			// Key States
			enum 
			{ 
				Bit_Current		= 0x80, 
				Bit_Previous	= 0x40,
				Bit_Toggled		= 0x01,
				Mask_State		= 0xC0,
				State_Pressed	= Bit_Current,
				State_Released	= Bit_Previous,
			};

			unsigned char				m_aKeyboard[ 256 ];			// mouse / keyboard button state


			// KEY NAME HELPER METHOD
			static inline const wchar_t* GetAllKeyNames( void );	// all keyboard/mouse key names in a 1D array (wchar_t[256][32])

			// JOYSTICK ENUMERATION HELPER METHOD
			static BOOL CALLBACK EnumerateJoystick( const DIDEVICEINSTANCEW* instance, void* extra );
		};
		//*************************************************************//

	}	// namespace SGD_IMPLEMENTATION



	//*****************************************************************//
	// Interface singleton accessor
	/*static*/ InputManager* InputManager::GetInstance( void )
	{
		// Return the singleton
		return (SGD::InputManager*)SGD_IMPLEMENTATION::InputManager::GetInstance();
	}

	// Interface singleton destructor
	/*static*/ void InputManager::DeleteInstance( void )
	{
		// Deallocate singleton
		return SGD_IMPLEMENTATION::InputManager::DeleteInstance();
	}
	//*****************************************************************//



	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//	
		// SINGLETON

		// Instantiate static pointer to null (no instance yet)
		/*static*/ InputManager* InputManager::s_Instance = nullptr;

		// Singleton accessor
		/*static*/ InputManager* InputManager::GetInstance( void )
		{
			// Allocate singleton on first use
			if( InputManager::s_Instance == nullptr )
				InputManager::s_Instance = new InputManager;

			// Return the singleton
			return InputManager::s_Instance;
		}

		// Singleton destructor
		/*static*/ void InputManager::DeleteInstance( void )
		{
			// Deallocate singleton
			delete InputManager::s_Instance;
			InputManager::s_Instance = nullptr;
		}
		//*************************************************************//



		//*************************************************************//
		// CONSTRUCTOR
		InputManager::InputManager( void )
		{
			// Set array data members to clean values
			memset( m_aKeyboard, 0, sizeof(m_aKeyboard) );
		}	
		//*************************************************************//



		//*************************************************************//
		// INITIALIZE
		bool InputManager::Initialize( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_UNINITIALIZED && "InputManager::Initialize - wrapper has already been initialized" );
			if( m_eStatus != E_UNINITIALIZED )
				return false;

			// Validate the thread's window handle
			HWND hWnd = GetActiveWindow();
			assert( hWnd != nullptr && "InputManager::Initialize - invalid window handle" );
			if( hWnd == nullptr )
			{
				// MESSAGE
				char szBuffer[ 64 ];
				_snprintf_s( szBuffer, 64, _TRUNCATE, "!!! InputManager::Initialize - invalid window handle !!!\n" );
				OutputDebugStringA( szBuffer );

				return false;
			}


			// When using a COM object, the thread should be CoInitialized
			CoInitializeEx( nullptr, COINIT_MULTITHREADED );


			// Attempt to create the DirectInput8 interface
			HRESULT hResult = S_OK;
			hResult = DirectInput8Create( (HINSTANCE)GetModuleHandle( nullptr ), DIRECTINPUT_VERSION, IID_IDirectInput8W, (void**)&m_pDirectInput, NULL);
			if( FAILED( hResult ) )
			{
				CoUninitialize();

				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! InputManager::Initialize - failed to initialize DirectInput (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}


			// Store window
			m_hWnd = hWnd;

			
			// Store mouse position
			POINT ptMouse;
			GetCursorPos( &ptMouse );
			ScreenToClient( hWnd, &ptMouse );
			m_ptMouse = Point{ (float)ptMouse.x, (float)ptMouse.y };

			// Clip the position
			RECT rClip = { };
			GetClipCursor( &rClip );
			m_ptMouse.x -= (float)rClip.left;
			m_ptMouse.y -= (float)rClip.top;


			// Success!
			m_eStatus = E_INITIALIZED;
			
			// Check for joysticks
			CheckForNewControllers();

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// UPDATE
		bool InputManager::Update( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::Update - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;


			// Store mouse position
			POINT mouse = { };
			RECT clip = { };
			GetCursorPos( &mouse );
			ScreenToClient( m_hWnd, &mouse );
			GetClipCursor( &clip );
			mouse.x -= clip.left;
			mouse.y -= clip.top;

			m_vMouseMovement = Vector{ (float)mouse.x - m_ptMouse.x, (float)mouse.y - m_ptMouse.y };
			m_ptMouse = Point{ (float)mouse.x, (float)mouse.y };


			// Poll mouse / keyboard button state ONLY if window has focus
			BYTE keyboard[256] = { };
			GetKeyState( 0 );

			if( GetForegroundWindow() == m_hWnd )
				GetKeyboardState( keyboard );

			// Ignore: 0, 7, 10-11, 14-15, 21-26, 28-31, 58-64, 94, 136-143, 146-159, 184-185, 193-218, 224-225, 227-245, 252, 255
#define UPDATE_KEY( key )			m_aKeyboard[ key ] = ((m_aKeyboard[ key ] & Bit_Current) >> 1) | (keyboard[ key ])
#define UPDATE_KEYS( start, stop )	for( int i = start; i <= stop; i++ )	UPDATE_KEY(i);

			UPDATE_KEYS( 1, 6 );
			UPDATE_KEYS( 8, 9 );
			UPDATE_KEYS( 12, 13 );
			UPDATE_KEYS( 16, 20 );
			UPDATE_KEY( 27 );
			UPDATE_KEYS( 32, 57 );
			UPDATE_KEYS( 65, 93 );
			UPDATE_KEYS( 95, 135 );
			UPDATE_KEYS( 144, 145 );
			UPDATE_KEYS( 160, 183 );
			UPDATE_KEYS( 186, 192 );
			UPDATE_KEYS( 219, 223 );
			UPDATE_KEY( 226 );
			UPDATE_KEYS( 246, 251 );
			UPDATE_KEYS( 253, 254 );

#undef UPDATE_KEYS
#undef UPDATE_KEY



			// Update gamepads
			for( unsigned int i = 0; i < m_vGamepads.size(); i++ )
			{
				GamepadInfo& info = m_vGamepads[ i ];

				// Read device state
				DIJOYSTATE state;
				HRESULT hResult = info.pDevice->GetDeviceState( sizeof( DIJOYSTATE ), &state );
				if( FAILED( hResult ) )
				{
					hResult = info.pDevice->Acquire();
					if( FAILED( hResult ) )
					{
						info = GamepadInfo{ info.id, info.pDevice, info.wszName, info.bHasTriggerAxis, false };
						continue;
					}
				}
				
				info.bConnected = true;

				
				// Interpret the dpad / point-of-view hat
				info.ePreviousDPad = info.eDPad;

				DWORD dpad0 = state.rgdwPOV[ 0 ];
				if( dpad0 == 0xFFFFFFFF || dpad0 == 0xFFFF )
					info.eDPad = DPad::Neutral;
				else if( dpad0 < 2250 )
					info.eDPad = DPad::Up;
				else if( dpad0 < 6750 )
					info.eDPad = DPad::UpRight;
				else if( dpad0 < 11250 )
					info.eDPad = DPad::Right;
				else if( dpad0 < 15750 )
					info.eDPad = DPad::DownRight;
				else if( dpad0 < 20250 )
					info.eDPad = DPad::Down;
				else if( dpad0 < 24750 )
					info.eDPad = DPad::DownLeft;
				else if( dpad0 < 29250 )
					info.eDPad = DPad::Left;
				else if( dpad0 < 33750 )
					info.eDPad = DPad::UpLeft;
				else if( dpad0 <= 36000 )
					info.eDPad = DPad::Up;
				else	// invalid?
					info.eDPad = DPad::Neutral;


				// Interpret the buttons
				for( int b = 0; b < 32; b++ )
					info.aButtons[ b ] = ((info.aButtons[ b ] & Bit_Current) >> 1) | state.rgbButtons[ b ];


				// Interpret the joysticks
				info.fLeftX = state.lX * 0.001f;
				info.fLeftY = state.lY * 0.001f;

				if( info.bHasTriggerAxis == true )
				{
					// Right stick *should* be separate
					info.fRightX = state.lRx * 0.001f;
					info.fRightY = state.lRy * 0.001f;
					info.fTrigger = -state.lZ * 0.001f;		// left is negative
				}
				else 
				{
					// Right stick uses left z ... somehow
					info.fRightX = state.lZ  * 0.001f;
					info.fRightY = state.lRz * 0.001f;
					info.fTrigger = 0.0f;
				}
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// TERMINATE
		bool InputManager::Terminate( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::Terminate - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;
			

			// Release all devices
			for( unsigned int i = 0; i < m_vGamepads.size(); i++ )
			{
				m_vGamepads[ i ].pDevice->Release();
				delete[] m_vGamepads[ i ].wszName;
			}
			m_vGamepads.clear();

			// Release DirectInput
			m_pDirectInput->Release();
			m_pDirectInput = nullptr;


			CoUninitialize();

			m_eStatus = E_DESTROYED;
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// IS KEY PRESSED
		bool InputManager::IsKeyPressed( Key key ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsKeyPressed - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			return (m_aKeyboard[ (int)key ] & Mask_State) == State_Pressed;
		}
		//*************************************************************//



		//*************************************************************//
		// IS KEY DOWN
		bool InputManager::IsKeyDown( Key key ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsKeyDown - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			return (m_aKeyboard[ (int)key ] & Bit_Current) == Bit_Current;
		}
		//*************************************************************//



		//*************************************************************//
		// IS KEY UP
		bool InputManager::IsKeyUp( Key key ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsKeyUp - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			return (m_aKeyboard[ (int)key ] & Bit_Current) != Bit_Current;
		}
		//*************************************************************//



		//*************************************************************//
		// IS KEY RELEASED
		bool InputManager::IsKeyReleased( Key key ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsKeyReleased - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			return (m_aKeyboard[ (int)key ] & Mask_State) == State_Released;
		}
		//*************************************************************//



		//*************************************************************//
		// IS ANY KEY PRESSED
		bool InputManager::IsAnyKeyPressed( void ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsAnyKeyPressed - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;


			// Check if any key is pressed
			for( int key = 255; key >= 0; key-- )
			{
				if( (m_aKeyboard[ key ] & Mask_State) == State_Pressed )
					return true;
			}


			// No keys are pressed
			return false;
		}
		//*************************************************************//



		//*************************************************************//
		// GET ANY KEY PRESSED
		Key InputManager::GetAnyKeyPressed( void ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::GetAnyKeyPressed - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return Key::None;
			

			// Check if any character-key is down first (to match GetAnyCharDown)
			// otherwise, use any key
			Key any = Key::None;


			// Check if any character-key is pressed
			for( int key = 255; key >= 0; key-- )
			{
				if( (m_aKeyboard[ key ] & Mask_State) == State_Pressed )
				{
					// Attempt to translate the key into a character
					// (some keys do not convert: Shift, Ctrl, Alt, etc.)
					unsigned int scan = MapVirtualKeyW( (unsigned int)key, MAPVK_VK_TO_VSC );

					WCHAR ch[ 32 ];
					int result = ToUnicode( (unsigned int)key, scan, m_aKeyboard, ch, 32, 0 );
					if( result == 1 )				// it is a character-key
						return (Key)key;
					else if( any == Key::None )		// it is the first 'any' key
						any = (Key)key;
				}
			}

			
			// No character-keys are down
			return any;
		}
		//*************************************************************//



		//*************************************************************//
		// GET ANY CHAR PRESSED
		wchar_t InputManager::GetAnyCharPressed( void ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::GetAnyCharPressed - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return 0;


			// Check if any character-key is pressed
			for( int key = 255; key >= 0; key-- )
			{
				if( (m_aKeyboard[ key ] & Mask_State) == State_Pressed )
				{
					// Attempt to translate the key into a character
					// (some keys do not convert: Shift, Ctrl, Alt, etc.)
					unsigned int scan = MapVirtualKeyW( (unsigned int)key, MAPVK_VK_TO_VSC );

					WCHAR ch[ 32 ];
					int result = ToUnicode( (unsigned int)key, scan, m_aKeyboard, ch, 32, 0 );
					if( result == 1 )
						return (wchar_t)(ch[0]);
				}
			}


			// No character-keys are pressed
			return 0;
		}
		//*************************************************************//



		//*************************************************************//
		// IS ANY KEY DOWN
		bool InputManager::IsAnyKeyDown( void ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsAnyKeyDown - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;


			// Check if any key is down
			for( int key = 255; key >= 0; key-- )
			{
				if( (m_aKeyboard[ key ] & Bit_Current) == Bit_Current )
					return true;
			}


			// No keys are down
			return false;
		}
		//*************************************************************//



		//*************************************************************//
		// GET ANY KEY DOWN
		Key InputManager::GetAnyKeyDown( void ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::GetAnyKeyDown - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return Key::None;


			// Check if any character-key is down first (to match GetAnyCharDown)
			// otherwise, use any key
			Key any = Key::None;


			// Check if any character-key is down
			for( int key = 255; key >= 0; key-- )
			{
				if( (m_aKeyboard[ key ] & Bit_Current) == Bit_Current )
				{
					// Attempt to translate the key into a character
					// (some keys do not convert: Shift, Ctrl, Alt, etc.)
					unsigned int scan = MapVirtualKeyW( (unsigned int)key, MAPVK_VK_TO_VSC );

					WCHAR ch[ 32 ];
					int result = ToUnicode( (unsigned int)key, scan, m_aKeyboard, ch, 32, 0 );
					if( result == 1 )				// it is a character-key
						return (Key)key;
					else if( any == Key::None )		// it is the first 'any' key
						any = (Key)key;
				}
			}


			// No character-keys are down
			return any;
		}
		//*************************************************************//



		//*************************************************************//
		// GET ANY CHAR DOWN
		wchar_t InputManager::GetAnyCharDown( void ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::GetAnyCharDown - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return 0;


			// Check if any character-key is down
			for( int key = 255; key >= 0; key-- )
			{
				if( (m_aKeyboard[ key ] & Bit_Current) == Bit_Current )
				{
					// Attempt to translate the key into a character
					// (some keys do not convert: Shift, Ctrl, Alt, etc.)
					unsigned int scan = MapVirtualKeyW( (unsigned int)key, MAPVK_VK_TO_VSC );

					WCHAR ch[ 32 ];
					int result = ToUnicode( (unsigned int)key, scan, m_aKeyboard, ch, 32, 0 );
					if( result == 1 )
						return (wchar_t)(ch[0]);
				}
			}


			// No character-keys are down
			return 0;
		}
		//*************************************************************//
		
		
		
		//*************************************************************//
		// GET KEY NAME
		const wchar_t* InputManager::GetKeyName( Key key ) const
		{
			// Sanity-check the key index
			assert( (int)key >= 0 && (int)key <= 255 && "InputManager::GetKeyName - invalid key (range is 0-255)" );
			if( (int)key < 0 || (int)key > 255 )
				return L"Invalid";


			// Access the contiguous block of names (wchar_t[256][32])
			const wchar_t* names = GetAllKeyNames();

			// Manually index into the array (skip 32 characters per key)
			return (names + ((int)key * 32));
		}
		//*************************************************************//



		//*************************************************************//
		// GET MOUSE POSITION
		Point InputManager::GetMousePosition( void ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::GetMousePosition - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return Point{ 0, 0 };

			return m_ptMouse;
		}
		//*************************************************************//



		//*************************************************************//
		// SET MOUSE POSITION
		bool InputManager::SetMousePosition( Point position )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::SetMousePosition - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;


			// Attempt to move the Windows cursor
			POINT mouse = { (LONG)position.x, (LONG)position.y };
			ClientToScreen( m_hWnd, &mouse );
			BOOL result = SetCursorPos( (int)mouse.x, (int)mouse.y );

			if( result == FALSE )
			{
				// MESSAGE
				char szBuffer[ 64 ];
				_snprintf_s( szBuffer, 64, _TRUNCATE, "!!! InputManager::SetMousePosition - failed to set Windows cursor position !!!\n" );
				OutputDebugStringA( szBuffer );

				return false;
			}

			
			// Store updated mouse position
			RECT clip = { };
			GetCursorPos( &mouse );
			ScreenToClient( m_hWnd, &mouse );
			GetClipCursor( &clip );
			mouse.x -= clip.left;
			mouse.y -= clip.top;

			m_ptMouse = Point{ (float)mouse.x, (float)mouse.y };
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// GET MOUSE MOVEMENT
		Vector InputManager::GetMouseMovement( void ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::GetMouseMovement - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return Vector{ 0, 0 };

			return m_vMouseMovement;
		}
		//*************************************************************//



		//*************************************************************//
		// CHECK FOR NEW CONTROLLERS
		bool InputManager::CheckForNewControllers( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::CheckForNewControllers - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Read new gamepads (too slow to call every Update)
			std::vector< DIDEVICEINSTANCEW > instances;
			HRESULT hResult = m_pDirectInput->EnumDevices( DI8DEVCLASS_GAMECTRL, &EnumerateJoystick, &instances, DIEDFL_ATTACHEDONLY );
			if( FAILED( hResult ) )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! InputManager::CheckForNewControllers - failed to enumerate DirectInput devices (0x%X) !!!\n", hResult );
				OutputDebugStringA( szBuffer );

				return false;
			}


			// Hold the current number of known devices
			unsigned int numKnown = m_vGamepads.size();


			// Initialize each device
			for( unsigned int i = 0; i < instances.size(); i++ )
			{
				// Has this device already been initialized?
				bool known = false;
				for( unsigned int j = 0; j < m_vGamepads.size(); j++ )
					if( instances[ i ].guidInstance == m_vGamepads[ j ].id )
					{
						known = true;
						break;
					}

				if( known == true )
					continue;


				// New device
				GamepadInfo info = { };
				info.id = instances[ i ].guidInstance;


				// Create the device
				hResult = m_pDirectInput->CreateDevice( info.id, &info.pDevice, nullptr );
				if( FAILED( hResult ) )
					continue;


				// Get the capabilities
				DIDEVCAPS capabilities;
				memset( &capabilities, 0, sizeof( capabilities ) );
				capabilities.dwSize = sizeof( capabilities );
				hResult = info.pDevice->GetCapabilities( &capabilities );
				if( FAILED( hResult ) )
					continue;

				if( capabilities.dwAxes > 4 )
					info.bHasTriggerAxis = true;


				// Set the cooperative level
				hResult = info.pDevice->SetCooperativeLevel( m_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND );
				if( FAILED( hResult ) )
					continue;
				

				// Set the data format (simple joystick)
				hResult = info.pDevice->SetDataFormat( &c_dfDIJoystick );
				if( FAILED( hResult ) )
					continue;


				// Set joystick axes properties: range / deadzone(min) / saturation(max)
				DIPROPRANGE range;
				range.lMin = -1000;
				range.lMax = +1000;
				range.diph.dwSize = sizeof( DIPROPRANGE );
				range.diph.dwHeaderSize = sizeof( DIPROPHEADER );
				range.diph.dwHow = DIPH_DEVICE;
				range.diph.dwObj = 0;

				info.pDevice->SetProperty( DIPROP_RANGE, &range.diph );

				
				DIPROPDWORD deadzone;
				deadzone.dwData = 1000;		// 10%  	- any movement less than deadzone threshold is min
				deadzone.diph.dwSize = sizeof( DIPROPDWORD );
				deadzone.diph.dwHeaderSize = sizeof( DIPROPHEADER );
				deadzone.diph.dwHow = DIPH_DEVICE;
				deadzone.diph.dwObj = 0;

				info.pDevice->SetProperty( DIPROP_DEADZONE, &deadzone.diph );

								
				DIPROPDWORD saturation;
				saturation.dwData = 9000;	// 90%  	- any movement greater than saturation threshold is max
				saturation.diph.dwSize = sizeof( DIPROPDWORD );
				saturation.diph.dwHeaderSize = sizeof( DIPROPHEADER );
				saturation.diph.dwHow = DIPH_DEVICE;
				saturation.diph.dwObj = 0;

				info.pDevice->SetProperty( DIPROP_SATURATION, &saturation.diph );
						


				// Acquire the joystick 
				hResult = info.pDevice->Acquire();

				// Ignore failure so the device registers; will be handled in Update
				//if( FAILED( hResult ) )
				//	continue;


				// Store the product name				
				info.wszName = _wcsdup( instances[ i ].tszProductName );

				// Store the gamepad
				m_vGamepads.push_back( info );
			}
					
			// Did a joystick initialization fail?
			if( m_vGamepads.size() != instances.size() )
			{
				// MESSAGE
				char szBuffer[ 128 ];
				_snprintf_s( szBuffer, 128, _TRUNCATE, "!!! InputManager::CheckForNewControllers - failed to initialize a DirectInput device !!!\n" );
				OutputDebugStringA( szBuffer );
			}


			// Was there a new joystick?
			return ( m_vGamepads.size() > numKnown );
		}
		//*************************************************************//



		//*************************************************************//
		// IS CONTROLLER CONNECTED
		bool InputManager::IsControllerConnected( unsigned int controller ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsControllerConnected - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			if( controller >= m_vGamepads.size() )
				return false;

			return m_vGamepads[ controller ].bConnected;
		}
		//*************************************************************//
		
		

		//*************************************************************//
		// GET CONTROLLER NAME
		const wchar_t* InputManager::GetControllerName( unsigned int controller ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::GetControllerName - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return L"";

			if( controller >= m_vGamepads.size() )
				return L"";

			return m_vGamepads[ controller ].wszName;
		}
		//*************************************************************//



		//*************************************************************//
		// GET LEFT JOYSTICK
		Vector InputManager::GetLeftJoystick( unsigned int controller ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::GetLeftJoystick - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return Vector{ 0, 0 };

			// Verify the controller index
			if( controller >= m_vGamepads.size() )
				return Vector{ 0, 0 };

			return Vector{ m_vGamepads[ controller ].fLeftX, m_vGamepads[ controller ].fLeftY };
		}
		//*************************************************************//



		//*************************************************************//
		// GET RIGHT JOYSTICK
		Vector InputManager::GetRightJoystick( unsigned int controller ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::GetRightJoystick - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return Vector{ 0, 0 };

			// Verify the controller index
			if( controller >= m_vGamepads.size() )
				return Vector{ 0, 0 };
			

			return Vector{ m_vGamepads[ controller ].fRightX, m_vGamepads[ controller ].fRightY };
		}
		//*************************************************************//



		//*************************************************************//
		// GET TRIGGER
		float InputManager::GetTrigger( unsigned int controller ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::GetTrigger - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return 0.0f;

			// Verify the controller index
			if( controller >= m_vGamepads.size() )
				return 0.0f;

			return m_vGamepads[ controller ].fTrigger;
		}
		//*************************************************************//



		//*************************************************************//
		// GET DPAD
		DPad InputManager::GetDPad( unsigned int controller ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::GetDPad - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return DPad::Neutral;

			// Verify the controller index
			if( controller >= m_vGamepads.size() )
				return DPad::Neutral;

			return m_vGamepads[ controller ].eDPad;
		}
		//*************************************************************//



		//*************************************************************//	
		// IS DPAD PRESSED	
		bool InputManager::IsDPadPressed( unsigned int controller, DPad direction ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsDPadPressed - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Verify the controller index
			if( controller >= m_vGamepads.size() )
				return false;

			
			return (((int)m_vGamepads[ controller ].eDPad & (int)direction) == (int)direction)
				&& (((int)m_vGamepads[ controller ].ePreviousDPad & (int)direction) != (int)direction);
		}
		//*************************************************************//



		//*************************************************************//
		// IS DPAD DOWN	
		bool InputManager::IsDPadDown( unsigned int controller, DPad direction ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsDPadDown - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Verify the controller index
			if( controller >= m_vGamepads.size() )
				return false;

			
			return ((int)m_vGamepads[ controller ].eDPad & (int)direction) == (int)direction;
		}
		//*************************************************************//



		//*************************************************************//
		// IS DPAD UP	
		bool InputManager::IsDPadUp( unsigned int controller, DPad direction ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsDPadUp - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Verify the controller index
			if( controller >= m_vGamepads.size() )
				return false;
			

			return ((int)m_vGamepads[ controller ].eDPad & (int)direction) != (int)direction;
		}
		//*************************************************************//



		//*************************************************************//
		// IS DPAD RELEASED	
		bool InputManager::IsDPadReleased( unsigned int controller, DPad direction ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsDPadReleased - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Verify the controller index
			if( controller >= m_vGamepads.size() )
				return false;
			

			return (((int)m_vGamepads[ controller ].eDPad & (int)direction) != (int)direction)
				&& (((int)m_vGamepads[ controller ].ePreviousDPad & (int)direction) == (int)direction);
		}
		//*************************************************************//



		//*************************************************************//
		// IS BUTTON PRESSED	
		bool InputManager::IsButtonPressed( unsigned int controller, unsigned int button ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsButtonPressed - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Sanity-check the button
			assert( button < 32 && "InputManager::IsButtonPressed - button index must be between 0 and 31" );
			if( button >= 32 )
				return false;

			// Verify the controller index
			if( controller >= m_vGamepads.size() )
				return false;


			return (m_vGamepads[ controller ].aButtons[ button ] & Mask_State) == State_Pressed;
		}
		//*************************************************************//



		//*************************************************************//
		// IS BUTTON DOWN
		bool InputManager::IsButtonDown( unsigned int controller, unsigned int button ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsButtonDown - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Sanity-check the button
			assert( button < 32 && "InputManager::IsButtonDown - button index must be between 0 and 31" );
			if( button >= 32 )
				return false;

			// Verify the controller index
			if( controller >= m_vGamepads.size() )
				return false;


			return (m_vGamepads[ controller ].aButtons[ button ] & Bit_Current) == Bit_Current;
		}
		//*************************************************************//



		//*************************************************************//
		// IS BUTTON UP
		bool InputManager::IsButtonUp( unsigned int controller, unsigned int button ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsButtonUp - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Sanity-check the button
			assert( button < 32 && "InputManager::IsButtonDown - button index must be between 0 and 31" );
			if( button >= 32 )
				return false;

			// Verify the controller index
			if( controller >= m_vGamepads.size() )
				return false;


			return (m_vGamepads[ controller ].aButtons[ button ] & Bit_Current) != Bit_Current;
		}
		//*************************************************************//



		//*************************************************************//
		// IS BUTTON RELEASED
		bool InputManager::IsButtonReleased( unsigned int controller, unsigned int button ) const
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "InputManager::IsButtonReleased - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Sanity-check the button
			assert( button < 32 && "InputManager::IsButtonDown - button index must be between 0 and 31" );
			if( button >= 32 )
				return false;

			// Verify the controller index
			if( controller >= m_vGamepads.size() )
				return false;


			return (m_vGamepads[ controller ].aButtons[ button ] & Mask_State) == State_Released;
		}
		//*************************************************************//



		//*************************************************************//
		// GET ALL KEY NAMES
		/*static*/ const wchar_t* InputManager::GetAllKeyNames( void )
		{
			// Return all the mouse / keyboard key names in a
			// contiguous block of wchar_t[256][32]
			// Literal constant string has static storage
			return
				L"Unknown 0x00\0 000000000000000000"
				L"Left Mouse Button\0 0000000000000"
				L"Right Mouse Button\0 000000000000"
				L"Unknown 0x03\0 000000000000000000"
				L"Middle Mouse Button\0 00000000000"
				L"X1 Mouse Button\0 000000000000000"
				L"X2 Mouse Button\0 000000000000000"
				L"Unknown 0x07\0 000000000000000000"
				L"Backspace\0 000000000000000000000"
				L"Tab\0 000000000000000000000000000"
				L"Unknown 0x0A\0 000000000000000000"
				L"Unknown 0x0B\0 000000000000000000"
				L"Clear\0 0000000000000000000000000"
				L"Enter\0 0000000000000000000000000"
				L"Unknown 0x0E\0 000000000000000000"
				L"Unknown 0x0F\0 000000000000000000"
				L"Shift\0 0000000000000000000000000"
				L"Control\0 00000000000000000000000"
				L"Alt\0 000000000000000000000000000"
				L"Pause\0 0000000000000000000000000"
				L"Caps Lock\0 000000000000000000000"
				L"Unknown 0x15\0 000000000000000000"
				L"Unknown 0x16\0 000000000000000000"
				L"Unknown 0x17\0 000000000000000000"
				L"Unknown 0x18\0 000000000000000000"
				L"Unknown 0x19\0 000000000000000000"
				L"Unknown 0x1A\0 000000000000000000"
				L"Escape\0 000000000000000000000000"
				L"Unknown 0x1C\0 000000000000000000"
				L"Unknown 0x1D\0 000000000000000000"
				L"Unknown 0x1E\0 000000000000000000"
				L"Unknown 0x1F\0 000000000000000000"
				L"Space\0 0000000000000000000000000"
				L"Page Up\0 00000000000000000000000"
				L"Page Down\0 000000000000000000000"
				L"End\0 000000000000000000000000000"
				L"Home\0 00000000000000000000000000"
				L"Left Arrow\0 00000000000000000000"
				L"Up Arrow\0 0000000000000000000000"
				L"Right Arrow\0 0000000000000000000"
				L"Down Arrow\0 00000000000000000000"
				L"Select\0 000000000000000000000000"
				L"Print\0 0000000000000000000000000"
				L"Execute\0 00000000000000000000000"
				L"Print Screen\0 000000000000000000"
				L"Insert\0 000000000000000000000000"
				L"Delete\0 000000000000000000000000"
				L"Help\0 00000000000000000000000000"
				L"0\0 00000000000000000000000000000"
				L"1\0 00000000000000000000000000000"
				L"2\0 00000000000000000000000000000"
				L"3\0 00000000000000000000000000000"
				L"4\0 00000000000000000000000000000"
				L"5\0 00000000000000000000000000000"
				L"6\0 00000000000000000000000000000"
				L"7\0 00000000000000000000000000000"
				L"8\0 00000000000000000000000000000"
				L"9\0 00000000000000000000000000000"
				L"Unknown 0x3A\0 000000000000000000"
				L"Unknown 0x3B\0 000000000000000000"
				L"Unknown 0x3C\0 000000000000000000"
				L"Unknown 0x3D\0 000000000000000000"
				L"Unknown 0x3E\0 000000000000000000"
				L"Unknown 0x3F\0 000000000000000000"
				L"Unknown 0x3A\0 000000000000000000"
				L"A\0 00000000000000000000000000000"
				L"B\0 00000000000000000000000000000"
				L"C\0 00000000000000000000000000000"
				L"D\0 00000000000000000000000000000"
				L"E\0 00000000000000000000000000000"
				L"F\0 00000000000000000000000000000"
				L"G\0 00000000000000000000000000000"
				L"H\0 00000000000000000000000000000"
				L"I\0 00000000000000000000000000000"
				L"J\0 00000000000000000000000000000"
				L"K\0 00000000000000000000000000000"
				L"L\0 00000000000000000000000000000"
				L"M\0 00000000000000000000000000000"
				L"N\0 00000000000000000000000000000"
				L"O\0 00000000000000000000000000000"
				L"P\0 00000000000000000000000000000"
				L"Q\0 00000000000000000000000000000"
				L"R\0 00000000000000000000000000000"
				L"S\0 00000000000000000000000000000"
				L"T\0 00000000000000000000000000000"
				L"U\0 00000000000000000000000000000"
				L"V\0 00000000000000000000000000000"
				L"W\0 00000000000000000000000000000"
				L"X\0 00000000000000000000000000000"
				L"Y\0 00000000000000000000000000000"
				L"Z\0 00000000000000000000000000000"
				L"Left Windows\0 000000000000000000"
				L"Right Windows\0 00000000000000000"
				L"Applications\0 000000000000000000"
				L"Unknown 0x5E\0 000000000000000000"
				L"Sleep\0 0000000000000000000000000"
				L"NumPad 0\0 0000000000000000000000"
				L"NumPad 1\0 0000000000000000000000"
				L"NumPad 2\0 0000000000000000000000"
				L"NumPad 3\0 0000000000000000000000"
				L"NumPad 4\0 0000000000000000000000"
				L"NumPad 5\0 0000000000000000000000"
				L"NumPad 6\0 0000000000000000000000"
				L"NumPad 7\0 0000000000000000000000"
				L"NumPad 8\0 0000000000000000000000"
				L"NumPad 9\0 0000000000000000000000"
				L"Multiply\0 0000000000000000000000"
				L"Add\0 000000000000000000000000000"
				L"Separator\0 000000000000000000000"
				L"Subtract\0 0000000000000000000000"
				L"Decimal\0 00000000000000000000000"
				L"Divide\0 000000000000000000000000"
				L"F1\0 0000000000000000000000000000"
				L"F2\0 0000000000000000000000000000"
				L"F3\0 0000000000000000000000000000"
				L"F4\0 0000000000000000000000000000"
				L"F5\0 0000000000000000000000000000"
				L"F6\0 0000000000000000000000000000"
				L"F7\0 0000000000000000000000000000"
				L"F8\0 0000000000000000000000000000"
				L"F9\0 0000000000000000000000000000"
				L"F10\0 000000000000000000000000000"
				L"F11\0 000000000000000000000000000"
				L"F12\0 000000000000000000000000000"
				L"F13\0 000000000000000000000000000"
				L"F14\0 000000000000000000000000000"
				L"F15\0 000000000000000000000000000"
				L"F16\0 000000000000000000000000000"
				L"F17\0 000000000000000000000000000"
				L"F18\0 000000000000000000000000000"
				L"F19\0 000000000000000000000000000"
				L"F20\0 000000000000000000000000000"
				L"F21\0 000000000000000000000000000"
				L"F22\0 000000000000000000000000000"
				L"F23\0 000000000000000000000000000"
				L"F24\0 000000000000000000000000000"
				L"Unknown 0x88\0 000000000000000000"
				L"Unknown 0x89\0 000000000000000000"
				L"Unknown 0x8A\0 000000000000000000"
				L"Unknown 0x8B\0 000000000000000000"
				L"Unknown 0x8C\0 000000000000000000"
				L"Unknown 0x8D\0 000000000000000000"
				L"Unknown 0x8E\0 000000000000000000"
				L"Unknown 0x8F\0 000000000000000000"
				L"Num Lock\0 0000000000000000000000"
				L"Scroll Lock\0 0000000000000000000"
				L"Unknown 0x92\0 000000000000000000"
				L"Unknown 0x93\0 000000000000000000"
				L"Unknown 0x94\0 000000000000000000"
				L"Unknown 0x95\0 000000000000000000"
				L"Unknown 0x96\0 000000000000000000"
				L"Unknown 0x97\0 000000000000000000"
				L"Unknown 0x98\0 000000000000000000"
				L"Unknown 0x99\0 000000000000000000"
				L"Unknown 0x9A\0 000000000000000000"
				L"Unknown 0x9B\0 000000000000000000"
				L"Unknown 0x9C\0 000000000000000000"
				L"Unknown 0x9D\0 000000000000000000"
				L"Unknown 0x9E\0 000000000000000000"
				L"Unknown 0x9F\0 000000000000000000"
				L"Left Shift\0 00000000000000000000"
				L"Right Shift\0 0000000000000000000"
				L"Left Control\0 000000000000000000"
				L"Right Control\0 00000000000000000"
				L"Left Alt\0 0000000000000000000000"
				L"Right Alt\0 000000000000000000000"
				L"Browser Back\0 000000000000000000"
				L"Browser Forward\0 000000000000000"
				L"Browser Refresh\0 000000000000000"
				L"Browser Stop\0 000000000000000000"
				L"Browser Search\0 0000000000000000"
				L"Browser Favorites\0 0000000000000"
				L"Browser Home\0 000000000000000000"
				L"Volume Mute\0 0000000000000000000"
				L"Volume Down\0 0000000000000000000"
				L"Volume Up\0 000000000000000000000"
				L"Media Next Track\0 00000000000000"
				L"Media Previous Track\0 0000000000"
				L"Media Stop\0 00000000000000000000"
				L"Media Play / Pause\0 000000000000"
				L"Launch Mail\0 0000000000000000000"
				L"Select Media\0 000000000000000000"
				L"Launch Application 1\0 0000000000"
				L"Launch Application 2\0 0000000000"
				L"Unknown 0xB8\0 000000000000000000"
				L"Unknown 0xB9\0 000000000000000000"
				L"Semicolon\0 000000000000000000000"
				L"Plus\0 00000000000000000000000000"
				L"Comma\0 0000000000000000000000000"
				L"Minus\0 0000000000000000000000000"
				L"Period\0 000000000000000000000000"
				L"Question\0 0000000000000000000000"
				L"Tilde\0 0000000000000000000000000"
				L"Unknown 0xC1\0 000000000000000000"
				L"Unknown 0xC2\0 000000000000000000"
				L"Unknown 0xC3\0 000000000000000000"
				L"Unknown 0xC4\0 000000000000000000"
				L"Unknown 0xC5\0 000000000000000000"
				L"Unknown 0xC6\0 000000000000000000"
				L"Unknown 0xC7\0 000000000000000000"
				L"Unknown 0xC8\0 000000000000000000"
				L"Unknown 0xC9\0 000000000000000000"
				L"Unknown 0xCA\0 000000000000000000"
				L"Unknown 0xCB\0 000000000000000000"
				L"Unknown 0xCC\0 000000000000000000"
				L"Unknown 0xCD\0 000000000000000000"
				L"Unknown 0xCE\0 000000000000000000"
				L"Unknown 0xCF\0 000000000000000000"
				L"Unknown 0xD0\0 000000000000000000"
				L"Unknown 0xD1\0 000000000000000000"
				L"Unknown 0xD2\0 000000000000000000"
				L"Unknown 0xD3\0 000000000000000000"
				L"Unknown 0xD4\0 000000000000000000"
				L"Unknown 0xD5\0 000000000000000000"
				L"Unknown 0xD6\0 000000000000000000"
				L"Unknown 0xD7\0 000000000000000000"
				L"Unknown 0xD8\0 000000000000000000"
				L"Unknown 0xD9\0 000000000000000000"
				L"Unknown 0xDA\0 000000000000000000"
				L"Open Bracket\0 000000000000000000"
				L"Pipe\0 00000000000000000000000000"
				L"Close Bracket\0 00000000000000000"
				L"Quote\0 0000000000000000000000000"
				L"OEM 8\0 0000000000000000000000000"
				L"Unknown 0xE0\0 000000000000000000"
				L"Unknown 0xE1\0 000000000000000000"
				L"Backslash\0 000000000000000000000"
				L"Unknown 0xE3\0 000000000000000000"
				L"Unknown 0xE4\0 000000000000000000"
				L"Unknown 0xE5\0 000000000000000000"
				L"Unknown 0xE6\0 000000000000000000"
				L"Unknown 0xE7\0 000000000000000000"
				L"Unknown 0xE8\0 000000000000000000"
				L"Unknown 0xE9\0 000000000000000000"
				L"Unknown 0xEA\0 000000000000000000"
				L"Unknown 0xEB\0 000000000000000000"
				L"Unknown 0xEC\0 000000000000000000"
				L"Unknown 0xED\0 000000000000000000"
				L"Unknown 0xEE\0 000000000000000000"
				L"Unknown 0xEF\0 000000000000000000"
				L"Unknown 0xF0\0 000000000000000000"
				L"Unknown 0xF1\0 000000000000000000"
				L"Unknown 0xF2\0 000000000000000000"
				L"Unknown 0xF3\0 000000000000000000"
				L"Unknown 0xF4\0 000000000000000000"
				L"Unknown 0xF5\0 000000000000000000"
				L"Attn\0 00000000000000000000000000"
				L"CrSel\0 0000000000000000000000000"
				L"ExSel\0 0000000000000000000000000"
				L"Erase EOF\0 000000000000000000000"
				L"Play\0 00000000000000000000000000"
				L"Zoom\0 00000000000000000000000000"
				L"Unknown 0xFC\0 000000000000000000"
				L"PA1\0 000000000000000000000000000"
				L"OEM Clear\0 000000000000000000000"
				L"Unknown 0xFC\0 000000000000000000"
				;
		}
		//*************************************************************//

		
		
		//*************************************************************//
		// ENUMERATE JOYSTICK
		/*static*/ BOOL CALLBACK InputManager::EnumerateJoystick( const DIDEVICEINSTANCEW* instance, void* extra )
		{
			// Convert the void* back to the vector
			std::vector< DIDEVICEINSTANCEW >* instances = reinterpret_cast< std::vector< DIDEVICEINSTANCEW >* >( extra );

			// Store the joystick id
			instances->push_back( *instance );

			// Continue to the next connected device
			return DIENUM_CONTINUE;
		}
		//*************************************************************//


	}	// namespace SGD_IMPLEMENTATION

}	// namespace SGD

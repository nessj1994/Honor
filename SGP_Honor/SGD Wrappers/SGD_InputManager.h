/***********************************************************************\
|																		|
|	File:			SGD_InputManager.h  								|
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

#ifndef SGD_INPUTMANAGER_H
#define SGD_INPUTMANAGER_H


#include "SGD_Key.h"			// Accesses keys using Key enumerators
#include "SGD_Geometry.h"		// Uses floating-point Point and Vector


namespace SGD
{
	//*****************************************************************//
	// InputManager
	//	- SINGLETON class for detecting keyboard, mouse, and gamepad input
	//	- Key and DPad enumerators REQUIRE the enum typename scope: Key::None
	class InputManager
	{
	public:
		static	InputManager*	GetInstance		( void );
		static	void			DeleteInstance	( void );


		virtual	bool		Initialize			( void )			= 0;
		virtual	bool		Update				( void )			= 0;
		virtual	bool		Terminate			( void )			= 0;


		virtual bool		IsKeyPressed		( Key key )			const	= 0;
		virtual bool		IsKeyDown			( Key key )			const	= 0;
		virtual bool		IsKeyUp				( Key key )			const	= 0;
		virtual bool		IsKeyReleased		( Key key )			const	= 0;
		
		virtual bool		IsAnyKeyPressed		( void )			const	= 0;
		virtual Key			GetAnyKeyPressed	( void )			const	= 0;
		virtual wchar_t		GetAnyCharPressed	( void )			const	= 0;
		virtual bool		IsAnyKeyDown		( void )			const	= 0;
		virtual Key			GetAnyKeyDown		( void )			const	= 0;
		virtual wchar_t		GetAnyCharDown		( void )			const	= 0;


		virtual const wchar_t*	GetKeyName		( Key key )			const	= 0;

		
		virtual Point		GetMousePosition	( void )			const	= 0;
		virtual bool		SetMousePosition	( Point position )			= 0;
		virtual Vector		GetMouseMovement	( void )			const	= 0;
		
		
		virtual bool		CheckForNewControllers	( void )													= 0;
		virtual bool		IsControllerConnected	( unsigned int controller )							const	= 0;
		
		virtual const wchar_t*	GetControllerName	( unsigned int controller )							const	= 0;

		virtual Vector		GetLeftJoystick			( unsigned int controller )							const	= 0;
		virtual Vector		GetRightJoystick		( unsigned int controller )							const	= 0;
		virtual float		GetTrigger				( unsigned int controller )							const	= 0;
 
		virtual DPad		GetDPad					( unsigned int controller )							const	= 0;
		
		virtual bool		IsDPadPressed			( unsigned int controller, DPad direction )			const	= 0;
		virtual bool		IsDPadDown				( unsigned int controller, DPad direction )			const	= 0;
		virtual bool		IsDPadUp				( unsigned int controller, DPad direction )			const	= 0;
		virtual bool		IsDPadReleased			( unsigned int controller, DPad direction )			const	= 0;
		
		virtual bool		IsButtonPressed			( unsigned int controller, unsigned int button )	const	= 0;
		virtual bool		IsButtonDown			( unsigned int controller, unsigned int button )	const	= 0;
		virtual bool		IsButtonUp				( unsigned int controller, unsigned int button )	const	= 0;
		virtual bool		IsButtonReleased		( unsigned int controller, unsigned int button )	const	= 0;


	protected:
		InputManager				( void )					= default;
		virtual	~InputManager		( void )					= default;

		InputManager				( const InputManager& )		= delete;
		InputManager&	operator=	( const InputManager& )		= delete;
	};

}	// namespace SGD

#endif	//SGD_INPUTMANAGER_H

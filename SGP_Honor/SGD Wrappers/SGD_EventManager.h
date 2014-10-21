/***********************************************************************\
|																		|
|	File:			SGD_EventManager.h   								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To queue events for later processing by				|
|					the registered listeners' callback functions		|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_EVENTMANAGER_H
#define SGD_EVENTMANAGER_H


namespace SGD
{
	// Forward declarations
	class Event;
	class Listener;
		

	//*****************************************************************//
	// EventManager
	//	- SINGLETON class for queuing events sent to registered Listeners
	class EventManager
	{
	public:
		static	EventManager*	GetInstance		( void );
		static	void			DeleteInstance	( void );
		

		virtual	bool		Initialize			( void )					= 0;
		virtual	bool		Update				( void )					= 0;
		virtual	bool		Terminate			( void )					= 0;

		virtual bool		RegisterForEvent	( Listener* listener, const char* eventID )					= 0;
		virtual bool		UnregisterFromEvent	( Listener* listener, const char* eventID = nullptr )		= 0;

		virtual bool		QueueEvent			( const Event* pEvent, const void* destination = nullptr )	= 0;
		virtual bool		SendEventNow		( const Event* pEvent, const void* destination = nullptr )	= 0;

		virtual bool		ClearEvents			( void )					= 0;
				
		
	protected:
		EventManager			( void )				= default;
		virtual	~EventManager	( void )				= default;

		EventManager			( const EventManager& )	= delete;
		EventManager& operator=	( const EventManager& )	= delete;
	};

}	// namespace SGD

#endif	//SGD_EVENTMANAGER_H

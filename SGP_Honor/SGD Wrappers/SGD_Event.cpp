/***********************************************************************\
|																		|
|	File:			SGD_Event.cpp  										|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To store data, which is sent through the			|
|					EventManager to all registered IListeners			|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#include "SGD_Event.h"


// Uses assert for debug breaks
#include <cassert>

// Uses strcmp for comparisons
#include <cstring>

// Uses Event Manager
#include "SGD_EventManager.h"


namespace SGD
{

#pragma region EVENTID_METHODS
	
	//*****************************************************************//
	// EVENT ID METHODS

	// Default constructor
	EventID::EventID( const char* str )
	{
		// Copy the parameter
		memcpy( id, str, SIZE );

		// Pad the remaining string with null terminators (for quick mem compare)
		int eos = 0;
		while( eos < SIZE && id[ eos ] != '\0' )
			eos++;
		while( eos < SIZE )
			id[ eos++ ] = '\0';
	}

	// Is-equal-to
	bool EventID::operator == ( const EventID& other ) const
	{
		return memcmp( id, other.id, SIZE ) == 0;
	}
	
	// Not-equal-to
	bool EventID::operator != ( const EventID& other ) const
	{
		return memcmp( id, other.id, SIZE ) != 0;
	}

	// Less-than
	bool EventID::operator < ( const EventID& other ) const
	{
		return strcmp( id, other.id ) < 0;
	}

	// Is-equal-to string
	bool EventID::operator == ( const char* str ) const
	{
		return strcmp( id, str ) == 0;
	}

	// Not-equal-to string
	bool EventID::operator != ( const char* str ) const
	{
		return strcmp( id, str ) != 0;
	}

	// String is-equal-to
	/*friend*/ bool operator == ( const char* str, const EventID& other )
	{
		return strcmp( str, other.id ) == 0;
	}

	// String not-equal-to
	/*friend*/ bool operator != ( const char* str, const EventID& other )
	{
		return strcmp( str, other.id ) != 0;
	}
	//*****************************************************************//

#pragma endregion EVENTID_METHODS


#pragma region EVENT_METHODS
	
	//*****************************************************************//
	// EVENT METHODS

	// Overloaded constructor
	Event::Event
	(	
		const char*		eventID,		// event ID / name
		void*			data,			// attached data (any type)
		void*			sender			// object that sent the event
	)
	: m_pData{ data }, m_pSender{ sender }
	{
		// Validate the ID
		assert( eventID != nullptr && "Event::Event - event ID cannot be null!" );
		if( eventID == nullptr )
			return;

		assert( strlen( eventID ) < 32 && "Event::Event - event ID length cannot exceed 31 characters!" );
		if( strlen( eventID ) >= 32 )
			return;


		// Store the ID		
		m_ID = eventID;
	}

	// EventManager Interaction:
	bool Event::QueueEvent( const void* destination ) const
	{
		return EventManager::GetInstance()->QueueEvent( this, destination );
	}

	bool Event::SendEventNow( const void* destination ) const
	{
		return EventManager::GetInstance()->SendEventNow( this, destination );
	}
	//*****************************************************************//
	
#pragma endregion EVENT_METHODS

}	// namespace SGD

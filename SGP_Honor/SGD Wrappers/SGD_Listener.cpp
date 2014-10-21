/***********************************************************************\
|																		|
|	File:			SGD_Listener.cpp  									|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To declare the interface used by the EventManager	|
|					to send events to registered listeners				|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#include "SGD_Listener.h"


// Uses assert for debug breaks
#include <cassert>

// Uses Event Manager
#include "SGD_EventManager.h"


namespace SGD
{
	//*****************************************************************//
	// LISTENER METHODS

	// Overloaded constructor
	Listener::Listener( const void* child )
	{
		// Sanity-check the parameter
		assert( child != nullptr && "Listener::Listener - self cannot be null (should be the child object)" );

		m_pChild = child;
	}

	// Destructor
	/*virtual*/ Listener::~Listener( void )
	{
		// Unregister from all events
		UnregisterFromEvent();
	}

	// EventManager Interactions:
	bool Listener::RegisterForEvent( const char* eventID )
	{
		return EventManager::GetInstance()->RegisterForEvent( this, eventID );
	}

	bool Listener::UnregisterFromEvent( const char* eventID )
	{
		return EventManager::GetInstance()->UnregisterFromEvent( this, eventID );
	}

	// Listener Interface:
	/*virtual*/ void Listener::HandleEvent( const SGD::Event* pEvent )
	{
		// DUMMY IMPLEMENTATION
		(void)pEvent;
	}
	//*****************************************************************//


}	// namespace SGD

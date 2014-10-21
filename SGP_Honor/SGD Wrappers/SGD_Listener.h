/***********************************************************************\
|																		|
|	File:			SGD_Listener.h   									|
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

#ifndef SGD_LISTENER_H
#define SGD_LISTENER_H


namespace SGD
{
	// Forward declaration
	class Event;


	//*****************************************************************//
	// Listener
	//	- parent class for objects registered with the Event Manager
	//	- children classes override the HandleEvent method, which is called
	//	  when a registered Event is processed
	//	- MUST be given a pointer to the child object when constructed!
	class Listener
	{
	public:
		Listener			( const void* child );		// Overloaded constructor
		virtual ~Listener	( void );					// Destructor


		// EventManager Interactions:
		bool			RegisterForEvent	( const char* eventID );
		bool			UnregisterFromEvent	( const char* eventID = nullptr );
		const void*		GetChild			( void ) const	{	return m_pChild;	}
		

		// Listener Interface:
		virtual void	HandleEvent			( const SGD::Event* pEvent );	// Callback function to process events

	private:
		const void*		m_pChild;	// address of the uncasted child object
	};

}	// namespace SGD

#endif	//SGD_LISTENER_H

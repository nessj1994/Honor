/***********************************************************************\
|																		|
|	File:			SGD_EventManager.cpp   								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-03-10											|
|																		|
|	Purpose:		To queue events for later processing by				|
|					the registered listeners' callback functions		|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#include "SGD_EventManager.h"


// Uses assert for debug breaks
#include <cassert>

// Uses OutputDebugString for debug text
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstring>

// Uses std::string for mapping event IDs
#include <string>

// Uses std::queue for storing events
#include <queue>

// Uses std::multimap for storing listeners
#include <map>

// Uses std::vector for storing current event listeners
#include <vector>

// Uses Event & Listener
#include "SGD_Event.h"
#include "SGD_Listener.h"


namespace SGD
{
	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//
		// EventManager
		//	- concrete class for sending Events to registered IListeners
		class EventManager : public SGD::EventManager
		{
		public:
			// SINGLETON
			static	EventManager*	GetInstance		( void );
			static	void			DeleteInstance	( void );
		

			virtual	bool		Initialize			( void )					override;
			virtual	bool		Update				( void )					override;
			virtual	bool		Terminate			( void )					override;

			virtual bool		RegisterForEvent	( Listener* listener, const char* eventID )			override;
			virtual bool		UnregisterFromEvent	( Listener* listener, const char* eventID )			override;

			virtual bool		QueueEvent			( const Event* pEvent, const void* destination )	override;
			virtual bool		SendEventNow		( const Event* pEvent, const void* destination )	override;

			virtual bool		ClearEvents			( void )					override;
			

		private:
			// SINGLETON
			static	EventManager*		s_Instance;		// the ONE instance

			EventManager				( void )				= default;		// Default constructor
			virtual	~EventManager		( void )				= default;		// Destructor

			EventManager				( const EventManager& )	= delete;		// Copy constructor
			EventManager&	operator=	( const EventManager& )	= delete;		// Assignment operator


			// Wrapper Status
			enum EEventManagerStatus
			{	
				E_UNINITIALIZED,
				E_INITIALIZED,
				E_DESTROYED
			};

			EEventManagerStatus			m_eStatus	= E_UNINITIALIZED;		// wrapper initialization status
			
			typedef	std::pair< const Event*, const void* >	EventDestinationPair;
			typedef std::queue< EventDestinationPair >		EventQueue;
			EventQueue					m_qEvents;							// event queue

			typedef std::multimap< EventID, Listener* >	ListenerMap;
			typedef ListenerMap::const_iterator			LMapIter;
			typedef ListenerMap::value_type				LMapValue;
			typedef std::pair< LMapIter, LMapIter >		LMapRange;
			typedef std::vector< Listener* >			ListenerVector;
			ListenerMap					m_mListeners;				// registered listeners
			ListenerMap					m_mUnlisteners;				// unregistered listeners

		};
		//*************************************************************//

	}	// namespace SGD_IMPLEMENTATION



	//*****************************************************************//
	// Interface singleton accessor
	/*static*/ EventManager* EventManager::GetInstance( void )
	{
		// Return the singleton
		return (SGD::EventManager*)SGD_IMPLEMENTATION::EventManager::GetInstance();
	}

	// Interface singleton destructor
	/*static*/ void EventManager::DeleteInstance( void )
	{
		// Deallocate singleton
		return SGD_IMPLEMENTATION::EventManager::DeleteInstance();
	}
	//*****************************************************************//



	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//	
		// SINGLETON

		// Instantiate static pointer to null (no instance yet)
		/*static*/ EventManager* EventManager::s_Instance = nullptr;

		// Singleton accessor
		/*static*/ EventManager* EventManager::GetInstance( void )
		{
			// Allocate singleton on first use
			if( EventManager::s_Instance == nullptr )
				EventManager::s_Instance = new EventManager;

			// Return the singleton
			return EventManager::s_Instance;
		}

		// Singleton destructor
		/*static*/ void EventManager::DeleteInstance( void )
		{
			// Deallocate singleton
			delete EventManager::s_Instance;
			EventManager::s_Instance = nullptr;
		}
		//*************************************************************//



		//*************************************************************//
		// INITIALIZE
		bool EventManager::Initialize( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus != E_INITIALIZED && "EventManager::Initialize - wrapper has already been initialized" );
			if( m_eStatus == E_INITIALIZED )
				return false;

			
			// Success!
			m_eStatus = E_INITIALIZED;
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// UPDATE
		bool EventManager::Update( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "EventManager::Update - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;


			// Iterate through the entire queue
			while( m_qEvents.empty() == false )
			{
				// Dequeue the first event
				EventDestinationPair eventPair = m_qEvents.front();
				m_qEvents.pop();

				
				// Hold a local ref to the ID (for readability)
				const EventID& id = eventPair.first->GetEventID();


				// Send the event to all registered listeners for processing
				LMapRange range = m_mListeners.equal_range( id );
				if( range.first != range.second )
				{
					// Copy the intended listeners
					ListenerVector vec;

					// All listeners?
					if( eventPair.second == nullptr )
					{
						for( LMapIter iter = range.first; iter != range.second; ++iter )
							vec.push_back( iter->second );
					}
					else 
					{
						// One intended listener (which may not exist)
						for( LMapIter iter = range.first; iter != range.second; ++iter )
						{
							if( iter->second->GetChild() == eventPair.second )
							{
								vec.push_back( iter->second );
								break;
							}
						}
					}

					// Send the event to the current listeners
					for( unsigned int i = 0; i < vec.size(); i++ )
					{
						// Has a listener been removed from this event?
						if( m_mUnlisteners.empty() == false )
						{
							LMapRange removed = m_mUnlisteners.equal_range( id );
							if( removed.first != removed.second )
							{
								// Is this listener marked for removal from this event?
								LMapIter iter = std::find( removed.first, removed.second, LMapValue( id, vec[i] ) );
								if( iter != removed.second )
								{
									m_mUnlisteners.erase( iter );

									// Skip over the HandleEvent
									continue;
								}
							}
						}

						// Send event
						vec[ i ]->HandleEvent( eventPair.first );
					}
				}
				
				// Deallocate the event
				delete eventPair.first;
			}


			// Unregistered listeners have all been processed
			m_mUnlisteners.clear();
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// TERMINATE
		bool EventManager::Terminate( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "EventManager::Terminate - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			
			// Deallocate all events in the queue
			while( m_qEvents.empty() == false )
			{
				delete m_qEvents.front().first;
				m_qEvents.pop();
			}

			// Remove the registered listeners
			m_mListeners.clear();
			m_mUnlisteners.clear();


			m_eStatus = E_DESTROYED;
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// REGISTER FOR EVENT
		bool EventManager::RegisterForEvent( Listener* listener, const char* eventID )
		{	
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "EventManager::RegisterForEvent - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Sanity-check the parameters
			assert( listener != nullptr && "EventManager::RegisterForEvent - listener cannot be null" );
			if( listener == nullptr )
				return false;

			assert( eventID != nullptr && "EventManager::RegisterForEvent - event ID cannot be null" );
			if( eventID == nullptr )
				return false;


			// Convert the C-style string parameter to an EventID
			EventID id = eventID;


			// Check if the listener is NOT already registered
			LMapRange range = m_mListeners.equal_range( id );
			for( LMapIter iter = range.first; iter != range.second; ++iter )
				if( iter->second == listener )
					return true;		// already registered!


			// Register the new listener
			m_mListeners.insert( LMapValue( id, listener ) );
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// UNREGISTER FROM EVENT
		bool EventManager::UnregisterFromEvent( Listener* listener, const char* eventID )
		{	
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "EventManager::UnregisterFromEvent - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Sanity-check the listener parameter
			assert( listener != nullptr && "EventManager::UnregisterFromEvent - listener cannot be null" );
			if( listener == nullptr )
				return false;


			// Should we unregister from all events?
			if( eventID == nullptr )
			{
				// Unregister from all events
				LMapIter iter = m_mListeners.begin(); 
				while( iter != m_mListeners.end() )
				{
					if( iter->second == listener )
					{
						m_mUnlisteners.insert( *iter );
						iter = m_mListeners.erase( iter );
						continue;	// may be listening to another event
					}
					
					++iter;
				}
			}
			else 
			{
				// Find all registered listeners for the event
				EventID id = eventID;
				LMapRange range = m_mListeners.equal_range( id );
				
				// Unregister from the event
				LMapIter iter = range.first; 
				while( iter != range.second )
				{
					if( iter->second == listener )
					{
						m_mUnlisteners.insert( *iter );
						iter = m_mListeners.erase( iter );
						break;		// cannot be listening to the same event again
					}
					
					++iter;
				}
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// QUEUE EVENT
		bool EventManager::QueueEvent( const Event* pEvent, const void* destination )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "EventManager::QueueEvent - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Sanity-check the parameter
			assert( pEvent != nullptr && "EventManager::QueueEvent - event parameter cannot be null" );
			if( pEvent == nullptr )
				return false;

			
			// Queue the event
			m_qEvents.push( EventDestinationPair{ pEvent, reinterpret_cast< const Listener* >( destination ) } );
			
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// SEND EVENT NOW
		bool EventManager::SendEventNow( const Event* pEvent, const void* destination )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "EventManager::SendMessageNow - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;
			
			// Sanity-check the parameter
			assert( pEvent != nullptr && "EventManager::SendEventNow - event parameter cannot be null" );
			if( pEvent == nullptr )
				return false;

				
			// Hold a local ref (for readability)
			const EventID& id = pEvent->GetEventID();


			// Send the event to all registered listeners to process
			// (does not deallocate the event)
			LMapRange range = m_mListeners.equal_range( id );
			if( range.first != range.second )
			{
				// Copy the intended listeners
				ListenerVector vec;

				// All listeners?
				if( destination == nullptr )
				{
					for( LMapIter iter = range.first; iter != range.second; ++iter )
						vec.push_back( iter->second );
				}
				else 
				{
					// One intended listener (which may not exist)
					for( LMapIter iter = range.first; iter != range.second; ++iter )
					{
						if( iter->second->GetChild() == destination )
						{
							vec.push_back( iter->second );
							break;
						}
					}
				}


				// Send the event to the current listeners
				for( unsigned int i = 0; i < vec.size(); i++ )
				{
					// Has this listener been removed from the event?
					if( m_mUnlisteners.empty() == false )
					{
						LMapRange removed = m_mUnlisteners.equal_range( id );
						if( removed.first != removed.second )
						{
							LMapIter iter = std::find( removed.first, removed.second, LMapValue( id, vec[i] ) );
							if( iter != removed.second )
								continue;
						}
					}

					// Send event
					vec[ i ]->HandleEvent( pEvent );
				}
			}

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// CLEAR EVENTS
		bool EventManager::ClearEvents( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "EventManager::ClearEvents - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			
			// Deallocate all messages in the queue
			while( m_qEvents.empty() == false )
			{
				delete m_qEvents.front().first;
				m_qEvents.pop();
			}


			// Unregistered listeners have been effectively been processed
			m_mUnlisteners.clear();
			return true;
		}
		//*************************************************************//
		

	}	// namespace SGD_IMPLEMENTATION

}	// namespace SGD

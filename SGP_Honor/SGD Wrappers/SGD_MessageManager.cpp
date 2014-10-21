/***********************************************************************\
|																		|
|	File:			SGD_MessageManager.cpp 								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-05-24											|
|																		|
|	Purpose:		To queue messages for later processing by			|
|					the registered callback function					|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#include "SGD_MessageManager.h"


// Uses assert for debug breaks
#include <cassert>

// Uses OutputDebugString for debug text
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstring>

// Uses TSTRING for text
#include "SGD_String.h"

// Uses std::queue for storing messages
#include <queue>

// Uses Message (virtual destructor)
#include "SGD_Message.h"


namespace SGD
{
	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//
		// MessageManager
		//	- concrete class for queuing messages sent to a specified function
		class MessageManager : public SGD::MessageManager
		{
		public:
			// SINGLETON
			static	MessageManager*		GetInstance		( void );
			static	void				DeleteInstance	( void );

			
			virtual	bool		Initialize				( void (*pfMessageProc)( const Message* ) )	override;
			virtual	bool		Update					( void )				override;
			virtual	bool		Terminate				( void )				override;

			virtual bool		QueueMessage			( const Message* pMsg )	override;
			virtual bool		SendMessageNow			( const Message* pMsg )	override;

			virtual bool		ClearMessages			( void )				override;
						

		private:
			// SINGLETON
			static	MessageManager*		s_Instance;		// the ONE instance

			MessageManager				( void )					= default;		// Default constructor
			virtual	~MessageManager		( void )					= default;		// Destructor

			MessageManager				( const MessageManager& )	= delete;		// Copy constructor
			MessageManager&	operator=	( const MessageManager& )	= delete;		// Assignment operator


			// Wrapper Status
			enum EMessageManagerStatus
			{	
				E_UNINITIALIZED,
				E_INITIALIZED,
				E_DESTROYED
			};

			EMessageManagerStatus		m_eStatus	= E_UNINITIALIZED;		// wrapper initialization status
			
			typedef std::queue< const Message* > MessageQueue;
			MessageQueue				m_qMessages;						// message queue

			typedef void (*MessageProcedure)( const Message* );
			MessageProcedure			m_pCallback	= nullptr;				// callback function
		};
		//*************************************************************//

	}	// namespace SGD_IMPLEMENTATION



	//*****************************************************************//
	// Interface singleton accessor
	/*static*/ MessageManager* MessageManager::GetInstance( void )
	{
		// Return the singleton
		return (SGD::MessageManager*)SGD_IMPLEMENTATION::MessageManager::GetInstance();
	}

	// Interface singleton destructor
	/*static*/ void MessageManager::DeleteInstance( void )
	{
		// Deallocate singleton
		return SGD_IMPLEMENTATION::MessageManager::DeleteInstance();
	}
	//*****************************************************************//



	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//	
		// SINGLETON

		// Instantiate static pointer to null (no instance yet)
		/*static*/ MessageManager* MessageManager::s_Instance = nullptr;

		// Singleton accessor
		/*static*/ MessageManager* MessageManager::GetInstance( void )
		{
			// Allocate singleton on first use
			if( MessageManager::s_Instance == nullptr )
				MessageManager::s_Instance = new MessageManager;

			// Return the singleton
			return MessageManager::s_Instance;
		}

		// Singleton destructor
		/*static*/ void MessageManager::DeleteInstance( void )
		{
			// Deallocate singleton
			delete MessageManager::s_Instance;
			MessageManager::s_Instance = nullptr;
		}
		//*************************************************************//



		//*************************************************************//
		// INITIALIZE
		bool MessageManager::Initialize( void (*pfMessageProc)( const Message* ) )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus != E_INITIALIZED && "MessageManager::Initialize - wrapper has already been initialized" );
			if( m_eStatus == E_INITIALIZED )
				return false;

			// Sanity-check the parameter
			assert( pfMessageProc != nullptr && "MessageManager::Initialize - callback function cannot be null" );
			if( pfMessageProc == nullptr )
				return false;


			// Store the callback function
			m_pCallback = pfMessageProc;


			// Success!
			m_eStatus = E_INITIALIZED;
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// UPDATE
		bool MessageManager::Update( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "MessageManager::Update - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;


			// Iterate through the entire queue
			while( m_qMessages.empty() == false )
			{
				(*m_pCallback)( m_qMessages.front() );		// send the message to the callback function to process
				delete m_qMessages.front();					// deallocate the message (virtual destructor)
				m_qMessages.pop();							// remove the message from the queue
			}


			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// TERMINATE
		bool MessageManager::Terminate( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "MessageManager::Terminate - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			
			// Deallocate all messages in the queue
			while( m_qMessages.empty() == false )
			{
				delete m_qMessages.front();
				m_qMessages.pop();
			}

			// Remove the callback function
			m_pCallback = nullptr;


			m_eStatus = E_DESTROYED;
			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// QUEUE MESSAGE
		bool MessageManager::QueueMessage( const Message* pMsg )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "MessageManager::QueueMessage - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			// Sanity-check the parameter
			assert( pMsg != nullptr && "MessageManager::QueueMessage - message parameter cannot be null" );
			if( pMsg == nullptr )
				return false;

			
			// Queue the message
			m_qMessages.push( pMsg );


			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// SEND MESSAGE NOW
		bool MessageManager::SendMessageNow( const Message* pMsg )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "MessageManager::SendMessageNow - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;
			
			// Sanity-check the parameter
			assert( pMsg != nullptr && "MessageManager::SendMessageNow - message parameter cannot be null" );
			if( pMsg == nullptr )
				return false;
			

			// Send the message to the callback function to process
			// (does not deallocate the message)
			(*m_pCallback)( pMsg );


			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// CLEAR MESSAGES
		bool MessageManager::ClearMessages( void )
		{
			// Sanity-check the wrapper's status
			assert( m_eStatus == E_INITIALIZED && "MessageManager::ClearMessages - wrapper has not been initialized" );
			if( m_eStatus != E_INITIALIZED )
				return false;

			
			// Deallocate all messages in the queue
			while( m_qMessages.empty() == false )
			{
				delete m_qMessages.front();
				m_qMessages.pop();
			}


			return true;
		}
		//*************************************************************//
		

	}	// namespace SGD_IMPLEMENTATION

}	// namespace SGD

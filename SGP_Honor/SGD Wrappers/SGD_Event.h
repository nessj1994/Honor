/***********************************************************************\
|																		|
|	File:			SGD_Event.h   										|
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

#ifndef SGD_EVENT_H
#define SGD_EVENT_H


namespace SGD
{
	//*****************************************************************//
	// EventID
	//	- 32 character string (optimized for comparisons)
	class EventID
	{
	public:
		EventID( void )										// default constructor
			{	id[ 0 ] = 0;	}
		EventID( const char* str );							// overloaded constructor

		char&		operator [] ( unsigned int i )			// array accessor
			{	return id[ i ];	}
		const char& operator [] ( unsigned int i ) const	// const array accessor
			{	return id[ i ];	}

		bool operator == ( const EventID& other ) const;	// is-equal-to
		bool operator != ( const EventID& other ) const;	// not-equal-to
		bool operator <  ( const EventID& other ) const;	// less-than
		
		bool operator == ( const char* str )	  const;	// is-equal-to string
		bool operator != ( const char* str )	  const;	// not-equal-to string
		
		friend bool operator == ( const char* str, const EventID& other );	// string is-equal-to
		friend bool operator != ( const char* str, const EventID& other );	// string not-equal-to

	private:
		enum { SIZE = 32 };	// compile-time constant
		char id[ SIZE ];	// event ID (limited to 32 characters!)
	};


	//*****************************************************************//
	// Event
	//	- data packet sent to registered listeners
	//	- children classes can be derived to store more specific data
	class Event
	{
	public:
		// Overloaded constructor
		Event	(	const char*		eventID,						// event ID / name (limited to 32 characters!)
					void*			data			= nullptr,		// attached data (any type)
					void*			sender			= nullptr		// object that sent the event
				);

		// Destructor
		virtual ~Event	( void )	= default;


		// EventManager Interaction:
		bool			QueueEvent		( const void* destination = nullptr )	const;	// event will be stored & deallocated by the EventManager
		bool			SendEventNow	( const void* destination = nullptr )	const;	// event will NOT be stored or deallocated!


		// Accessors:
		const EventID&	GetEventID		( void )	const	{	return m_ID;			}
		void*			GetData			( void )	const	{	return m_pData;			}
		void*			GetSender		( void )	const	{	return m_pSender;		}


	private:
		Event				( const Event& )	= delete;	// Copy constructor
		Event& operator=	( const Event& )	= delete;	// Assignment operator

		// members:
		EventID			m_ID;					// event ID (limited to 32 characters!)
		void*			m_pData;				// attached data (can point to any type)
		void*			m_pSender;				// object that sent the event
	};

}	// namespace SGD

#endif	//SGD_EVENT_H

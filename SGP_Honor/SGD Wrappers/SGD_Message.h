/***********************************************************************\
|																		|
|	File:			SGD_Message.h   									|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To store data, which is sent through the			|
|					MessageManager to the registered callback function	|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_MESSAGE_H
#define SGD_MESSAGE_H

//*********************************************************************//
// Forward enum class declaration (MUST BE DEFINED SOMEWHERE)
enum class MessageID;


namespace SGD
{
	//*****************************************************************//
	// Message
	//	- data packet sent to the function registered with the Message Manager
	//	- children classes can be derived to store more specific data
	class Message
	{
	public:
		// Overloaded constructor
		Message( MessageID ID )	{	m_nMessageID = ID;		}

		// Destructor
		virtual	~Message( void )	= default;
		

		// MessageManager Interaction:
		bool	QueueMessage	( void )	const;			// message will be stored & deallocated by the MessageManager
		bool	SendMessageNow	( void )	const;			// message will NOT be stored or deallocated!


		// Accessors:
		MessageID		GetMessageID	( void )	const	{	return m_nMessageID;	}
		
	private:
		Message				( const Message& )	= delete;	// Copy constructor
		Message& operator=	( const Message& )	= delete;	// Assignment operator

		// members:
		MessageID		m_nMessageID;		// message ID
	};

}	// namespace SGD

#endif	//SGD_MESSAGE_H

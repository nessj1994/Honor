/***********************************************************************\
|																		|
|	File:			SGD_MessageManager.h   								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To queue messages for later processing by			|
|					the registered callback function					|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_MESSAGEMANAGER_H
#define SGD_MESSAGEMANAGER_H


namespace SGD
{
	// Forward declaration
	class Message;
	

	//*****************************************************************//
	// MessageManager
	//	- SINGLETON class for queuing messages sent to a specified function
	class MessageManager
	{
	public:
		static	MessageManager*		GetInstance		( void );
		static	void				DeleteInstance	( void );
		

		virtual	bool		Initialize				( void (*pfMessageProc)( const Message* ) )	= 0;
		virtual	bool		Update					( void )				= 0;
		virtual	bool		Terminate				( void )				= 0;

		virtual bool		QueueMessage			( const Message* pMsg )	= 0;
		virtual bool		SendMessageNow			( const Message* pMsg )	= 0;

		virtual bool		ClearMessages			( void )				= 0;

		
	protected:
		MessageManager				( void )					= default;
		virtual	~MessageManager		( void )					= default;

		MessageManager				( const MessageManager& )	= delete;
		MessageManager& operator=	( const MessageManager& )	= delete;
	};

}	// namespace SGD

#endif	//SGD_MESSAGEMANAGER_H

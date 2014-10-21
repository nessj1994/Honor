/***********************************************************************\
|																		|
|	File:			SGD_Message.cpp  									|
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

#include "SGD_Message.h"


// Uses Message Manager
#include "SGD_MessageManager.h"


namespace SGD
{

	// MessageManager Interaction:
	bool Message::QueueMessage( void ) const
	{
		return MessageManager::GetInstance()->QueueMessage( this );
	}

	bool Message::SendMessageNow( void ) const
	{
		return MessageManager::GetInstance()->SendMessageNow( this );
	}

}	// namespace SGD

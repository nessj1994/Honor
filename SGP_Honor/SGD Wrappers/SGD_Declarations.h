/***********************************************************************\
|																		|
|	File:			SGD_Declarations.h  								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To forward declare all SGD wrapper classes			|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_DECLARATIONS_H
#define SGD_DECLARATIONS_H


namespace SGD
{
	class AudioManager;
	class GraphicsManager;
	class InputManager;

	class HTexture;
	class HAudio;
	class HVoice;
	
	class Color;
	class Rectangle;
	class Point;
	class Vector;
	class Size;

	class Message;
	class MessageManager;
	class Event;
	class EventID;
	class EventManager;
	class Listener;

}	// namespace SGD

#endif	//SGD_DECLARATIONS_H

/***********************************************************************\
|																		|
|	File:			SGD_Color.h  										|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To store color components							|
|					alpha, red, green, blue								|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_COLOR_H
#define SGD_COLOR_H


namespace SGD
{
	//*****************************************************************//
	// Color
	//	- 32-bit color with alpha, red, green, blue components
	class Color 
	{
	public:
		unsigned char alpha;	// alpha (opacity) component
		unsigned char red;		// red component
		unsigned char green;	// green component
		unsigned char blue;		// blue component


		// Default constructor (white)
		Color( void )
			: alpha( 255 ), red( 255 ), green( 255 ), blue( 255 )
		{	}

		// RGB constructor
		Color( unsigned char R, unsigned char G, unsigned char B )
			: alpha( 255 ), red( R ), green( G ), blue( B )
		{	}

		// ARGB constructor
		Color( unsigned char A, unsigned char R, unsigned char G, unsigned char B )
			: alpha( A ), red( R ), green( G ), blue( B )
		{	}


		// Typecast to unsigned long
		operator unsigned long ( void ) const 
		{
			return ( (alpha << 24) | (red << 16) | (green << 8) | (blue) );
		}
	};

}	// namespace SGD

#endif	//SGD_COLOR_H

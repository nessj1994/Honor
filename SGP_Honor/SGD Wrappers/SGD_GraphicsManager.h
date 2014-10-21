/***********************************************************************\
|																		|
|	File:			SGD_GraphicsManager.h  								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To render simple geometry and						|
|					load and render image files							|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_GRAPHICSMANAGER_H
#define SGD_GRAPHICSMANAGER_H


#include "SGD_Handle.h"			// Accesses data using HTexture handles
#include "SGD_Geometry.h"		// Uses floating-point Rectangles & Points
#include "SGD_Color.h"			// Uses Color for blending


namespace SGD
{
	// Forward declaration of global variable
	extern const float PI;

	
	//*****************************************************************//
	// GraphicsManager
	//	- SINGLETON class for rendering text, geometry, and textures
	//	- supports .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga files
	//	- texture dimensions will be rounded up to the nearest power of 2 (e.g. 2,4,8,16,32,64, etc.)
	class GraphicsManager
	{
	public:
		static	GraphicsManager*	GetInstance		( void );
		static	void				DeleteInstance	( void );


		virtual	bool		Initialize			( bool vsync = true )	= 0;
		virtual	bool		Update				( void )				= 0;
		virtual	bool		Terminate			( void )				= 0;
			
		
		virtual bool		SetClearColor		( Color color = {0,0,0} )				= 0;
		virtual bool		Resize				( Size size, bool windowed = true )		= 0;


		virtual bool		DrawString			( const wchar_t* text, Point position,  Color color = {} )										= 0;
		virtual bool		DrawString			( const char* text, Point position,  Color color = {} )											= 0;
		virtual bool		DrawLine			( Point position1, Point position2, Color color = {}, unsigned int lineWidth = 3 )				= 0;
		virtual bool		DrawRectangle		( Rectangle rect, Color fillColor, Color lineColor = {0,0,0,0}, unsigned int lineWidth = 3 )	= 0;


		virtual	HTexture	LoadTexture			( const wchar_t* filename, Color colorKey = {0,0,0,0} )		= 0;
		virtual	HTexture	LoadTexture			( const char* filename, Color colorKey = {0,0,0,0} )		= 0;
		virtual	bool		DrawTexture			( HTexture handle, Point position, float rotation = 0.0f, Vector rotationOffset = {}, Color color = {}, Size scale = {1.0f, 1.0f} )						= 0;
		virtual	bool		DrawTextureSection	( HTexture handle, Point position, Rectangle section, float rotation = 0.0f, Vector rotationOffset = {}, Color color = {}, Size scale = {1.0f, 1.0f} )	= 0;
		virtual	bool		UnloadTexture		( HTexture& handle )										= 0;
		virtual SGD::Size GetTextureSize(SGD::HTexture _Texture) = 0;


	protected:
		GraphicsManager					( void )					= default;
		virtual	~GraphicsManager		( void )					= default;

		GraphicsManager					( const GraphicsManager& )	= delete;
		GraphicsManager&	operator=	( const GraphicsManager& )	= delete;
	};

}	// namespace SGD

#endif	//SGD_GRAPHICSMANAGER_H

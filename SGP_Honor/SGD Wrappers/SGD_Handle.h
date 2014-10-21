/***********************************************************************\
|																		|
|	File:			SGD_Handle.h    									|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To store a unique value to identify the location	|
|					of an element within a container					|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_HANDLE_H
#define SGD_HANDLE_H


namespace SGD
{
	// Forward declarations
	class HTexture;
	class HAudio;
	class HVoice;


	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//
		// Handle
		//	- generic 32-bit handle identifier
		class Handle
		{
		public:		
			Handle( void )								: m_ulHandle( 0 )	{	}					// default constructor
		
			// Comparison operators
			bool operator == ( const Handle& h ) const	{	return m_ulHandle == h.m_ulHandle;	}	// comparison for equality
			bool operator != ( const Handle& h ) const	{	return m_ulHandle != h.m_ulHandle;	}	// comparison for inequality
			bool operator <  ( const Handle& h ) const	{	return m_ulHandle <  h.m_ulHandle;	}	// comparison for less than

		private:
			unsigned long	m_ulHandle;																// encrypted location

			// Mutators
			Handle( unsigned long h )					: m_ulHandle( h )	{	}					// overloaded constructor
			Handle& operator= ( unsigned long h )		{	m_ulHandle = h;	return *this;	}		// assignment operator
		
			// Only HandleDecoder can get/set the handle value
			friend class HandleDecoder;
		};

		
		// Forward declarations
		class GraphicsManager;
		class AudioManager;

	}	// namespace SGD_IMPLEMENTATION


/* Derived Handle Macro */
#define MAKE_DERIVED_HANDLE( name, manager )							\
																		\
	class name	: private SGD_IMPLEMENTATION::Handle					\
	{																	\
	public:																\
		/* Constructor */												\
		name( const Handle& h = Handle() )	: Handle( h )	{	}		\
																		\
		/* Comparison operators for parent type */						\
		using Handle::operator ==;										\
		using Handle::operator !=;										\
		using Handle::operator <;										\
																		\
		/* Comparison operators */										\
		bool operator == ( const name& h ) const						\
			{	return Handle(*this) == Handle(h);	}					\
		bool operator != ( const name& h ) const						\
			{	return Handle(*this) != Handle(h);	}					\
		bool operator <  ( const name& h ) const						\
			{	return Handle(*this) <  Handle(h);	}					\
																		\
		/* Only <manager> can upcast to a Handle */						\
		friend class manager;											\
	}																	/*end*/

	
	//*****************************************************************//
	// HTexture, HAudio, HVoice
	//	- handle typenames used exclusively by their respective manager
	MAKE_DERIVED_HANDLE( HTexture,	SGD_IMPLEMENTATION::GraphicsManager	);
	MAKE_DERIVED_HANDLE( HAudio,	SGD_IMPLEMENTATION::AudioManager	);
	MAKE_DERIVED_HANDLE( HVoice,	SGD_IMPLEMENTATION::AudioManager	);

#undef MAKE_DERIVED_HANDLE
	

	//*****************************************************************//
	// Global identifier for invalid handles
	const SGD_IMPLEMENTATION::Handle INVALID_HANDLE;
	
}	// namespace SGD

#endif //SGD_HANDLE_H

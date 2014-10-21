/***********************************************************************\
|																		|
|	File:			SGD_HandleManager.h    								|
|	Author:			Douglas Monroe										|
|	Last Modified:	2014-02-03											|
|																		|
|	Purpose:		To store data with unique identifiers				|
|																		|
|	© 2014 Full Sail, Inc. All rights reserved. The terms "Full Sail", 	|
|	"Full Sail University", and the Full Sail University logo are 	   	|
|	either registered service marks or service marks of Full Sail, Inc.	|
|																		|
\***********************************************************************/

#ifndef SGD_HANDLEMANAGER_H
#define SGD_HANDLEMANAGER_H


#include "SGD_Handle.h"
#include <vector>
#include <list>


namespace SGD
{
	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//
		// HandleManager<>
		//	- templated data storage, using compressed handle identifiers
		template< typename DataType >
		class HandleManager
		{
		public:
			HandleManager	( void );					// Default constructor
			~HandleManager	( void );					// Destructor


			Handle			StoreData		( DataType data );
			bool			IsHandleValid	( Handle handle ) const;
			DataType*		GetData			( Handle handle ) const;
			bool			RemoveData		( Handle handle, DataType* data );
			bool			Clear			( void );

			template< typename TExtraInfo >
			void			ForEach			( bool (*pFunction)( Handle handle, DataType& data, TExtraInfo* extra ), TExtraInfo* extra ) const;


		private:
			HandleManager				( const HandleManager& )	= delete;	// Copy constructor
			HandleManager&	operator=	( const HandleManager& )	= delete;	// Assignment operator


			// Type names for data architecture
			typedef std::pair	< Handle, DataType >		DataPair;
			typedef std::vector	< DataPair >				DataVector;
			typedef std::list	< Handle >					HandleList;

			// Data Storage:
			DataVector			m_vData;					// Vector of handle-data pairs
			HandleList			m_lFreeIndices;				// Linked-list of empty locations in the vector
		};

	};	// namespace SGD_IMPLEMENTATION

}	// namespace SGD


// Template definitions are within the .hpp
#define	INC_SGD_HANDLE_MANAGER_HPP
#include "SGD_HandleManager.hpp"
#undef	INC_SGD_HANDLE_MANAGER_HPP

#endif //SGD_HANDLEMANAGER_H

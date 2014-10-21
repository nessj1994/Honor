/***********************************************************************\
|																		|
|	File:			SGD_HandleManager.hpp 								|
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

// This .hpp can ONLY be included from SGD_HandleManager.h
#ifndef INC_SGD_HANDLE_MANAGER_HPP
#error	FILE "SGD_HandleManager.hpp" CANNOT BE INCLUDED EXPLICITLY
#else


#include <cassert>


namespace SGD
{
	namespace SGD_IMPLEMENTATION
	{
		//*************************************************************//
		// Handle Components
		//	- union interprets between handle and components
		//	- bitfield enforces how much data the component can store
		//	  rrrrrrrr iiiiiiii iiiiiiii iiiiiiii
		class HandleDecoder
		{
			#define BITS_REUSE	(8)
			#define BITS_INDEX	(32 - BITS_REUSE)

			#define MAX_REUSES	((1 << BITS_REUSE) - 1)
			#define MAX_INDICES	((1 << BITS_INDEX) - 1)

			union UHandle
			{
				unsigned long		handle;					// Handle data
	
				struct /* anonymous */
				{
					unsigned long	index	: BITS_INDEX;	// index into the vector
					unsigned long	reuse	: BITS_REUSE;	// reuse number
				};
			};

		public:
			// Handle Conversion
			//	- inline function attempts to optimize for speed
			static inline unsigned long HandleToReuse( Handle handle )
			{
				UHandle convert = { handle.m_ulHandle };
				return convert.reuse;
			}
	
			static inline unsigned long HandleToIndex( Handle handle )
			{
				UHandle convert = { handle.m_ulHandle };
				return convert.index;
			}
	
			static inline Handle CreateHandle( unsigned long reuse, unsigned long index )
			{
				assert( index <= MAX_INDICES && "HandleDecoder::CreateHandle - index exceeds maximum value!" );
				if( index > MAX_INDICES )
					return SGD::INVALID_HANDLE;

				UHandle convert = { };
				convert.reuse = reuse;
				convert.index = index;
				return Handle( convert.handle );
			}
	
			static inline Handle ReuseHandle( Handle handle )
			{
				UHandle convert = { handle.m_ulHandle };
	
				if( convert.reuse == MAX_REUSES )
					convert.reuse = 1;
				else
					convert.reuse++;

				return Handle( convert.handle );
			}

			#undef	BITS_REUSE
			#undef	BITS_INDEX

			#undef	MAX_REUSES
			#undef	MAX_INDICES
		};
		//*************************************************************//
	


		//*************************************************************//
		// CONSTRUCTOR
		template< typename DataType >
		HandleManager< DataType >::HandleManager( void )
		{
			// Allocate some room in the vector
			m_vData.reserve( 4 );
		}
		//*************************************************************//


	
		//*************************************************************//
		// DESTRUCTOR
		template< typename DataType >
		HandleManager< DataType >::~HandleManager( void )
		{
			// Explicitly empty the containers (does not deallocate stored data)
			m_vData.clear();
			m_lFreeIndices.clear();
		}
		//*************************************************************//
	

	
		//*************************************************************//
		// STORE DATA
		// - store data into the list (reusing an empty spot if available)
		// - return the unique handle to the data
		template< typename DataType >
		Handle HandleManager< DataType >::StoreData( DataType data )
		{
			// Are there any free spots available?
			if( m_lFreeIndices.empty() == false )
			{
				// Reuse the last handle in the free list
				Handle handle = m_lFreeIndices.back();
				m_lFreeIndices.pop_back();
				handle = HandleDecoder::ReuseHandle( handle );
				assert( handle != SGD::INVALID_HANDLE && "HandleManager::StoreData - new handle is invalid ... sorry!" );
				if( handle == SGD::INVALID_HANDLE )
					return SGD::INVALID_HANDLE;

				// Store the new data
				unsigned int index = HandleDecoder::HandleToIndex( handle );
				m_vData[ index ].first	= handle;
				m_vData[ index ].second	= data;

				// Return the handle
				return handle;
			}
			else
			{
				// Create a new handle (at the end of the container)
				Handle handle = HandleDecoder::CreateHandle( 1, m_vData.size() );
				assert( handle != SGD::INVALID_HANDLE && "HandleManager::StoreData - new handle is invalid ... sorry!" );
				if( handle == SGD::INVALID_HANDLE )
					return SGD::INVALID_HANDLE;
		
				// Store the new data
				DataPair info;
				info.first		= handle;
				info.second		= data;
				m_vData.push_back( info );
		
				// Return the handle
				return handle;
			}
		}
		//*************************************************************//


		
		//*************************************************************//
		// IS HANDLE VALID
		//	- check if the handle is still valid
		template< typename DataType >
		bool HandleManager< DataType >::IsHandleValid( Handle handle ) const
		{
			// Easy check
			if( handle == SGD::INVALID_HANDLE )
				return false;

			// Verify the index indicated by the handle
			unsigned int index = HandleDecoder::HandleToIndex( handle );
			if( index >= m_vData.size() )
				return false;
	
			// Verify the handle still corresponds to the stored data
			const DataPair& info = m_vData[ index ];
			return (info.first == handle);
		}
		//*************************************************************//
	

	
		//*************************************************************//
		// GET DATA
		//	- return the data stored at the location indicated by the handle
		template< typename DataType >
		DataType* HandleManager< DataType >::GetData( Handle handle ) const
		{
			// Verify the parameter
			assert( handle != SGD::INVALID_HANDLE && "HandleManager::GetData - handle is invalid" );
			if( handle == SGD::INVALID_HANDLE )
				return nullptr;

			// Access the data indicated by the handle
			unsigned int index = HandleDecoder::HandleToIndex( handle );
			assert( index < m_vData.size() && "HandleManager::GetData - handle has invalid index" );
			if( index >= m_vData.size() )
				return nullptr;
	
			// Verify the handle still corresponds to the stored data
			const DataPair& info = m_vData[ index ];
			assert( info.first != SGD::INVALID_HANDLE && "HandleManager::GetData - handle has been removed" );
			assert( info.first == handle && "HandleManager::GetData - handle has expired (another asset is using the location)" );
			if( info.first != handle )
				return nullptr;

			// Return the stored data
			return const_cast< DataType* >(&info.second);
		}
		//*************************************************************//
	

	
		//*************************************************************//
		// REMOVE DATA
		//	- remove the data stored at the location indicated by the handle
		//	- return the data
		template< typename DataType >
		bool HandleManager< DataType >::RemoveData( Handle handle, DataType* data )
		{
			// Verify the parameter
			assert( handle != SGD::INVALID_HANDLE && "HandleManager::GetData - handle is invalid" );
			if( handle == SGD::INVALID_HANDLE )
				return false;

			// Access the data indicated by the handle
			unsigned int index = HandleDecoder::HandleToIndex( handle );
			assert( index < m_vData.size() && "HandleManager::RemoveData - handle has invalid index" );
			if( index >= m_vData.size() )
				return false;
	
			// Verify the handle still corresponds to the stored data
			DataPair& info = m_vData[ index ];
			assert( info.first != SGD::INVALID_HANDLE && "HandleManager::RemoveData - handle has already been removed" );
			assert( info.first == handle && "HandleManager::RemoveData - handle has expired (location has been reused)" );
			if( info.first != handle )
				return false;


			// Store the data in the parameter (if they want it)
			if( data != nullptr )
				*data = info.second;


			// Erase data from the container
			m_vData[ index ].first	= SGD::INVALID_HANDLE;
			m_vData[ index ].second	= DataType();			// calling the constructor may be harmful ... oh well

			// Add the handle into the free list
			m_lFreeIndices.push_back( handle );
			return true;
		}
		//*************************************************************//


		
		//*************************************************************//
		// CLEAR
		//	- remove all handles
		template< typename DataType >
		bool HandleManager< DataType >::Clear( void )
		{
			// Clear the free list
			m_lFreeIndices.clear();

			// Clear the data (does not deallocate individual objects)
			m_vData.clear();
			m_vData.swap( std::move( DataVector() ) );		// force the collapse

			return true;
		}
		//*************************************************************//



		//*************************************************************//
		// FOR EACH
		//	- iterate through the data, calling the provided function for each
		template< typename DataType >
		template< typename TExtraInfo >
		void HandleManager< DataType >::ForEach( bool (*pFunction)( Handle handle, DataType& data, TExtraInfo* extra ), TExtraInfo* extra ) const
		{
			// Verify the callback function
			assert( pFunction != nullptr && "HandleManager::ForEach - invalid function pointer" );

			// Iterate through all the (valid) stored data
			DataVector::const_iterator iter;
			for( iter = m_vData.cbegin(); iter != m_vData.cend(); ++iter )
			{
				if( iter->first != SGD::INVALID_HANDLE )
					// Stop looping if the callback function returns false
					if( pFunction( iter->first, *const_cast< DataType* >(&iter->second), extra ) == false )
						break;
			}
		}
		//*************************************************************//
	

	}	// namespace SGD_IMPLEMENTATION

}	// namespace SGD


#endif //INC_SGD_HANDLE_MANAGER_HPP

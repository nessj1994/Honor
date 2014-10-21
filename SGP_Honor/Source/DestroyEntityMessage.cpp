/**************************************************************************
|	File:	DestroyEntityMessage.cpp
|	Author: Jordan Ness
|	Purpose: The DestroyEntityMessage class is used by the message
|			callback function to allow entities to be destroyed by others
**************************************************************************/

#include "DestroyEntityMessage.h"
#include "MessageID.h"
#include "Entity.h"
#include <cassert>

DestroyEntityMessage::DestroyEntityMessage(Entity* destroy) 
	: SGD::Message(MessageID::MSG_DESTROY_ENTITY)
{
		//Is the parameter null?
		assert(destroy != nullptr
			&& "DestroyEntityMessage - Cannot destroy a nullptr");

		//Store the parameter
		m_pDestroy = destroy;
		m_pDestroy->AddRef();

}


DestroyEntityMessage::~DestroyEntityMessage()
{
	m_pDestroy->Release();
	m_pDestroy = nullptr;

}

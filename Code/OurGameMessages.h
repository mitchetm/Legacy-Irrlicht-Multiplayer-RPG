#pragma once

#include "MessageIdentifiers.h"

enum GameMessages
{
	ID_SEND_CHAT = ID_USER_PACKET_ENUM + 1, // when we send a chat

	// Server usually send request to clients
	// Clients usually send info and confirms to server

	ID_REQUEST_REVISION,
	ID_INFO_REVISION,
	ID_REVISION_BAD,
	ID_REQUEST_USER_DATA,
	ID_INFO_USER_DATA,
	ID_CHARACTER_READY,
	ID_CONFIRM_CHARACTER,
	ID_ADD_CURRENT_USERS,
	ID_CONFIRM_CURRENT_USERS,
	ID_REQUST_ADD_NEW_USER,
	ID_CONFIRM_NEW_USER,
	ID_YOU_ARE_ACTIVE,
	ID_UPDATE,
	ID_REMOVE_USER,

	ID_REQUEST_LOCK_ON, // thsi is to tell the server that we are locked on to someone
	ID_ACCEPT_LOCK_ON, // this tells the client that we now know, and we also send you some info, like its current HP

	ID_UPDATE_HP, // this is to tell the everone that our HP has changed

	ID_START_ATTACK, // we send this to tell the server we are attacking
	ID_ATTACK, // tell clients someone has attacked

	ID_DEAD, // someone died
	ID_RESPAWN, // someone spawned from death

	ID_BLOCK_START, // we start blocking
	ID_BLOCK_END // we stop blocking
};


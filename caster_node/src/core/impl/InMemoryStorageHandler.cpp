//
// Created by Kevin Guthrie on 1/31/16.
//

#include "InMemoryStorageHandler.h"

InMemoryStorageHandler::InMemoryStorageHandler()
: _storage()
{

}

InMemoryStorageHandler::~InMemoryStorageHandler()
{
}

void InMemoryStorageHandler::readFromPath(
        boost::filesystem::path const &readFromPath, MessageBuffer &buffer
        , int sourceLocation, int targetLocation, int bytesToRead)
{

}

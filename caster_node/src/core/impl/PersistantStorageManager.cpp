//
// Created by Kevin Guthrie on 1/28/16.
//

#include "PersistantStorageManager.h"

PersistantStorageManager::~PersistantStorageManager() { }

PersistantStorageManager::PersistantStorageManager(
        std::vector<boost::filesystem::path> storageDirectories)
: _storageDirectories(storageDirectories)
{

}

void PersistantStorageManager::readFromPath(
        boost::filesystem::path const &readFromPath, MessageBuffer &buffer
        , int sourceLocation, int targetLocation, int bytesToRead)
{

}

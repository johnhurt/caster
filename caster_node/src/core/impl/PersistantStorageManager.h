//
// Created by Kevin Guthrie on 1/28/16.
//

#ifndef CASTER_NODE_PERSISTANTSTORAGEMANAGER_H
#define CASTER_NODE_PERSISTANTSTORAGEMANAGER_H

#include <core/StorageManager.h>


/**
 * Basic implementation of the Storage manager that wrotes to the file system
 */
class PersistantStorageManager
: public StorageManager
{
private:

    std::vector<boost::filesystem::path> _storageDirectories;

public:

    PersistantStorageManager(
            std::vector<boost::filesystem::path> storageDirectories);

    void readFromPath(
            boost::filesystem::path const& readFromPath
            , MessageBuffer& buffer
            , int sourceLocation
            , int targetLocation
            , int bytesToRead);

    virtual ~PersistantStorageManager();

};


#endif //CASTER_NODE_PERSISTANTSTORAGEMANAGER_H

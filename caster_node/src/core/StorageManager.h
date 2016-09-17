//
// Created by Kevin Guthrie on 1/28/16.
//

#ifndef CASTER_NODE_STORAGEMANAGER_H
#define CASTER_NODE_STORAGEMANAGER_H

#include <boost/filesystem/path.hpp>

#include "MessageBuffer.h"

/**
 * Interface for interacting with stored data.  This class provides some
 * convenience methods for reading from a storage engine based on a few
 * methods that have to be implemented specifically for the engine
 */
class StorageManager {
public:

    virtual void readFromPath(
            boost::filesystem::path const& readFromPath
            , MessageBuffer& buffer
            , int sourceLocation
            , int targetLocation
            , int bytesToRead) = 0;

    virtual ~StorageManager();
};

#endif //CASTER_NODE_STORAGEMANAGER_H

//
// Created by Kevin Guthrie on 1/31/16.
//

#ifndef CASTER_NODE_INMEMORYSTORAGEHANDLER_H
#define CASTER_NODE_INMEMORYSTORAGEHANDLER_H

#include <core/StorageManager.h>
#include <boost/unordered_map.hpp>

class InMemoryStorageHandler
: public StorageManager
{
private:

    boost::unordered_map<std::string,MessageBuffer> _storage;

public:

    InMemoryStorageHandler();
    virtual ~InMemoryStorageHandler();

    void readFromPath(
            boost::filesystem::path const& readFromPath
            , MessageBuffer& buffer
            , int sourceLocation
            , int targetLocation
            , int bytesToRead);

};


#endif //CASTER_NODE_INMEMORYSTORAGEHANDLER_H

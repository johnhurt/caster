//
// Created by Kevin Guthrie on 10/30/15.
//

#ifndef CASTER_NODE_CASTERNODE_H
#define CASTER_NODE_CASTERNODE_H

#include "boost/shared_ptr.hpp"
#include "CasterNodeConfig.h"
#include "net/NetworkInterface.h"
#include <vector>


class CasterNode
{
private:

    CasterNodeConfig _config;
    NetworkInterface _internalInterface;
    NetworkInterface _externalInterface;
    NetworkInterface _remoteInterface;

public:

    /** Default Constructor */
    CasterNode();

    /** Constructor for the representation of the local caster node */
    CasterNode(CasterNodeConfig const& config);

    CasterNodeConfig const& config()const ;

    NetworkInterface const& internalInterface() const;
    NetworkInterface const& externalInterface() const;
    NetworkInterface const& remoteInterface() const;
};


#endif //CASTER_NODE_CASTERNODE_H

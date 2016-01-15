//
// Created by Kevin Guthrie on 11/9/15.
//

#ifndef CASTER_NODE_HOSTRESOLVER_H
#define CASTER_NODE_HOSTRESOLVER_H

#include <string>
#include "NetworkEndpoint.h"

class HostResolver
{
public:

    /**
     * Resolve the given hostname into a network interface
     */
    static NetworkEndpoint resolve(std::string const& hostname);

    /**
     * Resolve the hostname for the given netork Interface
     */
    static std::string resolve(NetworkEndpoint const&networkEndpoint);

    /**
     * Check to see if the given hostname is the hostname for a local interface
     */
    static bool isLocalInterface(std::string const& hostname);

    /**
     * Check to see if the given hostname is a local endpoint
     */
    static bool isLocalInterface(NetworkEndpoint const& endpoint);
};


#endif //CASTER_NODE_HOSTRESOLVER_H

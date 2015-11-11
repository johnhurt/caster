//
// Created by Kevin Guthrie on 11/9/15.
//

#ifndef CASTER_NODE_HOSTRESOLVER_H
#define CASTER_NODE_HOSTRESOLVER_H

#include <string>
#include "NetworkInterface.h"

class HostResolver
{
public:

    /**
     * Resolve the given hostname into a network interface
     */
    static NetworkInterface resolve(std::string const& hostname);

    /**
     * Resolve the hostname for the given netork Interface
     */
    static std::string resolve(NetworkInterface const& networkInterface);
};


#endif //CASTER_NODE_HOSTRESOLVER_H

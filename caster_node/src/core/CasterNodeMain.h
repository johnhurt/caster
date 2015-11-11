//
// Created by Kevin Guthrie on 10/30/15.
//

#ifndef CASTER_NODE_CASTERNODEMAIN_H
#define CASTER_NODE_CASTERNODEMAIN_H

#include "boost/shared_ptr.hpp"
#include "CasterNodeConfig.h"
#include "CasterNode.h"

/**
 * EntryPoint to the
 */
class CasterNodeMain
{
protected:
    CasterNodeConfig _config;
    CasterNode _localNode;
    int _exitCode;

public:

    /** Simpler constructor for testing */
    CasterNodeMain(std::vector<std::string> const& args);
    CasterNodeMain(int argc, const char *const *const argv);

    CasterNodeConfig const& config() const;

    int const& exitCode() const;

    CasterNode const& localNode() const;
};


#endif //CASTER_NODE_CASTERNODEMAIN_H

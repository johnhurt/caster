//
// Created by Kevin Guthrie on 11/1/15.
//

#ifndef CASTER_NODE_CASTERNODECONFIG_H
#define CASTER_NODE_CASTERNODECONFIG_H

#include <vector>
#include <string>

/**
 * Configuration object for the current cluster node
 */
class CasterNodeConfig
{
private:

    int _chatterPort;
    std::vector<int> _ownedPorts;
    std::string _internalInterface;
    std::string _externalInterface;
    std::string _remoteInterface;

public:

    CasterNodeConfig();

    CasterNodeConfig(const int &chatterPort
            , std::vector<int> const& ownedPorts
            , std::string const& _internalInterface
            , std::string const& _externalInterface
            , std::string const& _remoteInterface);

    int const& chatterPort() const;
    std::vector<int> const& ownedPorts() const;
    std::string const& internalInterface() const;
    std::string const& externalInterface() const;
    std::string const& remoteInterface() const;
};


#endif //CASTER_NODE_CASTERNODECONFIG_H

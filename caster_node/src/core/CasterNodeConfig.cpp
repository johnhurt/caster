//
// Created by Kevin Guthrie on 11/1/15.
//

#include "CasterNodeConfig.h"

CasterNodeConfig::CasterNodeConfig()
{

}

CasterNodeConfig::CasterNodeConfig(const int &chatterPort
        , std::vector<int> const& ownedPorts
        , std::string const& _internalInterface
        , std::string const& _externalInterface
        , std::string const& _remoteInterface)
: _chatterPort(chatterPort)
, _ownedPorts(ownedPorts)
, _internalInterface(_internalInterface)
, _externalInterface(_externalInterface)
, _remoteInterface(_remoteInterface)
{
}

/**
 * Get the port configured for cluster chattering
 */
int const& CasterNodeConfig::chatterPort() const
{
    return _chatterPort;
}

/**
 * Get the set of ports available for general use
 */
std::vector<int> const& CasterNodeConfig::ownedPorts() const
{
    return _ownedPorts;
}

std::string const& CasterNodeConfig::internalInterface() const
{
    return _internalInterface;
}

std::string const& CasterNodeConfig::externalInterface() const
{
    return _externalInterface;
}

std::string const& CasterNodeConfig::remoteInterface() const
{
    return _remoteInterface;
}

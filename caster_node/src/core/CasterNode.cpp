//
// Created by Kevin Guthrie on 10/30/15.
//

#include "CasterNode.h"
#include "net/HostResolver.h"

using namespace boost;

CasterNode::CasterNode()
{

}

CasterNode::CasterNode(CasterNodeConfig const &config)
: _config(config)
{
    _internalInterface = HostResolver::resolve(config.internalInterface());
    _externalInterface = HostResolver::resolve(config.externalInterface());
    _remoteInterface = HostResolver::resolve(config.remoteInterface());
}

CasterNodeConfig const& CasterNode::config() const
{
    return _config;
}

NetworkInterface const& CasterNode::internalInterface() const
{
    return _internalInterface;
}

NetworkInterface const& CasterNode::externalInterface() const
{
    return _externalInterface;
}

NetworkInterface const& CasterNode::remoteInterface() const
{
    return _remoteInterface;
}

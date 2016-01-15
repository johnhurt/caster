//
// Created by Kevin Guthrie on 11/9/15.
//

#include <boost/algorithm/string/case_conv.hpp>
#include "HostResolver.h"

using namespace boost;

NetworkEndpoint HostResolver::resolve(std::string const& hostname)
{
    NetworkEndpoint bestResult;

    bool resultFound(false);
    bool isIpv6(false);

    asio::io_service io_service;
    asio::ip::tcp::resolver resolver(io_service);
    asio::ip::tcp::resolver::query query(hostname,"");

    for(asio::ip::tcp::resolver::iterator i = resolver.resolve(query)
            ; i != asio::ip::tcp::resolver::iterator()
            ; ++i)
    {
        NetworkEndpoint currResult = i->endpoint();

        if (!resultFound)
        {
            bestResult = currResult;
            resultFound = true;
        }

        if (!isIpv6 && currResult.address().is_v6()) {
            bestResult = currResult;
            isIpv6 = true;
        }
    }

    return bestResult;
}

std::string HostResolver::resolve(NetworkEndpoint const &networkEndpoint)
{
    asio::io_service io_service;
    asio::ip::tcp::resolver resolver(io_service);

    for(asio::ip::tcp::resolver::iterator i = resolver.resolve(networkEndpoint)
            ; i != asio::ip::tcp::resolver::iterator(); )
    {
        return boost::algorithm::to_lower_copy(i->host_name());
    }

    return "";
}

bool HostResolver::isLocalInterface(const std::string &hostname)
{
    return isLocalInterface(resolve(hostname));
}

bool HostResolver::isLocalInterface(NetworkEndpoint const &endpoint)
{
    asio::io_service io_service;

    NetworkEndpoint ep(endpoint.address(), 55555);

    try
    {
        asio::ip::tcp::acceptor acceptor(io_service, ep);
    }
    catch(system::system_error & e) {
        return false;
    }

    return true;
}

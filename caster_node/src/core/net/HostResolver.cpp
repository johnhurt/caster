//
// Created by Kevin Guthrie on 11/9/15.
//

#include <boost/algorithm/string/case_conv.hpp>
#include "HostResolver.h"

using namespace boost;

NetworkInterface HostResolver::resolve(std::string const& hostname)
{
    NetworkInterface bestResult;

    bool resultFound(false);
    bool isIpv6(false);

    asio::io_service io_service;
    asio::ip::tcp::resolver resolver(io_service);
    asio::ip::tcp::resolver::query query(hostname,"");

    for(asio::ip::tcp::resolver::iterator i = resolver.resolve(query);
            i != asio::ip::tcp::resolver::iterator();
            ++i)
    {
        NetworkInterface currResult = i->endpoint();

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

std::string HostResolver::resolve(NetworkInterface const &networkInterface)
{
    asio::io_service io_service;
    asio::ip::tcp::resolver resolver(io_service);

    for(asio::ip::tcp::resolver::iterator i = resolver.resolve(networkInterface)
            ; i != asio::ip::tcp::resolver::iterator()
            ; ++i)
    {
        return boost::algorithm::to_lower_copy(i->host_name());
    }

    return "";
}


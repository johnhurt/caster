//
// Created by Kevin Guthrie on 11/12/15.
//

#include <core/net/HostResolver.h>
#include "ChatterServer.h"
#include <utility>

using namespace boost::asio;

ChatterServer::ChatterServer(CasterNodeConfig &currentNodeConfig
        , boost::asio::io_service& ioService)
: ioService(ioService)
, _currentNodeConfig(currentNodeConfig)
, acceptor(ioService, NetworkEndpoint(
        HostResolver::resolve(currentNodeConfig.internal_interface()).address()
        , currentNodeConfig.chatter_port()))
, socket(ioService)
{

}

/**
 * This is mostly coppied from
 * http://www.boost.org/doc/libs/1_57_0/doc/html/boost_asio/example/cpp11/http/server/server.cpp
 */
void ChatterServer::doAccept()
{
    acceptor.async_accept(socket, [this](boost::system::error_code error)
    {
        // Check whether the server was stopped by a signal before this
        // completion handler had a chance to run.
        if (!acceptor.is_open())
        {
            return;
        }

        if (!error)
        {
            std::make_shared<ChatterServerClient>(std::move(socket))->start();
        }

        doAccept();
    });
}

void ChatterServer::start()
{
    doAccept();
}

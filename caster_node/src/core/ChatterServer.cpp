//
// Created by Kevin Guthrie on 11/12/15.
//

#include <core/net/HostResolver.h>
#include "ChatterServer.h"

using namespace boost::asio;

ChatterServer::ChatterServer(boost::asio::io_service& ioService
        , CasterNode& localNode)
: ioService(ioService)
, _localNode(localNode)
, acceptor(ioService, NetworkEndpoint(
        HostResolver::resolve(localNode.config().internal_interface()).address()
        , localNode.config().chatter_port()))
, socket(ioService)
, raftNode(ioService, localNode.config().seed())
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
            std::make_shared<ChatterServerClient>(std::move(socket)
                    , raftNode)->start();
        }

        doAccept();
    });
}

void ChatterServer::start()
{
    // If this is the seed node, the list of nodes in the cluster is just this
    // node.   Ask the existing cluster to join, and get the new set of nodes
    if (!_localNode.config().seed()) {

    }

    raftNode.start();
    doAccept();
}

void ChatterServer::stop()
{
    acceptor.cancel();
    raftNode.stop();
}

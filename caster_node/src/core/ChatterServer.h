//
// Created by Kevin Guthrie on 11/12/15.
//

#ifndef CASTER_NODE_CHATTERSERVER_H
#define CASTER_NODE_CHATTERSERVER_H

#include <boost/thread.hpp>
#include <memory>
#include "CasterNode.pb.h"
#include "ChatterServerClient.h"

/**
 * Network socket acceptor for for the Chatter service
 */
class ChatterServer
{
private:
    boost::asio::io_service& ioService;
    boost::asio::ip::tcp::acceptor acceptor;
    boost::asio::ip::tcp::socket socket;
    CasterNodeConfig& _currentNodeConfig;


public:
    ChatterServer(CasterNodeConfig& currentNodeConfig
            , boost::asio::io_service& ioService);

    /**
     * Start the caster chatter server
     */
    void start();

private:

    void doAccept();


};


#endif //CASTER_NODE_CHATTERSERVER_H

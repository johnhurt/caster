//
// Created by Kevin Guthrie on 11/12/15.
//

#ifndef CASTER_NODE_CHATTERSERVER_H
#define CASTER_NODE_CHATTERSERVER_H

#include <boost/thread.hpp>
#include <memory>
#include <core/raft/RaftNode.h>
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
    CasterNode _localNode;
    RaftNode raftNode;


public:
    ChatterServer(boost::asio::io_service& ioService
            , CasterNode& localNode);

    /**
     * Start the caster chatter server
     */
    void start();

    /**
     * Stop the caster chatter server
     */
    void stop();

private:

    void doAccept();


};


#endif //CASTER_NODE_CHATTERSERVER_H

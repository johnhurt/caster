//
// Created by Kevin Guthrie on 11/13/15.
//

#ifndef CASTER_NODE_CHATTERSERVERCLIENT_H
#define CASTER_NODE_CHATTERSERVERCLIENT_H

#include <boost/asio.hpp>
#include <core/rpc/DescribeHandler.h>
#include "TypedMessage.hpp"
#include <memory>

class ChatterServerClient
: public std::enable_shared_from_this<ChatterServerClient>
{
private:

    boost::asio::ip::tcp::socket _socket;
    MessageBuffer headerBuffer;
    MessageBuffer messageBuffer;
    int requestType;
    int requestSize;

    DescribeHandler describeHandler;

public:

    ChatterServerClient(boost::asio::ip::tcp::socket socket);
    virtual ~ChatterServerClient();

    boost::asio::ip::tcp::socket& socket();

    void start();

protected:

    void handleRequestHeader();

    void handleRequest();

};


#endif //CASTER_NODE_CHATTERSERVERCLIENT_H

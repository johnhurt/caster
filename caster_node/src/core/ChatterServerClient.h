//
// Created by Kevin Guthrie on 11/13/15.
//

#ifndef CASTER_NODE_CHATTERSERVERCLIENT_H
#define CASTER_NODE_CHATTERSERVERCLIENT_H

#include <boost/asio.hpp>
#include <core/rpc/DescribeHandler.h>
#include <core/rpc/AppendRaftEntriesHandler.h>
#include <core/rpc/RequestRaftVoteHandler.h>
#include "TypedMessage.hpp"
#include <memory>

#define DECLARE_HANDLER_FOR(T) T ## Handler handlerFor ## T;

#define WRITE_HANDLER_CASE_FOR(T) \
        case T: { \
            T ## Request request; \
            request.ParseFromArray(&messageBuffer[0], requestSize); \
            T ## RpcResponse response \
                    = handlerFor ## T.handleRequest(request); \
            responseBuffer = response.buffer(); \
            break; \
        }

#define CONSTRUCT_HANDLER_FOR(T, ...) handlerFor ## T(__VA_ARGS__)

class ChatterServerClient
: public std::enable_shared_from_this<ChatterServerClient>
{
private:

    boost::asio::ip::tcp::socket _socket;
    MessageBuffer headerBuffer;
    MessageBuffer messageBuffer;
    int requestType;
    int requestSize;

    DECLARE_HANDLER_FOR(Describe);
    DECLARE_HANDLER_FOR(AppendRaftEntries);
    DECLARE_HANDLER_FOR(RequestRaftVote);

public:

    ChatterServerClient(boost::asio::ip::tcp::socket socket
            , RaftNode& raftNode);
    virtual ~ChatterServerClient();

    boost::asio::ip::tcp::socket& socket();

    void start();

protected:

    void handleRequestHeader();

    void handleRequestBody();

    void handleRequest(MessageBuffer& responseBuffer);
};


#endif //CASTER_NODE_CHATTERSERVERCLIENT_H

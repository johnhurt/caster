//
// Created by Kevin Guthrie on 11/13/15.
//

#include "ChatterServerClient.h"
#include <core/rpc/ChatterRpcMessages.h>
#include <boost/bind.hpp>

ChatterServerClient::ChatterServerClient(
        boost::asio::ip::tcp::socket socket
        , RaftNode& raftNode)
: _socket(std::move(socket))
, headerBuffer(TypedMessageUtil::HEADER_LENGTH)
, CONSTRUCT_HANDLER_FOR(Describe)
, CONSTRUCT_HANDLER_FOR(AppendRaftEntries, raftNode)
, CONSTRUCT_HANDLER_FOR(RequestRaftVote, raftNode)
{

}

boost::asio::ip::tcp::socket& ChatterServerClient::socket()
{
    return _socket;
}

void ChatterServerClient::start()
{
    auto self(shared_from_this());

    headerBuffer.resize(TypedMessageUtil::HEADER_LENGTH);

    _socket.async_read_some(
            boost::asio::buffer(&headerBuffer[0]
                    , TypedMessageUtil::HEADER_LENGTH)
            , [this, self](boost::system::error_code err, std::size_t length) {
                if (!err) {
                    handleRequestHeader();
                }
            });
}

void ChatterServerClient::handleRequestHeader()
{
    auto self(shared_from_this());

    requestType = TypedMessageUtil::getMessageType(headerBuffer);
    int requestSubtype = TypedMessageUtil::getMessageSubtype(headerBuffer);
    requestSize = TypedMessageUtil::getMessageSize(headerBuffer);

    if (requestSubtype != MessageSubtype::Request) {
        // Do something :P
    }

    messageBuffer.resize(requestSize);

    _socket.async_read_some(
            boost::asio::buffer(&messageBuffer[0], requestSize)
            , [this, self](boost::system::error_code err, std::size_t length) {
                if (!err) {
                    handleRequestBody();
                }
            });

}

void ChatterServerClient::handleRequestBody()
{
    MessageBuffer responseBuffer;

    handleRequest(responseBuffer);

    boost::asio::write(_socket, boost::asio::buffer(responseBuffer));
}


void ChatterServerClient::handleRequest(MessageBuffer &responseBuffer)
{

    switch(requestType) {
        WRITE_HANDLER_CASE_FOR(Describe)
        WRITE_HANDLER_CASE_FOR(AppendRaftEntries)
        WRITE_HANDLER_CASE_FOR(RequestRaftVote)
    }

}

ChatterServerClient::~ChatterServerClient()
{
}

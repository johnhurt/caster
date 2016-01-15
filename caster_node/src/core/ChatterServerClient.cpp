//
// Created by Kevin Guthrie on 11/13/15.
//

#include "ChatterServerClient.h"
#include <core/rpc/ChatterRpcMessages.h>
#include <boost/bind.hpp>

ChatterServerClient::ChatterServerClient(
        boost::asio::ip::tcp::socket socket)
: _socket(std::move(socket))
, headerBuffer(TypedMessageUtil::HEADER_LENGTH)
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
                    handleRequest();
                }
            });

}

void ChatterServerClient::handleRequest()
{

    MessageBuffer responseBuffer;

    switch(requestType) {
    case Describe: {
        DescribeRequest request;
        request.ParseFromArray(&messageBuffer[0], requestSize);
        DescribeRpcResponse response = describeHandler.handleRequest(request);
        responseBuffer = response.buffer();
        break;
    }
    }

    boost::asio::write(_socket, boost::asio::buffer(responseBuffer));
}

ChatterServerClient::~ChatterServerClient()
{
    int j = 0;
}

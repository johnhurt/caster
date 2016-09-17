#define BOOST_TEST_NO_MAIN DescribeRpcTest

#include <boost/test/unit_test.hpp>
#include <core/rpc/ChatterRpcMessages.h>
#include <boost/asio.hpp>
#include <core/CasterNodeOptionParser.h>
#include <core/CasterNodeMain.h>

#include "TestUtils.hpp"

/**
 * This test class is to make sure the rpc service can listen, deserialize,
 * answer, serialize, and transmit
 */

BOOST_FIXTURE_TEST_SUITE( describeRpcTests, BasicOptionsFixture )

    BOOST_AUTO_TEST_CASE( testDescribeRpcBasic )
    {
        CasterNodeConfig config = CasterNodeOptionParser::parseCommandLine(
                getBasicOptions({}));

        CasterNodeMain main(config);

        main.start();

        DescribeRequest request;

        request.set_verbose(true);

        DescribeRpcRequest rpcRequest(request);

        boost::asio::io_service ioService;
        boost::asio::ip::tcp::socket socket(ioService);
        boost::asio::ip::tcp::resolver resolver(ioService);
        boost::asio::connect(socket, resolver.resolve({ config.internal_interface()
                , std::to_string(config.chatter_port())}));

        boost::asio::write(socket, boost::asio::buffer(&rpcRequest.buffer()[0]
                , rpcRequest.buffer().size()));

        MessageBuffer headerBuffer;
        headerBuffer.resize(TypedMessageUtil::HEADER_LENGTH);

        boost::asio::read(socket, boost::asio::buffer(&headerBuffer[0]
                , TypedMessageUtil::HEADER_LENGTH));

        int responseType = TypedMessageUtil::getMessageType(headerBuffer);
        int responseSubtype = TypedMessageUtil::getMessageSubtype(headerBuffer);
        int responseSize = TypedMessageUtil::getMessageSize(headerBuffer);

        BOOST_CHECK_EQUAL(MessageType::Describe, responseType);
        BOOST_CHECK_EQUAL(MessageSubtype::Response, responseSubtype);

        MessageBuffer responseBody;
        responseBody.resize(responseSize);

        boost::asio::read(socket, boost::asio::buffer(&responseBody[0]
                , responseSize));

        DescribeResponse response;
        response.ParseFromArray(&responseBody[0], responseSize);

        BOOST_CHECK_EQUAL(response.description()
                , std::string("Everything is fine"));
        BOOST_CHECK(response.verbose());

        main.stop();
    }

BOOST_AUTO_TEST_SUITE_END()

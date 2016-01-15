#define BOOST_TEST_NO_MAIN DescribeRpcTest

#include <boost/test/unit_test.hpp>
#include <core/rpc/ChatterRpcMessages.h>
#include <boost/asio.hpp>
#include <core/CasterNodeOptionParser.h>
#include <core/CasterNodeMain.h>
#include <core/net/NetworkEndpoint.h>

/**
 * This test class is to make sure the rpc service can listen, deserialize,
 * answer, serialize, and transmit
 */


/**
 * Get the basic options for caster_node along with additional options specific
 * for the test
 */
std::vector<std::string> getBasicCasterOptions(
        std::vector<std::string> extraOpts) {

    std::vector<std::string> result = { "-s", "-i", "localhost" };

    result.insert(result.end(), extraOpts.begin(), extraOpts.end());

    return result;
}

BOOST_AUTO_TEST_CASE( testDescribeRpcBasic )
{
    CasterNodeConfig config = CasterNodeOptionParser::parseCommandLine(
            getBasicCasterOptions({}));

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

    int requestType = TypedMessageUtil::getMessageType(headerBuffer);
    int requestSubtype = TypedMessageUtil::getMessageSubtype(headerBuffer);
    int responseSize = TypedMessageUtil::getMessageSize(headerBuffer);

    MessageBuffer responseBody;
    responseBody.resize(responseSize);

    boost::asio::read(socket, boost::asio::buffer(&responseBody[0]
            , responseSize));

    DescribeResponse response;
    response.ParseFromArray(&responseBody[0], responseSize);

    BOOST_CHECK_EQUAL(response.description(), std::string("Everything is fine"));
    BOOST_CHECK(response.verbose());
    main.stop();
}
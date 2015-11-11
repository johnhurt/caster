#define BOOST_TEST_NO_MAIN CurrentNodeTest
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <core/net/HostResolver.h>
#include <core/CasterNodeMain.h>

/**
* Get the basic options for caster_node along with additional options specific
* for the test
*/
std::vector<std::string> getBasicOptionsNoNet(
        std::vector<std::string> extraOpts) {

    std::vector<std::string> result = { "-s" };

    result.insert(result.end(), extraOpts.begin(), extraOpts.end());

    return result;
}

BOOST_AUTO_TEST_CASE( testLocalNodeNetwork )
{
    std::string currHostname = boost::algorithm::to_lower_copy(
            boost::asio::ip::host_name());

    {
        CasterNodeMain casterMain(getBasicOptionsNoNet({"-i", "localhost"}));
        CasterNode node = casterMain.localNode();

        BOOST_CHECK(node.internalInterface().address().is_loopback());
        BOOST_CHECK(node.externalInterface().address().is_loopback());
        BOOST_CHECK(node.remoteInterface().address().is_loopback());
    }

    {
        CasterNodeMain casterMain(
                getBasicOptionsNoNet({"-i", currHostname.c_str()}));
        CasterNode node = casterMain.localNode();

        BOOST_CHECK(!node.internalInterface().address().is_loopback());
        BOOST_CHECK(!node.externalInterface().address().is_loopback());
        BOOST_CHECK(!node.remoteInterface().address().is_loopback());

        BOOST_CHECK_EQUAL(
                HostResolver::resolve(node.internalInterface())
                , currHostname);
    }

}
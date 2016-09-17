#define BOOST_TEST_NO_MAIN ProgramOptionsTest
#include <boost/test/unit_test.hpp>

#include <core/net/HostResolver.h>
#include <core/CasterNodeMain.h>
#include <core/CasterNodeOptionParser.h>

#include "TestUtils.hpp"

BOOST_FIXTURE_TEST_SUITE(programOptionsTests, BasicOptionsFixture)


    BOOST_AUTO_TEST_CASE(testMaxDefaultOptions)
    {
        CasterNodeConfig config = CasterNodeOptionParser::parseCommandLine(
                getBasicOptions({}));

        BOOST_CHECK_EQUAL(config.chatter_port(), 3130);

        int ownedPorts[] = {
                3131
                , 3132
                , 3133
                , 3134
                , 3135
                , 3136
                , 3137
                , 3138
                , 3139
                , 3140
        };

        for (int i = 0; i < 10; i++)
        {
            BOOST_CHECK_EQUAL(
                    config.owned_port(i), ownedPorts[i]);
        }
    }

    BOOST_AUTO_TEST_CASE(testSetChatterPort)
    {
        CasterNodeConfig config = CasterNodeOptionParser::parseCommandLine(
                getBasicOptions({"--chatter-port", "2041"}));
        BOOST_CHECK_EQUAL(config.chatter_port(), 2041);

        int ownedPorts[] = {
                3131
                , 3132
                , 3133
                , 3134
                , 3135
                , 3136
                , 3137
                , 3138
                , 3139
                , 3140
        };

        for (int i = 0; i < 10; i++)
        {
            BOOST_CHECK_EQUAL(
                    config.owned_port(i), ownedPorts[i]);
        }
    }

    BOOST_AUTO_TEST_CASE(testSetOwnedPortsSimple)
    {
        CasterNodeConfig config = CasterNodeOptionParser::parseCommandLine(
                getBasicOptions({"--owned-ports", "2042-2052"}));
        BOOST_CHECK_EQUAL(config.chatter_port(), 3130);

        int ownedPorts[] = {
                2042
                , 2043
                , 2044
                , 2045
                , 2046
                , 2047
                , 2048
                , 2049
                , 2050
                , 2051
                , 2052
        };

        for (int i = 0; i < 10; i++)
        {
            BOOST_CHECK_EQUAL(
                    config.owned_port(i), ownedPorts[i]);
        }
    }

    BOOST_AUTO_TEST_CASE(testSetOwnedPortsShortOption)
    {
        CasterNodeConfig config = CasterNodeOptionParser::parseCommandLine(
                getBasicOptions({"-o", "2042-2052"}));
        BOOST_CHECK_EQUAL(config.chatter_port(), 3130);

        int ownedPorts[] = {
                2042
                , 2043
                , 2044
                , 2045
                , 2046
                , 2047
                , 2048
                , 2049
                , 2050
                , 2051
                , 2052
        };

        for (int i = 0; i < 11; i++)
        {
            BOOST_CHECK_EQUAL(
                    config.owned_port(i), ownedPorts[i]);
        }
    }

    BOOST_AUTO_TEST_CASE(testSetChatterPortAndComplexOwnedPorts)
    {
        CasterNodeConfig config = CasterNodeOptionParser::parseCommandLine(
                getBasicOptions({"-c", "2041", "-o"
                        , "3131-3134,3136-3136,3138-3142"}));
        BOOST_CHECK_EQUAL(config.chatter_port(), 2041);

        int ownedPorts[] = {
                3131
                , 3132
                , 3133
                , 3134
                , 3136
                , 3138
                , 3139
                , 3140
                , 3141
                , 3142
        };

        for (int i = 0; i < 10; i++)
        {
            BOOST_CHECK_EQUAL(
                    config.owned_port(i), ownedPorts[i]);
        }
    }

    BOOST_AUTO_TEST_CASE(testUseLocalInterfacePropagation)
    {
        CasterNodeConfig config = CasterNodeOptionParser::parseCommandLine(
                getBasicOptionsNoNet({"-i", "localhost"}));

        BOOST_CHECK(HostResolver::resolve(
                config.internal_interface()).address().is_loopback());
        BOOST_CHECK(HostResolver::resolve(
                config.external_interface()).address().is_loopback());
        BOOST_CHECK(HostResolver::resolve(
                config.remote_interface()).address().is_loopback());
    }

    BOOST_AUTO_TEST_CASE( testUseCurrentHostnamePropagation ) {
        CasterNodeConfig config = CasterNodeOptionParser::parseCommandLine(
                getBasicOptionsNoNet({"-i", currHostname.c_str()}));

        BOOST_CHECK(!HostResolver::resolve(
                config.internal_interface()).address().is_loopback());
        BOOST_CHECK(!HostResolver::resolve(
                config.external_interface()).address().is_loopback());
        BOOST_CHECK(!HostResolver::resolve(
                config.remote_interface()).address().is_loopback());

        BOOST_CHECK_EQUAL(HostResolver::resolve(HostResolver::resolve(
                config.internal_interface())), currHostname);
        BOOST_CHECK_EQUAL(HostResolver::resolve(HostResolver::resolve(
                config.external_interface())), currHostname);
        BOOST_CHECK_EQUAL(HostResolver::resolve(HostResolver::resolve(
                config.remote_interface())), currHostname);
    }

    BOOST_AUTO_TEST_CASE( testUseHostnameForInternalAndLocalForExternal ) {
        CasterNodeConfig config = CasterNodeOptionParser::parseCommandLine(
                getBasicOptionsNoNet({
                        "-i", currHostname.c_str()
                        , "-e", "localhost"
                }));

        BOOST_CHECK(!HostResolver::resolve(
                config.internal_interface()).address().is_loopback());
        BOOST_CHECK(HostResolver::resolve(
                config.external_interface()).address().is_loopback());
        BOOST_CHECK(!HostResolver::resolve(
                config.remote_interface()).address().is_loopback());

        BOOST_CHECK_EQUAL(HostResolver::resolve(HostResolver::resolve(
                config.internal_interface())), currHostname);
    }

    BOOST_AUTO_TEST_CASE( testUseLocalForInternalAndHostnameForRemote ) {
        CasterNodeConfig config = CasterNodeOptionParser::parseCommandLine(
                getBasicOptionsNoNet({
                        "-i", "localhost"
                        , "-r", currHostname.c_str()
                }));

        BOOST_CHECK(HostResolver::resolve(
                config.internal_interface()).address().is_loopback());
        BOOST_CHECK(HostResolver::resolve(
                config.external_interface()).address().is_loopback());
        BOOST_CHECK(!HostResolver::resolve(
                config.remote_interface()).address().is_loopback());

        BOOST_CHECK_EQUAL(HostResolver::resolve(HostResolver::resolve(
                config.remote_interface())), currHostname);
    }

    BOOST_AUTO_TEST_CASE( testInvalidInterface ) {
        try
        {
            CasterNodeConfig config = CasterNodeOptionParser::parseCommandLine(
                    getBasicOptionsNoNet({"-i", "google.com"}));
            BOOST_FAIL("Trying to listen on hostname google.com should cause "
                    "an exception");
        }
        catch(std::exception& ex){}
    }


BOOST_AUTO_TEST_SUITE_END()
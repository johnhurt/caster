#define BOOST_TEST_NO_MAIN ProgramOptionsTest
#include <boost/test/unit_test.hpp>

#include "core/CasterNodeMain.h"

/**
 * Get the basic options for caster_node along with additional options specific
 * for the test
 */
std::vector<std::string> getBasicOptions(std::vector<std::string> extraOpts) {
    std::vector<std::string> result = { "-s", "-i", "localhost" };

    result.insert(result.end(), extraOpts.begin(), extraOpts.end());

    return result;
}

BOOST_AUTO_TEST_CASE( testPortUsageParsing )
{

    {
        CasterNodeMain casterMain(getBasicOptions({}));
        BOOST_CHECK_EQUAL(casterMain.config().chatterPort(), 3130);

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

        for (int i = 0; i < 10; i++) {
            BOOST_CHECK_EQUAL(
                    casterMain.config().ownedPorts()[i], ownedPorts[i]);
        }
    }

    {
        CasterNodeMain casterMain(getBasicOptions({"--chatter-port", "2041"}));
        BOOST_CHECK_EQUAL(casterMain.config().chatterPort(), 2041);

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

        for (int i = 0; i < 10; i++) {
            BOOST_CHECK_EQUAL(
                    casterMain.config().ownedPorts()[i], ownedPorts[i]);
        }
    }

    {
        CasterNodeMain casterMain(
                getBasicOptions({"--owned-ports", "2042-2052"}));
        BOOST_CHECK_EQUAL(casterMain.config().chatterPort(), 3130);

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

        for (int i = 0; i < 10; i++) {
            BOOST_CHECK_EQUAL(
                    casterMain.config().ownedPorts()[i], ownedPorts[i]);
        }
    }


    {
        CasterNodeMain casterMain(getBasicOptions({"-o", "2042-2052"}));
        BOOST_CHECK_EQUAL(casterMain.config().chatterPort(), 3130);

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

        for (int i = 0; i < 11; i++) {
            BOOST_CHECK_EQUAL(
                    casterMain.config().ownedPorts()[i], ownedPorts[i]);
        }
    }

    {
        CasterNodeMain casterMain(getBasicOptions({"-c", "2041", "-o"
                , "3131-3134,3136-3136,3138-3142"}));
        BOOST_CHECK_EQUAL(casterMain.config().chatterPort(), 2041);

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

        for (int i = 0; i < 10; i++) {
            BOOST_CHECK_EQUAL(
                    casterMain.config().ownedPorts()[i], ownedPorts[i]);
        }
    }

}

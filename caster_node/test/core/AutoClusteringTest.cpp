//
// Created by Kevin Guthrie on 1/18/16.
//

#define BOOST_TEST_NO_MAIN AutoClusteringTest

#include <boost/test/unit_test.hpp>
#include <CasterNodeConfig.pb.h>
#include <core/CasterNodeOptionParser.h>
#include <core/CasterNodeMain.h>

#include "TestUtils.hpp"

/**
 * This is a test to get the basic raft clustering working
 */

BOOST_FIXTURE_TEST_SUITE( testAutoClustering, BasicOptionsFixture )

    BOOST_AUTO_TEST_CASE( test2NodeCluster )
    {

        CasterNodeConfig seedConfig = CasterNodeOptionParser::parseCommandLine(
                getBasicOptions({}));

    //    CasterNodeConfig node2Config = CasterNodeOptionParser::parseCommandLine(
    //            getBasicOptions({ "-c", "3230", "-o", "3231-3240"
    //                    , "-P", "localhost"}));

        CasterNodeMain seedNode(seedConfig);
    //    CasterNodeMain node1(node2Config);

        seedNode.start();

        sleep(10);

        //node1.start();

        seedNode.stop();
    }


BOOST_AUTO_TEST_SUITE_END()
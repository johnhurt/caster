//
// Created by Kevin Guthrie on 1/22/16.
//

#ifndef CASTER_NODE_TESTUTILS_H
#define CASTER_NODE_TESTUTILS_H

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem.hpp>

struct BasicOptionsFixture {
    boost::filesystem::path casterDir;
    std::vector<std::string> basicOptions;
    std::vector<std::string> basicNoNetOptions;
    std::vector<std::string> basicNonSeedOptions;
    std::string currHostname;

    BasicOptionsFixture()
            : casterDir(boost::filesystem::temp_directory_path()
            .append(boost::filesystem::unique_path().string()))
            , basicOptions({ "-s", "-i", "localhost", "-d" })
            , basicNonSeedOptions({ "-i", "localhost", "-d" })
            , basicNoNetOptions({ "-s", "-d" })
            , currHostname(boost::algorithm::to_lower_copy(
                    boost::asio::ip::host_name()))
    {
        basicOptions.push_back(casterDir.string());
        basicNonSeedOptions.push_back(casterDir.string());
        basicNoNetOptions.push_back(casterDir.string());
    }

    ~BasicOptionsFixture() { boost::filesystem::remove_all(casterDir);}

    /**
     * Get the basic options for caster_node along with additional options specific
     * for the test
     */
    std::vector<std::string> getBasicOptions(
            std::vector<std::string> const& extraOptions) {

        std::vector<std::string> result(basicOptions);

        result.insert(result.end(), extraOptions.begin(), extraOptions.end());

        return result;
    }

    /**
     * Get the basic options for caster_node along with additional options
     * specific for the test
     */
    std::vector<std::string> getBasicOptionsNoNet(
            std::vector<std::string> extraOpts) {

        std::vector<std::string> result(basicNoNetOptions);

        result.insert(result.end(), extraOpts.begin(), extraOpts.end());

        return result;
    }
};


#endif //CASTER_NODE_TESTUTILS_H

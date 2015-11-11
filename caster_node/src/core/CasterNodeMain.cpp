//
// Created by Kevin Guthrie on 10/30/15.
//

#include "CasterNodeMain.h"
#include "CasterNode.h"

#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
#include "boost/foreach.hpp"

#include <iostream>

using namespace boost::program_options;


/**
 * Convert the string of ports and ranges into a list of explicit integers.
 * The string should be a comma separated list where each entry is of a form
 * like 5234 or 5143-5147.  The multi-port definition is inclusive on both sides
 */
void parsePortList(std::string const& portListString
        , std::vector<int>& targetList) {

    std::vector<std::string> parts;
    boost::split(parts, portListString, boost::is_any_of(","));

    BOOST_FOREACH(std::string& part, parts) {
        std::vector<std::string> bounds;

        if (part.empty()) {
            throw error(std::string("Invalid value for owned ports")
                        + portListString);
        }

        boost::split(bounds, part, boost::is_any_of("-"));

        if (bounds.empty() || bounds.size() > 2) {
            throw error(std::string("Invalid value for owned ports")
                        + portListString);
        }

        int lowerBound = std::stoi(bounds[0]);
        int upperBound = lowerBound;

        if (bounds.size() > 1) {
            upperBound = std::stoi(bounds[1]);

            if (upperBound < lowerBound) {
                throw error(std::string("Invalid port range (Descending): ")
                            + part);
            }
        }

        for (int currPort = lowerBound; currPort <= upperBound; currPort++) {
            targetList.push_back(currPort);
        }
    }

}

CasterNodeMain::CasterNodeMain(int argc, const char *const *const argv)
{
}

CasterNodeMain::CasterNodeMain(std::vector<std::string> const& args)
: _exitCode(0)
{
    int argc = args.size() + 1;
    const char *argv[argc];

    for (int i = 0; i < argc; i++)
    {
        argv[i + 1] = args[i].c_str();
    }

    argv[0] = "caster_node";

    options_description desc("Runtime Arguments");

    bool help = false; // If this is set, this method will print and exit
    bool seed = false;
    bool remoteSeed = false;
    int chatterPort(3130);
    std::string ownedPortsStr("3131-3140");
    std::string peerAddress;

    bool localMode = false; // If set, the node will only listen on localhost
    std::string internalInterface; // interface for intra-cluster communication
    std::string externalInterface; // interface for extra-cluster communication
    std::string remoteInterface;   // interface for inter-cluster communication

    std::vector<int> ownedPorts;

    // Describe the set of options that can be used for configuring this node
    desc.add_options()
            ("help,h", "Prints this info :)")
            ("seed,s", "Indicates this node is the seed of a cluster")
            ("remote-seed,R", "Indicates this node is the seed cluster bonded "
                    "to a remote cluster")
            ("chatter-port,c", value<int>(&chatterPort)
                    , "Port used for clustering chatter (default 3130)")
            ("owned-ports,o", value<std::string>(&ownedPortsStr)
                    , "Set of ports for general use (default 3131-3140)")
            ("peer-address,P", value<std::string>(&peerAddress)
                    , "Any other host(:chatter_port) in the cluster to be "
                     "joined either as a local or remote peer")
            ("internal-interface,i"
                    , value<std::string>(&internalInterface)->required()
                    , "Network Interface to use for communication among "
                     "peer nodes")
            ("external-interface,e"
                    , value<std::string>(&externalInterface)
                    , "Network Interface to use for communication between "
                     "this node and external tools or applications.  Defaults "
                     "to the internal interface")
            ("remote-interface,r"
                    , value<std::string>(&remoteInterface)
                    , "Network Interface to use for communication between "
                     "this node and remote peers.  Defaults to the internal "
                     "interface");

    variables_map opts;

    if (argc == 1) {
        help = true;
    }

    try
    {
        store(parse_command_line(argc, argv, desc), opts);
        notify(opts);

        int i = 0;

        if (opts.count("help")) {
            help = true;
        }
        if (opts.count("seed")) {
            seed = true;
        }
        if (opts.count("peer-address")) {
            if (seed) {
                throw error("A node a with peer address cannot be a seed");
            }
        }
        if (opts.count("remote-seed")) {
            if (seed) {
                throw error("A node cannot be a seed and remote seed at once");
            }
            remoteSeed = true;
        }

        if (!seed && peerAddress.empty() && !help) {
            throw error("A node has to have a peer address or be a seed");
        }

        parsePortList(ownedPortsStr, ownedPorts);

        if (ownedPorts.size() < 9) {
            throw error("Owned port list is too small.  "
                                "Should include 9-19 ports");
        }

        if (ownedPorts.size() > 19) {
            throw error("Owned port list is too larger.  "
                                "Should include 9-19 ports");
        }

        if (externalInterface.empty()) {
            externalInterface = internalInterface;
        }

        if (remoteInterface.empty()) {
            remoteInterface = internalInterface;
        }

    }
    catch(error& e) { // This catches parsing errors
        if (!help)
        {
            std::cerr << "Failed to Parse Command Line: "
            << e.what()
            << std::endl
            << std::endl;

            desc.print(std::cerr);

            std::cerr << std::endl;

            _exitCode = 1;
            return;
        }
    }

    if (help) {
        desc.print(std::cout);

        std::cout << std::endl;
    }

    _config = CasterNodeConfig(chatterPort, ownedPorts, internalInterface
            , externalInterface, remoteInterface);
    _localNode = CasterNode(_config);
}

/** Getter for config **/
CasterNodeConfig const &CasterNodeMain::config() const
{
    return _config;
}

int const & CasterNodeMain::exitCode() const
{
    return _exitCode;
}

CasterNode const & CasterNodeMain::localNode() const
{
    return _localNode;
}

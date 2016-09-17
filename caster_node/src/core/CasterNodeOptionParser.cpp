//
// Created by Kevin Guthrie on 12/14/15.
//

#include "CasterNodeOptionParser.h"
#include <boost/filesystem.hpp>
#include <fstream>

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
            throw po::error(std::string("Invalid value for owned ports")
                    + portListString);
        }

        boost::split(bounds, part, boost::is_any_of("-"));

        if (bounds.empty() || bounds.size() > 2) {
            throw po::error(std::string("Invalid value for owned ports")
                    + portListString);
        }

        int lowerBound = std::stoi(bounds[0]);
        int upperBound = lowerBound;

        if (bounds.size() > 1) {
            upperBound = std::stoi(bounds[1]);

            if (upperBound < lowerBound) {
                throw po::error(std::string("Invalid port range (Descending): ")
                        + part);
            }
        }

        for (int currPort = lowerBound; currPort <= upperBound; currPort++) {
            targetList.push_back(currPort);
        }
    }
}

/**
 * Parse and validate the configuration given as storage directories.  This
 * will split up the string on comma and ensure that each directory configured
 * exists (or can be created) and can be written to
 */
void parseAndValidateStorageDirectories(
        std::string const& storageDirectoriesList
        , std::vector<std::string>& target)
{
    std::vector<std::string> parts;
    boost::split(parts, storageDirectoriesList, boost::is_any_of(","));

    BOOST_FOREACH(std::string& part, parts)
    {
        boost::filesystem::path currPath(part);

        // If this directory doesn't exist, then make it
        if (!boost::filesystem::exists(currPath)) {
            try {
                if (!boost::filesystem::create_directories(currPath)) {
                    throw po::error(
                            std::string("Failed to create dir: ") + part);
                }
            }
            catch(boost::filesystem::filesystem_error const& err) {
                throw po::error(std::string("Failed to create dir: ") + part);
            }
        }

        // If the configuration points to a file and not a directory, then
        // throw
        if (!boost::filesystem::is_directory(currPath)) {
            throw po::error(std::string("Configured storage path: ")
                    + part + std::string(" is not a directory"));
        }

        // Try to create a new file in the storage directory.
        try
        {
            boost::filesystem::path testFile = currPath.append("0");
            std::ofstream writeFile;
            writeFile.exceptions (
                    std::ifstream::failbit | std::ifstream::badbit );
            writeFile.open(currPath.string());
            writeFile.close();

            boost::filesystem::remove(testFile);
        }
        catch(std::ifstream::failure &writeErr)
        {
            throw po::error(std::string("Configured storage path: ")
                    + part + std::string(" cannot be written to"));
        }
        catch(boost::filesystem::filesystem_error const& err)
        {
            throw po::error(std::string("Configured storage path: ")
                    + part + std::string(" cannot be written to"));
        }
    }

}

/**
 * Parse the list of peer addresses given as a command line option into a
 * Service Address
 */
ServiceAddress parsePeerAddressList(std::string const& peerListString
        , int defaultPort) {

    ServiceAddress result;

    std::vector<std::string> parts;
    boost::split(parts, peerListString, boost::is_any_of(","));


    BOOST_FOREACH(std::string& part, parts) {
        std::vector<std::string> hostAndOptionalPort;

        if (part.empty()) {
            throw po::error(std::string("Invalid value for peer address")
                    + peerListString);
        }

        boost::split(hostAndOptionalPort, part, boost::is_any_of(":"));

        if (hostAndOptionalPort.empty() || hostAndOptionalPort.size() > 2) {
            throw po::error(std::string("Invalid value for owned ports")
                    + peerListString);
        }

        std::string hostname = hostAndOptionalPort[0];
        int port = defaultPort;

        if (hostAndOptionalPort.size() > 1) {
            port = std::stoi(hostAndOptionalPort[1]);
        }

        ServiceEndpoint* endpoint = result.add_endpoint();
        endpoint->set_hostname(hostname);
        endpoint->set_port(port);
    }

    return result;
}

CasterNodeConfig CasterNodeOptionParser::parseCommandLine(int argc, char **argv)
{
    CasterNodeConfig result;

    po::options_description desc("Runtime Arguments");

    bool help = false; // If this is set, this method will print and exit
    bool seed = false;
    bool remoteSeed = false;
    int chatterPort(DEFAULT_CHATTER_PORT);
    std::string ownedPortsStr("3131-3140");
    std::string peerAddressesStr;
    ServiceAddress peerAddresses;

    std::string internalInterface; // interface for intra-cluster communication
    std::string externalInterface; // interface for extra-cluster communication
    std::string remoteInterface;   // interface for inter-cluster communication

    std::vector<int> ownedPorts;

    std::string storageDirsStr;

    std::vector<std::string> storageDirs;

    // Describe the set of options that can be used for configuring this node
    desc.add_options()
            ("help,h", "Prints this info :)")
            ("seed,s", "Indicates this node is the seed of a cluster")
            ("remote-seed,R"
                    , "Indicates this node is the seed cluster bonded "
                            "to a remote cluster")
            ("chatter-port,c", po::value<int>(&chatterPort)
                    , "Port used for clustering chatter (default 3130)")
            ("owned-ports,o", po::value<std::string>(&ownedPortsStr)
                    , "Set of ports for general use (default 3131-3140)")
            ("peer-address,P", po::value<std::string>(&peerAddressesStr)
                    , "Any other hosts(:chatter_port) as a comma-separated "
                            "list in the cluster to be joined either as a "
                            "local or remote peer.  Only the first "
                            "entry is used")
            ("internal-interface,i"
                    , po::value<std::string>(&internalInterface)->required()
                    , "Network Interface to use for communication among "
                            "peer nodes")
            ("external-interface,e"
                    , po::value<std::string>(&externalInterface)
                    , "Network Interface to use for communication between "
                            "this node and external tools or applications.  "
                            "Defaults to the internal interface")
            ("remote-interface,r"
                    , po::value<std::string>(&remoteInterface)
                    , "Network Interface to use for communication between "
                            "this node and remote peers.  Defaults to the "
                            "internal interface")
            ("storage-dirs,d"
                    , po::value<std::string>(&storageDirsStr)->required()
                    , "Comma separated list of directories to use for storage");

    po::variables_map opts;

    if (argc == 1)
    {
        help = true;
    }

    try
    {
        po::store(po::parse_command_line(argc, argv, desc), opts);
        po::notify(opts);

        int i = 0;

        if (opts.count("help"))
        {
            help = true;
        }
        if (opts.count("seed"))
        {
            seed = true;
        }
        if (opts.count("peer-address"))
        {
            if (seed)
            {
                throw po::error("A node a with peer address cannot be a seed");
            }

            peerAddresses = parsePeerAddressList(peerAddressesStr
                    , DEFAULT_CHATTER_PORT);
        }
        if (opts.count("remote-seed"))
        {
            if (seed)
            {
                throw po::error(
                        "A node cannot be a seed and remote seed at once");
            }
            remoteSeed = true;
        }

        if (!seed && peerAddresses.endpoint_size() == 0 && !help)
        {
            throw po::error("A node has to have a peer address or be a seed");
        }

        parsePortList(ownedPortsStr, ownedPorts);

        parseAndValidateStorageDirectories(storageDirsStr, storageDirs);

        if (ownedPorts.size() < 9)
        {
            throw po::error("Owned port list is too small.  "
                    "Should include 9-19 ports");
        }

        if (ownedPorts.size() > 19)
        {
            throw po::error("Owned port list is too larger.  "
                    "Should include 9-19 ports");
        }

        if (externalInterface.empty())
        {
            externalInterface = internalInterface;
        }

        if (remoteInterface.empty())
        {
            remoteInterface = internalInterface;
        }

        if (!HostResolver::isLocalInterface(internalInterface)) {
            throw po::error("Cannot listen on specified internal interface: "
                    + internalInterface);
        }

        if (!HostResolver::isLocalInterface(externalInterface)) {
            throw po::error("Cannot listen on specified external interface: "
                    + externalInterface);
        }

        if (!HostResolver::isLocalInterface(remoteInterface)) {
            throw po::error("Cannot listen on specified remote interface: "
                    + remoteInterface);
        }
    }
    catch (po::error &e)
    { // This catches parsing errors
        if (!help)
        {
            std::cerr << "Failed to Parse Command Line: "
                    << e.what()
                    << std::endl
                    << std::endl;

            desc.print(std::cerr);

            std::cerr << std::endl;
            throw e;
        }
    }

    // If all we want is help, just print and exit
    if (help)
    {
        desc.print(std::cout);
        std::cout << std::endl;
        throw po::error("Only help requested");
    }

    result.set_seed(seed);
    result.set_remote_seed(seed);
    result.mutable_peer_address()->CopyFrom(peerAddresses);
    result.set_chatter_port(chatterPort);
    result.set_internal_interface(internalInterface);
    result.set_external_interface(externalInterface);
    result.set_remote_interface(remoteInterface);

    for (int i = 0; i < ownedPorts.size(); i++) {
        result.add_owned_port(ownedPorts[i]);
    }

    return result;
}

/**
 * This is just a more convenient method of passing in command line arguments
 * for testing
 */
CasterNodeConfig CasterNodeOptionParser::parseCommandLine(
        std::vector<std::string> args)
{
    int argc = args.size() + 1;
    std::vector<char*> argsAsChars(args.size() + 1);

    for (int i = 0; i < args.size(); i++) {
        argsAsChars[i + 1] = &(args[i][0]);
    }

    return parseCommandLine(argc, &(argsAsChars[0]));
}

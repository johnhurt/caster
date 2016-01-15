//
// Created by Kevin Guthrie on 12/14/15.
//

#ifndef CASTER_NODE_CASTERNODEOPTIONPARSER_H
#define CASTER_NODE_CASTERNODEOPTIONPARSER_H

#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/exception/all.hpp>

#include <iostream>
#include <core/net/HostResolver.h>
#include <CasterNodeConfig.pb.h>

namespace po = boost::program_options;

/**
 * This static class contains the code that parses and validates the command
 * line options and produces a CasterNodeConfig object containing all the users
 * specifications
 */
class CasterNodeOptionParser
{
public:
    static const int DEFAULT_CHATTER_PORT = 3130;

    static CasterNodeConfig parseCommandLine(std::vector<std::string> args);

    static CasterNodeConfig parseCommandLine(int argc, char** argv);

};


#endif //CASTER_NODE_CASTERNODEOPTIONPARSER_H

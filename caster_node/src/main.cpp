#include <core/CasterNodeOptionParser.h>
#include "core/CasterNodeMain.h"


int main(int argc, char * * argv)
{
    CasterNodeConfig config;

    try
    {
        config = CasterNodeOptionParser::parseCommandLine(argc, argv);
    }
    catch(std::exception& e) {
         //The excetption should already be handled
        return 1;
    }

    CasterNodeMain caster(config);

    caster.start();
    caster.join();

    return caster.exitCode();
}
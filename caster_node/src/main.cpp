#include "core/CasterNodeMain.h"


int main(int argc, const char *const *const argv)
{
    std::vector<std::string> args;

    for (int i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }

    return CasterNodeMain(args).exitCode();
}
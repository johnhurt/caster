//
// Created by Kevin Guthrie on 10/30/15.
//

#include <boost/foreach.hpp>
#include <boost/exception/all.hpp>

#include <iostream>
#include <core/net/HostResolver.h>
#include <core/impl/InMemoryStorageHandler.h>

#include "CasterNodeMain.h"

/**
 * Get the storage manager described by the node configuration
 */
StorageManager getStorageManager(CasterNodeConfig const& config) {

    std::vector< of paths for result

    BOOST_FOREACH(std::string directory
            , config.storage_directories())
    {
        if (directory.compare("in-memory")) {
            return InMemoryStorageHandler();
        }
        else {

        }
    }

}

/**
 * Create a caster node that contains the given command line configurations
 */
CasterNode createCasterNodeFromConfig(CasterNodeConfig const& config)
{
    CasterNode result;
    result.mutable_config()->CopyFrom(config);
    return result;
}

CasterNodeMain::CasterNodeMain(CasterNodeConfig config)
: _exitCode(0)
, stopRequested(false)
, shutdownListener(startStopIoService)
, _localNode(createCasterNodeFromConfig(config))
, _chatterServer(componentIoService, _localNode)
{

}

size_t CasterNodeMain::incrementThreadPoolSize(size_t const &amount)
{
    boost::recursive_mutex::scoped_lock lock(componentThreadMutex);

    for (int i = 0; i < amount; i++) {
        componentThreads.create_thread([&](){componentIoService.run();});
    }

    return componentThreads.size();
}

void CasterNodeMain::start()
{

    boost::recursive_mutex::scoped_lock lock(startStopMutex);

    shutdownListener.add(SIGINT);
    shutdownListener.add(SIGTERM);
    shutdownListener.async_wait(boost::bind(&CasterNodeMain::stop, this));

    if (stopRequested) {
        return;
    }

    try {
        _chatterServer.start();
    }
    catch (std::exception & e) {
        std::cerr
                << "Failed to start local node: " << std::endl
                << boost::current_exception_diagnostic_information(true)
                << std::endl;
        _exitCode = 2;
        stop();
    }

    // Start the thread in the background that listens for the shutdown signal
    ioServiceThread = boost::thread([&](){ startStopIoService.run(); });

    incrementThreadPoolSize(20);

}


void CasterNodeMain::stop()
{
    boost::recursive_mutex::scoped_lock lock(startStopMutex);

    stopRequested = true;

    std::cerr << std::endl << std::endl << "Exiting..." << std::endl;
    _chatterServer.stop();
    componentIoService.stop();

    componentThreads.join_all();

    startStopIoService.stop();
}

void CasterNodeMain::join()
{
    componentThreads.join_all();
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

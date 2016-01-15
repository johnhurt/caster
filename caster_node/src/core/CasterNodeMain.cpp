//
// Created by Kevin Guthrie on 10/30/15.
//

#include <boost/foreach.hpp>
#include <boost/exception/all.hpp>

#include <iostream>
#include <core/net/HostResolver.h>

#include "CasterNodeMain.h"


CasterNodeMain::CasterNodeMain(CasterNodeConfig config)
: _exitCode(0)
, stopRequested(false)
, shutdownListener(startStopIoService)
, _config(config)
, _chatterServer(config, componentIoService)
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
    ioServiceThread = boost::thread([&](){startStopIoService.run();});

    incrementThreadPoolSize(20);

}


void CasterNodeMain::stop()
{
    boost::recursive_mutex::scoped_lock lock(startStopMutex);

    stopRequested = true;

    std::cerr << std::endl << std::endl << "Exiting..." << std::endl;
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

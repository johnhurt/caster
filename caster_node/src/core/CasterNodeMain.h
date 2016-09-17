//
// Created by Kevin Guthrie on 10/30/15.
//

#ifndef CASTER_NODE_CASTERNODEMAIN_H
#define CASTER_NODE_CASTERNODEMAIN_H

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <CasterNodeConfig.pb.h>
#include "CasterNode.pb.h"
#include "ChatterServer.h"
#include "StorageManager.h"

/**
 * EntryPoint to the
 */
class CasterNodeMain
{

private:
    // These objects are used to listen for the shutdown event and
    // gracefully shutdown all the components of the caster node
    volatile bool stopRequested;
    boost::recursive_mutex startStopMutex;
    boost::asio::io_service startStopIoService;
    boost::asio::signal_set shutdownListener;
    boost::thread ioServiceThread;

    // These objects are used for managing the threads used by components
    // of the caster node application
    boost::recursive_mutex componentThreadMutex;
    boost::thread_group componentThreads;
    boost::asio::io_service componentIoService;

    CasterNodeConfig _config;
    CasterNode _localNode;
    ChatterServer _chatterServer;
    StorageManager _storageManager;
    int _exitCode;

public:

    /**
     * Create a new instance of a caster node with the given program arguments
     */
    CasterNodeMain(CasterNodeConfig config);

    /**
     * Increase the number of threads in the thread pool by the given amount
     */
    size_t incrementThreadPoolSize(size_t const& amount);

    /**
     * Start the Caster Node in the background
     */
    void start();

    /**
     * Stop the Caster Node application, and wait for it to shutdown.
     */
    void stop();

    /**
     * Join the calling thread to all threads in the Caster Node application
     */
    void join();

    CasterNodeConfig const& config() const;

    int const& exitCode() const;

    CasterNode const& localNode() const;
};


#endif //CASTER_NODE_CASTERNODEMAIN_H

//
// Created by Kevin Guthrie on 1/15/16.
//

#include <iostream>
#include "RaftNode.h"

int RaftNode::getNextMutinyTimer()
{
    int dist = rand() % MUTINY_TIMEOUT_DISTRIBUTION_MS;
    dist -= MUTINY_TIMEOUT_DISTRIBUTION_MS / 2;
    return dist + MEAN_MUTINY_TIMEOUT_MS;
}

int RaftNode::getNextHeartbeatInterval()
{
    int dist = rand() % HEARTBEAT_INTERVAL_DISTRIBUTION_MS;
    dist -= HEARTBEAT_INTERVAL_DISTRIBUTION_MS / 2;
    return dist + MEAN_HEARTBEAT_INTERVAL_MS;
}

RaftNode::RaftNode(boost::asio::io_service &ioService
        , bool startAsLeader
        , RequestManager& requestManager
        , RaftStateManager& stateManager)
: ioService(ioService)
, mutinyTimer(ioService)
, heartbeatTimer(ioService)
, requestManager(requestManager)
, stateManager(stateManager)
, _role(startAsLeader ?  Leader : Applicant)
, _nodeId(startAsLeader ?  0 : -1)
{

}

void RaftNode::start()
{
    if (_role == Leader)
    {
        mutinyTimer.expires_from_now(
                boost::posix_time::milliseconds(getNextMutinyTimer()));

        mutinyTimer.async_wait([this](const boost::system::error_code& e)
        {
            if (e != boost::asio::error::operation_aborted)
            {
                handleTimeout();
            }
        });
    }
    else {
        sendHearbeat();
    }
}

void RaftNode::stop()
{
    mutinyTimer.cancel();
    heartbeatTimer.cancel();
}

void RaftNode::handleTimeout()
{
    campaignToBecomeLeader();
}


void RaftNode::campaignToBecomeLeader()
{
    // If there is only one node in the set of nodes, and it is this node
    // we are the _leader by default


    // Send out to all nodes to collect votes

    // If we got enough votes to become a _leader, then start sending out
    // heartbeats

    // otherwise start the timeout cycle
}

void RaftNode::scheduleHeartbeat()
{
    heartbeatTimer.expires_from_now(
            boost::posix_time::milliseconds(getNextHeartbeatInterval()));

    heartbeatTimer.async_wait([this](const boost::system::error_code& e) {
        sendHearbeat();
    });
}

/**
 * Send a heartbeat request to all the other nodes
 */
void RaftNode::sendHearbeat()
{
    std::cout << "lub dub" << std::endl;
    scheduleHeartbeat();
}

AppendRaftEntriesResponse RaftNode::handleAppendEntries(
        AppendRaftEntriesRequest const &request)
{

}

RequestRaftVoteResponse RaftNode::handleRequestVote(
        RequestRaftVoteRequest const &request)
{

}

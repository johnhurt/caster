//
// Created by Kevin Guthrie on 1/15/16.
//

#ifndef CASTER_NODE_RAFTNODE_H
#define CASTER_NODE_RAFTNODE_H

#include <boost/asio/io_service.hpp>
#include <boost/asio/deadline_timer.hpp>
#include <CasterNode.pb.h>

#include <AppendRaftEntriesRequest.pb.h>
#include <AppendRaftEntriesResponse.pb.h>
#include <RequestRaftVoteRequest.pb.h>
#include <RequestRaftVoteResponse.pb.h>
#include <RequestVoteResponse.pb.h>
#include <core/rpc/ChatterRpcMessages.h>

#include "RaftNodeRole.h"
#include "RequestManager.h"
#include "RaftStateManager.h"

class RaftNode
{

private:

    static const int MEAN_MUTINY_TIMEOUT_MS = 5000;
    static const int MUTINY_TIMEOUT_DISTRIBUTION_MS = 2000;

    static const int MEAN_HEARTBEAT_INTERVAL_MS = 1000;
    static const int HEARTBEAT_INTERVAL_DISTRIBUTION_MS = 400;

    static int getNextMutinyTimer();
    static int getNextHeartbeatInterval();

private:

    boost::asio::io_service& ioService;
    boost::asio::deadline_timer mutinyTimer;
    boost::asio::deadline_timer heartbeatTimer;

    RequestManager& requestManager;
    RaftStateManager& stateManager;

    RaftNodeRole _role;
    int _nodeId;

    std::vector<CasterNode> _nodes;
    CasterNode _leaderNode;

public:

    RaftNode(boost::asio::io_service& io_service
            , bool startAsLeader
            , RequestManager& requestManager
            , RaftStateManager& stateManager);

    void start();

    void stop();


    AppendRaftEntriesResponse handleAppendEntries(
            AppendRaftEntriesRequest const& request);

    RequestRaftVoteResponse handleRequestVote(
            RequestRaftVoteRequest const& request);

private:


    void handleTimeout();

    void campaignToBecomeLeader();

    void scheduleHeartbeat();

    void sendHearbeat();

};


#endif //CASTER_NODE_RAFTNODE_H

//
// Created by Kevin Guthrie on 1/28/16.
//

#ifndef CASTER_NODE_COMMUNICATIONINTEROP_H
#define CASTER_NODE_COMMUNICATIONINTEROP_H

#include <RequestRaftVoteRequest.pb.h>
#include <RequestRaftVoteResponse.pb.h>
#include <AppendRaftEntriesRequest.pb.h>
#include <AppendRaftEntriesResponse.pb.h>

/**
 * Interface for all the interations between raft nodes
 */
class RequestManager {
public:

    virtual RequestRaftVoteResponse requestVote(
            RequestRaftVoteRequest const& request) = 0;

    virtual AppendRaftEntriesResponse appendEntries(
            AppendRaftEntriesRequest const& request) = 0;

    virtual ~RequestManager(){}

};

#endif //CASTER_NODE_COMMUNICATIONINTEROP_H

//
// Created by Kevin Guthrie on 1/24/16.
//

#include "RequestRaftVoteHandler.h"

HANDLE_REQUEST_WITH_PROPOERTIES(RequestRaftVote, RaftNode&, raftNode, request) {
    return raftNode.handleRequestVote(request);
}
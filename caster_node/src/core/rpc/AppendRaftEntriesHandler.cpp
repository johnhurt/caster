//
// Created by Kevin Guthrie on 1/28/16.
//

#include "AppendRaftEntriesHandler.h"

HANDLE_REQUEST_WITH_PROPOERTIES(AppendRaftEntries
        , RaftNode&, raftNode, request) {
    return raftNode.handleAppendEntries(request);
}
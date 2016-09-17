//
// Created by Kevin Guthrie on 1/24/16.
//

#ifndef CASTER_NODE_REQUESTRAFTVOTEHANDLER_H
#define CASTER_NODE_REQUESTRAFTVOTEHANDLER_H

#include "RpcMessageHandler.hpp"
#include <core/raft/RaftNode.h>

DEFINE_HANDLER_CLASS_WITH_PROPETIES(RequestRaftVote, RaftNode&, raftNode);

#endif //CASTER_NODE_REQUESTRAFTVOTEHANDLER_H

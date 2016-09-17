//
// Created by Kevin Guthrie on 1/28/16.
//

#ifndef CASTER_NODE_APPENDRAFTENTRIESHANDLER_H
#define CASTER_NODE_APPENDRAFTENTRIESHANDLER_H

#include "RpcMessageHandler.hpp"
#include <core/raft/RaftNode.h>

DEFINE_HANDLER_CLASS_WITH_PROPETIES(AppendRaftEntries, RaftNode&, raftNode);

#endif //CASTER_NODE_APPENDRAFTENTRIESHANDLER_H

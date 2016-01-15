//
// Created by Kevin Guthrie on 1/6/16.
//

#ifndef CASTER_NODE_CHATTERRPCREQUESTS_H
#define CASTER_NODE_CHATTERRPCREQUESTS_H

#include <core/TypedMessage.hpp>
#include <MessageType.pb.h>
#include <MessageSubtype.pb.h>
#include <DescribeRequest.pb.h>
#include <DescribeResponse.pb.h>
#include <EmptyMessage.pb.h>

#define DEFINE_RPC_MESSAGES(T) \
        DEFINE_TYPED_MESSAGE(MessageType::T, MessageSubtype::Request \
                , T##Request, T##RpcRequest); \
        DEFINE_TYPED_MESSAGE(MessageType::T, MessageSubtype::Response \
                , T##Response, T##RpcResponse) \

DEFINE_RPC_MESSAGES(Describe);

#endif //CASTER_NODE_CHATTERRPCREQUESTS_H

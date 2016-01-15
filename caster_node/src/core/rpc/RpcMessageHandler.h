//
// Created by Kevin Guthrie on 1/14/16.
//

#ifndef CASTER_NODE_RPCMESSAGEHANDLER_H
#define CASTER_NODE_RPCMESSAGEHANDLER_H

#define DEFINE_HANDLER_CLASS(T) \
        class T ## Handler \
        : public RpcMessageHandler<T ## Request, T ## Response> \
        { \
        public: \
            T ## Response handleRequest(T ## Request const& request) const ; \
        }

#define HANDLE_REQUEST(T, request) \
        T ## Response T ## Handler::handleRequest( \
                T ## Request const& request) const


template <class RequestType, class ResponseType>
class RpcMessageHandler
{

    virtual ResponseType handleRequest(RequestType const& request) const = 0;

};


#endif //CASTER_NODE_RPCMESSAGEHANDLER_H

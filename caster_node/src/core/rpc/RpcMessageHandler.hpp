//
// Created by Kevin Guthrie on 1/14/16.
//

#ifndef CASTER_NODE_RPCMESSAGEHANDLER_H
#define CASTER_NODE_RPCMESSAGEHANDLER_H

#include "ChatterRpcMessages.h"

#define TILDA(T) ~T


#define DEFINE_HANDLER_CLASS(T) DEFINE_HANDLER_CLASS_PRIVATE(T, T ## Handler)

#define DEFINE_HANDLER_CLASS_PRIVATE(T, H) \
        class H \
        : public RpcMessageHandler<T ## Request, T ## Response> \
        { \
        public: \
            T ## Response handleRequest(T ## Request const& request) const ; \
            virtual ~H() ; \
        }

#define DEFINE_HANDLER_CLASS_WITH_PROPETIES(T, PT1, PN1) \
        DEFINE_HANDLER_CLASS_WITH_PROPETIES_PRIVATE(T, T ## Handler, PT1, PN1)

#define DEFINE_HANDLER_CLASS_WITH_PROPETIES_PRIVATE(T, H, PT1, PN1) \
        class H \
        : public RpcMessageHandler<T ## Request, T ## Response> \
        { \
        private: \
            PT1 PN1; \
        public: \
            H(PT1 PN1); \
            virtual ~H() ; \
            T ## Response handleRequest(T ## Request const& request) const ; \
        }

#define HANDLE_REQUEST(T, request) \
        HANDLE_REQUEST_PRIVATE(T, T ## Handler, request)

#define HANDLE_REQUEST_PRIVATE(T, H, request) \
        H::~H(){} \
        T ## Response H::handleRequest( \
                T ## Request const& request) const

#define HANDLE_REQUEST_WITH_PROPOERTIES(T, PT1, PN1, request) \
        HANDLE_REQUEST_WITH_PROPOERTIES_PRIVATE(T, T ## Handler \
                , PT1, PN1, request)

#define HANDLE_REQUEST_WITH_PROPOERTIES_PRIVATE(T, H, PT1, PN1, request) \
        H::H( PT1 PN1 ) : PN1(PN1) {} \
        H::~H(){} \
        T ## Response H::handleRequest( \
                T ## Request const& request) const


template <class RequestType, class ResponseType>
class RpcMessageHandler
{
protected:

    virtual ResponseType handleRequest(RequestType const& request) const = 0;

public:

    virtual ~RpcMessageHandler() {};

};


#endif //CASTER_NODE_RPCMESSAGEHANDLER_H

//
// Created by Kevin Guthrie on 1/6/16.
//

#ifndef CASTER_NODE_RPCREQUEST_H
#define CASTER_NODE_RPCREQUEST_H

#include <vector>
#include <boost/cstdint.hpp>
#include <EmptyMessage.pb.h>

#define DEFINE_TYPED_MESSAGE(TypeValue, SubtypeValue, MessageClass, TypeName) \
        typedef TypedMessage<TypeValue, SubtypeValue, MessageClass> TypeName

typedef std::vector<boost::uint8_t> MessageBuffer;

template <int type, int subtype, class MessageClass>
class TypedMessage
{
public:
    static const int HEADER_LENGTH = 8;

    /**
     * Get the type of the message contained in the buffer
     */
    static int getMessageType(MessageBuffer const& messageBuffer) {
        return readIntFromBuffer(messageBuffer, 3, 0);
    }

    /**
     * Get the subtype of the message contained in the buffer
     */
    static int getMessageSubtype(MessageBuffer const& messageBuffer) {
        return readIntFromBuffer(messageBuffer, 1, 3);
    }

    /**
     * Get the size of the message (not including the header)
     */
    static int getMessageSize(MessageBuffer const& messageBuffer) {
        return readIntFromBuffer(messageBuffer, 4, 4);
    }

private:

    /**
     * Read the next little-endian int from the given buffer
     */
    static int readIntFromBuffer(MessageBuffer const& buffer
            , int byteCount, int position)
    {
        int result = 0;

        for (int i = 0; i < byteCount; i++) {
            result += (buffer[position + i]) << (8 * i);
        }

        return result;
    }

private:
    MessageBuffer _buffer;

public:

    /**
     * Create a new rpc request from the given request classs
     */
    TypedMessage(MessageClass const& request)
    {
        int requestSize = request.GetCachedSize();
        if (requestSize == 0) {
            requestSize = request.ByteSize();
        }

        _buffer.resize(requestSize + HEADER_LENGTH);
        writeHeader(_buffer, requestSize);
        request.SerializeWithCachedSizesToArray(&_buffer[HEADER_LENGTH]);
    }

    MessageBuffer const& buffer() const { return _buffer; }


private:

    /**
     * Write the header for the request with the given request size to the given
     * buffer
     */
    void writeHeader(MessageBuffer& buffer, int requestSize)
    {
        int requestTypeAsInt = type;
        int requestSubtypeAsInt = subtype;
        writeIntToBuffer(buffer, requestTypeAsInt, 3, 0);
        writeIntToBuffer(buffer, requestSubtypeAsInt, 1, 3);
        writeIntToBuffer(buffer, requestSize, 4, 4);
    }

    /**
     * Write the given int to the buffer in little-endian format
     */
    void writeIntToBuffer(MessageBuffer& buffer
            , int toWrite
            , int byteCount
            , int position)
    {
        for (int i = 0; i < byteCount; i++)
        {
            buffer[position + i] = toWrite & 0xFF;
            toWrite >>= 8;
        }
    }

};

// Convenience method to get to static methods of the typed message
typedef TypedMessage<0, 0, EmptyMessage> TypedMessageUtil;


#endif //CASTER_NODE_RPCREQUEST_H

//
// Created by Kevin Guthrie on 1/28/16.
//

#ifndef CASTER_NODE_RAFTSTATEMANAGER_H
#define CASTER_NODE_RAFTSTATEMANAGER_H

/**
 * Interface for managing the state of the raft node
 */
class RaftStateManager {
public:

    virtual int getTerm() = 0;

    virtual ~RaftStateManager() {}

};

#endif //CASTER_NODE_RAFTSTATEMANAGER_H

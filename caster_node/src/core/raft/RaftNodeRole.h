//
// Created by Kevin Guthrie on 1/24/16.
//

#ifndef CASTER_NODE_RAFTNODESTATE_H
#define CASTER_NODE_RAFTNODESTATE_H

enum RaftNodeRole {
    Applicant = 0
    , Follower = 1
    , Candidate = 2
    , Leader = 3
};

#endif //CASTER_NODE_RAFTNODESTATE_H

//
// Created by Kevin Guthrie on 1/14/16.
//

#include "DescribeHandler.h"

HANDLE_REQUEST(Describe, request) {
    DescribeResponse result;

    result.set_verbose(request.verbose());
    result.set_description("Everything is fine");

    return result;
}
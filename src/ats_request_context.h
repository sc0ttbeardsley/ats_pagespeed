/*
 * ats_request_context.h
 *
 *  Created on: Jun 12, 2013
 *      Author: bgeffon
 */

#pragma once
#ifndef ATS_REQUEST_CONTEXT_H_
#define ATS_REQUEST_CONTEXT_H_
#include "net/instaweb/http/public/request_context.h"
#include "ats_pagespeed_logging.h"

class AtsRequestContext : public net_instaweb::RequestContext {
public:
    AtsRequestContext(net_instaweb::AbstractMutex* logging_mutex, net_instaweb::Timer* timer)
    : RequestContext(logging_mutex, timer)
    {
      LOG_DEBUG("Created request context %p", this);
    }

    virtual ~AtsRequestContext() {
      LOG_DEBUG("Deleted request context %p", this);
    }
};


#endif /* ATS_REQUEST_CONTEXT_H_ */

/*
 * Copyright (c) 2013 LinkedIn Corp. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of the license at
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the
 * License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied.
 *
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

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
#ifndef ATS_THREAD_SYSTEM_H_
#define ATS_THREAD_SYSTEM_H_
#include "ats_pagespeed_logging.h"
#include <pthread.h>

#include "net/instaweb/util/public/pthread_thread_system.h"
#include "net/instaweb/util/public/thread.h"
#include "net/instaweb/util/public/thread_system.h"
#include "net/instaweb/util/public/pthread_rw_lock.h"
#include "net/instaweb/util/public/condvar.h"

// Forward declare this (should be added to atscppapi at some point)
extern "C" void *TSThreadInit(void);

namespace ats_pagespeed {
class AtsThreadSystem : public net_instaweb::PthreadThreadSystem {
public:

  // We have to define a Thread System because we have to initailize
  // the threads used by pagespeed so they can call back into the ATS
  // api.
  virtual void BeforeThreadRunHook() {

    // This will establish the thread specifics necessary for stat aggregation
    // The way the stats work is they are thread specific and periodically
    // aggregated, so we'll need to call TSThreadInit() to allow this to happen.
    TSThreadInit();
    TS_DEBUG(TAG, "Initialized Pagespeed thread for ATS thread 0x%lx", pthread_self());
    net_instaweb::PthreadThreadSystem::BeforeThreadRunHook();
  }

  virtual ~AtsThreadSystem() { }
};

} /* ats_pagespeed */



#endif /* ATS_THREAD_SYSTEM_H_ */

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
#ifndef ATS_PAGESPEED_H_
#define ATS_PAGESPEED_H_

#include <map>
#include <vector>
#include <string>
#include "net/instaweb/automatic/public/proxy_fetch.h"

#ifdef GPERFTOOLS
#include <gperftools/heap-profiler.h>
#endif

namespace ats_pagespeed {

class AtsRewriteDriverFactory;
class AtsMessageHandler;
class AtsRewriteOptions;
class AtsServerContext;

struct GlobalConf {
  AtsRewriteDriverFactory *driver_factory;
  AtsMessageHandler *handler;
  AtsRewriteOptions *options;

  GlobalConf() :
    driver_factory(NULL), handler(NULL), options(NULL) {
  }
};

extern GlobalConf global_config;

namespace config {
// Here because of the dependency between ats_pagespeed_configure and ats_rewrite_options,
typedef std::map<std::string, std::vector<std::string> > OptionsMap;

} /* config */

} /* ats_pagespeed */

#endif /* ATS_PAGESPEED_H_ */

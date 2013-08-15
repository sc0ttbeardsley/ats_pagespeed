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
#ifndef ATS_SERVER_CONTEXT_H_
#define ATS_SERVER_CONTEXT_H_

#include "ats_rewrite_options.h"

#include "net/instaweb/system/public/system_server_context.h"
#include "net/instaweb/util/public/statistics.h"

namespace ats_pagespeed {

class AtsRewriteOptions;

class AtsServerContext : public net_instaweb::SystemServerContext {
 public:
  explicit AtsServerContext(AtsRewriteDriverFactory* factory);
  virtual ~AtsServerContext();

  virtual bool ProxiesHtml() const {
    return true;
  }

  void Init();

  AtsRewriteOptions *config();
  AtsRewriteDriverFactory *ats_rewrite_driver_factory() { return ats_factory_; }

 private:
  bool initialized_;
  AtsRewriteDriverFactory* ats_factory_;
};

} /* ats_pagespeed */

#endif /* ATS_SERVER_CONTEXT_H_ */

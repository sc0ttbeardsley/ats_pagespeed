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
#ifndef ATS_STATS_H_
#define ATS_STATS_H_
#include "ats_pagespeed_logging.h"
#include "ats_stats_variable.h"

#include "net/instaweb/util/public/statistics.h"
#include "net/instaweb/util/public/statistics_template.h"

namespace ats_pagespeed {

class AtsStats : public net_instaweb::ScalarStatisticsTemplate<AtsStatsVariable> {
public:
 AtsStats() {
   LOG_DEBUG("Created new instance of AtsStats %p", this);
 }

 virtual ~AtsStats() {
   LOG_DEBUG("Destroyed instance of AtsStats %p", this);
 }

protected:
  virtual AtsStatsVariable* NewVariable(const base::StringPiece& name, int index) {
    return new AtsStatsVariable(name);
  }

  // Yes this is a hack to make things work nicely
  virtual AtsStatsVariable* NewVariable(const base::StringPiece& name, int index) const {
    return new AtsStatsVariable(name);
  }

  virtual AtsStatsVariable* FindVariable(const base::StringPiece& name) const {
    return NewVariable(name, 0);
  }
};

} /* ats_pagespeed */

#endif /* ATS_STATS_H_ */

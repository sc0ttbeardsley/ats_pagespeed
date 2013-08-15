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
#ifndef ATS_STATS_VARIABLE_H_
#define ATS_STATS_VARIABLE_H_
#include <atscppapi/Stat.h>
#include <string>
#include <map>
#include "ats_pagespeed_logging.h"
#include "ats_thread_system.h"

#include "net/instaweb/util/public/statistics.h"
#include "net/instaweb/util/public/statistics_template.h"

namespace ats_pagespeed {

// This is the prefix used when creating a stat variable inside ats
// since we want them to be easily found we'll prefix with ats_pagespeed.
#define STAT_TAG TAG "."

class AtsStatsVariable : public net_instaweb::Variable {
public:
  AtsStatsVariable(const base::StringPiece &name);

  virtual ~AtsStatsVariable();

  virtual int64 Get() const;
  virtual void Set(int64 value);
  virtual int64 Add(int delta);
  virtual base::StringPiece GetName() const;

protected:
  atscppapi::Stat* LookupStat(const base::StringPiece &name);

private:
  // We can recreate already created stats so we'll use a map and a RW
  // lock, once they are all create threads will only need the Read Lock
  // so it shouldn't be too bad. Then under the covers atscppapi and ats
  // will keep per thread statistics and then aggregrate periodically.
  static net_instaweb::PthreadRWLock rw_lock_;
  static std::map<std::string, atscppapi::Stat *> stats_map_;
  atscppapi::Stat *stat_;
  base::StringPiece name_;
};

} /* ats_pagespeed */

#endif /* ATS_STATS_VARIABLE_H_ */

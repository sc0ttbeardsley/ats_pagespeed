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


#include <atscppapi/Stat.h>
#include "ats_pagespeed_logging.h"
#include "ats_stats_variable.h"

using namespace atscppapi;
using namespace ats_pagespeed;
using std::map;
using std::string;

net_instaweb::PthreadRWLock AtsStatsVariable::rw_lock_;
map<string, Stat *> AtsStatsVariable::stats_map_;

AtsStatsVariable::AtsStatsVariable(const base::StringPiece &name) : stat_(NULL), name_(name) {
  LOG_DEBUG("Created AtsStatsVariable %p", this);

  rw_lock_.ReaderLock();
  stat_ = LookupStat(name);
  rw_lock_.ReaderUnlock();

  // The stat was found so just return and we're constructed.
  if (stat_ != NULL) {
    return;
  }

  // Stat hasn't already been created create it and add it to
  // the map with a write lock.
  rw_lock_.Lock();
  // Make sure the stat wasn't added while we were waiting
  stat_ = LookupStat(name);
  if (stat_ != NULL) {
    rw_lock_.Unlock();
    return;
  }

  string stat_internal_name = string(STAT_TAG).append(name.as_string());
  stat_ = new Stat();
  stat_->init(stat_internal_name, Stat::SYNC_SUM, false);
  LOG_DEBUG("Created new AtsStatsVariable with name '%s'", stat_internal_name.c_str());
  stats_map_[name.as_string()] = stat_;
  rw_lock_.Unlock();
}

AtsStatsVariable::~AtsStatsVariable() {
  LOG_DEBUG("Destroying AtsStatsVariable %p", this);
}

int64 AtsStatsVariable::Get() const {
    return stat_->get();
}

base::StringPiece AtsStatsVariable::GetName() const {
  return name_;
}

void AtsStatsVariable::Set(int64 value) {
    stat_->set(value);
}

int64 AtsStatsVariable::Add(int delta) {
    stat_->increment(delta);
    return stat_->get();
}

Stat* AtsStatsVariable::LookupStat(const base::StringPiece &name) {
  // Remember you need to hold at least a read lock
  map<string, Stat *>::iterator stat_iter = stats_map_.find(name.as_string());
  if (stat_iter != stats_map_.end()) {
     return stat_iter->second;
  }
  return NULL;
}


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


#include "ats_pagespeed_logging.h"
#include "ats_rewrite_options.h"
#include <string>
#include <cstring>
#include <vector>

using namespace ats_pagespeed;
using std::string;
using std::vector;
using net_instaweb::StringCaseEqual;

net_instaweb::RewriteOptions::Properties* AtsRewriteOptions::ats_properties_ = NULL;

// See rewrite_options::Initialize and ::Terminate
void AtsRewriteOptions::Initialize() {
  LOG_ENTRY
  if (Properties::Initialize(&ats_properties_)) {
     TS_DEBUG(TAG, "Initializing SystemRewriteOptions");
     SystemRewriteOptions::Initialize();
     AddProperties();
   }
}

void AtsRewriteOptions::Terminate() {
  LOG_ENTRY
  if (Properties::Terminate(&ats_properties_)) {
    SystemRewriteOptions::Terminate();
  }
}

AtsRewriteOptions::AtsRewriteOptions() {
  LOG_ENTRY
  TS_DEBUG(TAG, "Created AtsRewriteOptions %p", this);
  Init();
}

AtsRewriteOptions::~AtsRewriteOptions() {
  LOG_ENTRY
  TS_DEBUG(TAG, "Destroyed AtsRewriteOptions %p", this);
}

void AtsRewriteOptions::AddProperties() {
  LOG_ENTRY

  MergeSubclassProperties(ats_properties_);
  AtsRewriteOptions config;
  config.InitializeSignaturesAndDefaults();
}

namespace {

int joinVector(vector<string> &vec, char ch = ',') {
  string res;
  for (vector<string>::const_iterator val_iter = vec.begin();
      val_iter != vec.end(); /* we advance it in the loop */) {
    res += *val_iter;
    if (++val_iter != vec.end())
      res += ch;
  }

  if (!res.empty()) {
    vec.resize(1);
    vec[0] = res;
  }

  return vec.size();
}

} /* anonymous namespace */

void AtsRewriteOptions::SetOptionsFromOptionsMap(const config::OptionsMap &options, net_instaweb::MessageHandler* handler,
    AtsRewriteDriverFactory* driver_factory) {
  LOG_ENTRY

  GoogleString msg;
  net_instaweb::RewriteOptions::OptionSettingResult result;

  for(config::OptionsMap::const_iterator option_iter = options.begin(); option_iter != options.end(); ++option_iter) {
    msg.clear();

    string name = option_iter->first;
    vector<string> vals = option_iter->second;
    int n_values = vals.size();

    if (StringCaseEqual(name, "EnableFilters") ||
        StringCaseEqual(name, "DisableFilters")) {
      if (n_values  > 1) {
        // Since we allow the user to specify Enable or Disable Filters as a sequence or even a single
        // quoted element we will handle the case where they specify the filter list as a sequence
        // by joining it into a single string.
        n_values = joinVector(vals);
      }
    }

    // the directive name is option_iter->first.
    if (n_values == 0) {
      // Do we really need to do the set_enabled( on, off, unplugged ) stuff??
      result = kOptionValueInvalid; // Is this case actually valid, can you have an option with no value?
    } else if (n_values == 1) {
      if (StringCaseEqual(name,"UseNativeFetcher") && driver_factory) {
        if (StringCaseEqual(vals[0], "on")) {
          driver_factory->set_use_native_fetcher(true);
          result = RewriteOptions::kOptionOk;
        } else if (StringCaseEqual(vals[0],"off")) {
          driver_factory->set_use_native_fetcher(false);
          result = RewriteOptions::kOptionOk;
        } else {
          result = RewriteOptions::kOptionValueInvalid;
        }
      } else if (StringCaseEqual(name,"DefaultThreadPoolSize") && driver_factory) {
        long thread_pool_size = strtol(vals[0].c_str(), NULL, 10);
        if (thread_pool_size > 0) {
          AtsRewriteDriverFactory::DEFAULT_THREAD_POOL_SIZE = static_cast<int>(thread_pool_size);
          result = RewriteOptions::kOptionOk;
        } else {
          result = RewriteOptions::kOptionValueInvalid;
        }
      } else {
        result = ParseAndSetOptionFromName1(name, vals[0], &msg, handler);
      }
    } else if (n_values == 2) {
      result = ParseAndSetOptionFromName2(name, vals[0], vals[1], &msg, handler);
    } else if (n_values == 3) {
      result = ParseAndSetOptionFromName3(name, vals[0], vals[1], vals[2], &msg, handler);
    } else {
      result = kOptionValueInvalid;
    }

    switch (result) {
      case net_instaweb::RewriteOptions::kOptionOk: {
        n_values = joinVector(vals, ' ');
        if (n_values == 1) {
          TS_DEBUG(TAG, "Option %s set to (%s).", name.c_str(), vals[0].c_str());
        } else {
          TS_DEBUG(TAG, "Option %s set.", name.c_str());
        }
        break;
      }
      case net_instaweb::RewriteOptions::kOptionNameUnknown: {
        TS_ERROR(TAG, "Option %s is an unknown option.", name.c_str());
        break;
      }
      case net_instaweb::RewriteOptions::kOptionValueInvalid: {
        TS_ERROR(TAG, "Option %s is had invalid values (%s)", name.c_str(), msg.c_str());
        break;
      }
    }
  }
}

AtsRewriteOptions* AtsRewriteOptions::Clone() const {
  LOG_ENTRY
  AtsRewriteOptions* options = new AtsRewriteOptions();
  options->Merge(*this);
  return options;
}

const AtsRewriteOptions* AtsRewriteOptions::DynamicCast(const net_instaweb::RewriteOptions* instance) {
  return dynamic_cast<const AtsRewriteOptions*>(instance);
}

AtsRewriteOptions* AtsRewriteOptions::DynamicCast(net_instaweb::RewriteOptions* instance) {
  return dynamic_cast<AtsRewriteOptions*>(instance);
}

void AtsRewriteOptions::Init() {
  LOG_ENTRY
  InitializeOptions(ats_properties_);
}

void AtsRewriteOptions::InitializeSignaturesAndDefaults() {
  LOG_ENTRY
  // This should become kModPagespeedVersion
  set_default_x_header_value("ats_pagespeed");
}


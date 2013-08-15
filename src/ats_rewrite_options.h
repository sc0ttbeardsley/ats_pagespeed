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
#ifndef ATS_REWRITE_OPTIONS_H_
#define ATS_REWRITE_OPTIONS_H_

#include "ats_rewrite_driver_factory.h"
#include "ats_pagespeed.h"

#include "net/instaweb/rewriter/public/rewrite_options.h"
#include "net/instaweb/system/public/system_rewrite_options.h"

namespace ats_pagespeed {

class AtsRewriteOptions : public net_instaweb::SystemRewriteOptions {

public:
  // See rewrite_options::Initialize and ::Terminate
  static void Initialize();
  static void Terminate();

  AtsRewriteOptions();
  ~AtsRewriteOptions();

  void SetOptionsFromOptionsMap(const config::OptionsMap &options, net_instaweb::MessageHandler* handler,
      AtsRewriteDriverFactory* driver_factory);

  // Make an identical copy of these options and return it.
  virtual AtsRewriteOptions* Clone() const;

  static const AtsRewriteOptions* DynamicCast(const RewriteOptions* instance);
  static AtsRewriteOptions* DynamicCast(RewriteOptions* instance);

private:
  static Properties* ats_properties_;

  void Init();
  void InitializeSignaturesAndDefaults();
  static void AddProperties();
};

} /* ats_pagespeed */

#endif /* ATS_REWRITE_OPTIONS_H_ */

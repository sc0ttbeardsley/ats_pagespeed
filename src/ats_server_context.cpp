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

#include "ats_server_context.h"
#include "ats_rewrite_driver_factory.h"
#include "ats_rewrite_options.h"

using namespace ats_pagespeed;

AtsRewriteOptions* AtsServerContext::config() {
  return AtsRewriteOptions::DynamicCast(global_options());
}

AtsServerContext::AtsServerContext(AtsRewriteDriverFactory* factory) :
    SystemServerContext(factory), initialized_(false),ats_factory_(factory){
  LOG_ENTRY
}

void AtsServerContext::Init() {
  LOG_ENTRY
  if (!initialized_) {
    initialized_ = true;
    set_lock_manager(ats_factory_->caches()->GetLockManager(config()));
    ats_factory_->InitServerContext(this);
  }
}
AtsServerContext::~AtsServerContext() {
  LOG_ENTRY
}

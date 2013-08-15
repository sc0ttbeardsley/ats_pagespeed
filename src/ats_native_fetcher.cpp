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

#include <tr1/memory>
#include "ats_native_fetcher.h"
#include "ats_pagespeed_logging.h"

using namespace atscppapi;
using namespace ats_pagespeed;
using std::tr1::shared_ptr;

AtsNativeAsyncFetcher::AtsNativeAsyncFetcher() : shutdown_(false) {
  LOG_DEBUG("(%p) Native fetcher is being created", this);
}

AtsNativeAsyncFetcher::~AtsNativeAsyncFetcher() {
  LOG_DEBUG("(%p) Native fetcher is being destroyed", this);
}

void AtsNativeAsyncFetcher::Fetch(const GoogleString& url,
                   net_instaweb::MessageHandler* message_handler,
                   net_instaweb::AsyncFetch* fetch) {
  if (shutdown_) {
    LOG_ERROR("(%p) Unable to use native fetcher because it has been shutdown.", this);
  }

  LOG_DEBUG("(%p) Using native fetcher to fetch url %s", this, url.c_str());

  Async::execute<AsyncHttpFetch>(this,
      new AsyncHttpFetch(url), shared_ptr<Mutex>());
}

void AtsNativeAsyncFetcher::ShutDown() {
  shutdown_ = true;
  LOG_DEBUG("(%p) Shutting down native fetcher.", this);
}

void AtsNativeAsyncFetcher::handleAsyncComplete(atscppapi::AsyncHttpFetch &fetch) {
  LOG_DEBUG("(%p) Native fetcher async fetch complete on url %s", this, fetch.getRequestUrl().getUrlString().c_str());
}

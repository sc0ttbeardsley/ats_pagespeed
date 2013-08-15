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
#ifndef ATS_NATIVE_FETCHER_H_
#define ATS_NATIVE_FETCHER_H_

#include <string>
#include <atscppapi/Async.h>
#include <atscppapi/AsyncHttpFetch.h>

#include "net/instaweb/http/public/url_async_fetcher.h"

namespace ats_pagespeed {
class AtsNativeAsyncFetcher : public net_instaweb::UrlAsyncFetcher,
                              public atscppapi::AsyncReceiver<atscppapi::AsyncHttpFetch> {
public:
    AtsNativeAsyncFetcher();
    virtual ~AtsNativeAsyncFetcher();


    // UrlAsyncFetcher interface
    virtual void Fetch(const GoogleString& url,
                       net_instaweb::MessageHandler* message_handler,
                       net_instaweb::AsyncFetch* fetch);

    virtual void ShutDown();

    // atscppapi AsyncReceiver interface
    virtual void handleAsyncComplete(atscppapi::AsyncHttpFetch &);
private:
    bool shutdown_;
};
} /* ats_pagespeed */

#endif /* ATS_NATIVE_FETCHER_H_ */

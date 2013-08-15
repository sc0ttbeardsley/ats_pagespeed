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
#ifndef ATS_BASE_FETCH_H_
#define ATS_BASE_FETCH_H_
#include <atscppapi/Mutex.h>
#include "ats_server_context.h"
#include <string>

#include "net/instaweb/http/public/async_fetch.h"
#include "net/instaweb/http/public/headers.h"
#include "net/instaweb/util/public/string.h"

namespace ats_pagespeed {

class AtsBaseFetch : public net_instaweb::AsyncFetch {
public:
  AtsBaseFetch(AtsServerContext* server_context,
               const net_instaweb::RequestContextPtr& request_ctx);
  virtual ~AtsBaseFetch();

  void PopulateRequestHeaders();
  void PopulateResponseHeaders();

  void Release();
private:
  virtual bool HandleWrite(const StringPiece& sp, net_instaweb::MessageHandler* handler);
  virtual bool HandleFlush( net_instaweb::MessageHandler* handler);
  virtual void HandleHeadersComplete();
  virtual void HandleDone(bool success);
  void Lock();
  void Unlock();
  void DecrefAndDeleteIfUnreferenced();

  std::string buffer_;
  AtsServerContext* server_context_;
  bool done_called_;
  bool last_buf_sent_;

  // How many active references there are to this fetch. Starts at two,
  // decremented once when Done() is called and once when Release() is called.
  int references_;
  atscppapi::Mutex mutex_;
};

} /* ats_pagespeed */


#endif /* ATS_BASE_FETCH_H_ */

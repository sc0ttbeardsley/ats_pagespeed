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


#include "ats_base_fetch.h"

using namespace ats_pagespeed;

AtsBaseFetch::AtsBaseFetch(AtsServerContext* server_context,
               const net_instaweb::RequestContextPtr& request_ctx) :
               server_context_(server_context),
               done_called_(false),
               last_buf_sent_(false),
               references_(2) /* one for the transformation, one for pagespeed */ {
  buffer_.reserve(1024 * 32); // 32kb is probably a good place to start
}

AtsBaseFetch::~AtsBaseFetch() {

}

void AtsBaseFetch::PopulateRequestHeaders() {

}

void AtsBaseFetch::PopulateResponseHeaders() {

}

void AtsBaseFetch::Release() {
  DecrefAndDeleteIfUnreferenced();
}

void AtsBaseFetch::Lock() {
  mutex_.lock();
}

void AtsBaseFetch::Unlock() {
  mutex_.unlock();
}

bool AtsBaseFetch::HandleWrite(const StringPiece& sp, net_instaweb::MessageHandler* handler) {
  buffer_.append(sp);
  return true;
}

bool AtsBaseFetch::HandleFlush( net_instaweb::MessageHandler* handler ) {
  buffer_.clear();
  return true;
}

void AtsBaseFetch::HandleHeadersComplete() {

}

void AtsBaseFetch::HandleDone(bool success) {
  Lock();
  done_called_ = true;
  Unlock();

  if (references_ < 2) {
    // The transformation died out from under us...we're SOL.
  }

  DecrefAndDeleteIfUnreferenced();
}

void AtsBaseFetch::Lock() {

}

void AtsBaseFetch::Unlock() {

}

void AtsBaseFetch::DecrefAndDeleteIfUnreferenced() {
  if (__sync_add_and_fetch(&references_, -1) == 0) {
      delete this;
  }
}

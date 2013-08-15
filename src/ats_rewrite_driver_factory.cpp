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


#include "ats_rewrite_driver_factory.h"
#include "ats_rewrite_options.h"
#include "ats_server_context.h"
#include "ats_native_fetcher.h"
#include <cassert>

#include "net/instaweb/apache/serf_url_async_fetcher.h"
#include "net/instaweb/http/public/content_type.h"
#include "net/instaweb/http/public/fake_url_async_fetcher.h"
#include "net/instaweb/http/public/wget_url_fetcher.h"
#include "net/instaweb/rewriter/public/rewrite_driver.h"
#include "net/instaweb/rewriter/public/rewrite_driver_factory.h"
#include "net/instaweb/rewriter/public/server_context.h"
#include "net/instaweb/rewriter/public/static_asset_manager.h"
#include "net/instaweb/system/public/system_caches.h"
#include "net/instaweb/util/public/google_message_handler.h"
#include "net/instaweb/util/public/null_shared_mem.h"
#include "net/instaweb/util/public/property_cache.h"
#include "net/instaweb/util/public/scheduler_thread.h"
#include "net/instaweb/util/public/posix_timer.h"
#include "net/instaweb/util/public/file_cache.h"
#include "net/instaweb/system/public/system_caches.h"
#include "net/instaweb/util/public/shared_circular_buffer.h"
#include "net/instaweb/util/public/shared_mem_statistics.h"
#include "net/instaweb/util/public/slow_worker.h"
#include "net/instaweb/util/public/pthread_shared_mem.h"
#include "net/instaweb/util/public/stdio_file_system.h"
#include "net/instaweb/util/public/string.h"
#include "net/instaweb/util/public/string_util.h"
#include "net/instaweb/util/public/thread_system.h"
#include "net/instaweb/util/public/scoped_ptr.h"
#include "pagespeed/kernel/base/string_util.h"

using namespace ats_pagespeed;

const char AtsRewriteDriverFactory::STATIC_ASSET_PREFIX[] = "/ats_pagespeed_static/";
int AtsRewriteDriverFactory::DEFAULT_THREAD_POOL_SIZE = 1;

AtsRewriteDriverFactory::AtsRewriteDriverFactory(AtsThreadSystem* ats_thread_system) :
    SystemRewriteDriverFactory(ats_thread_system),
    threads_started_(false),
    use_native_fetcher_(false),
    ats_thread_system_(ats_thread_system),
    ats_message_handler_(new AtsMessageHandler("ats_pagespeed")),
    ats_html_parse_message_handler_(new AtsMessageHandler("ats_pagespeed_html_parse"))
{
  LOG_ENTRY
  InitializeDefaultOptions();
  set_message_handler(ats_message_handler_);
  set_html_parse_message_handler(ats_html_parse_message_handler_);

  shared_mem_.reset(new net_instaweb::PthreadSharedMem());
  int thread_limit = 1; // Why Exactly..?

  caches_.reset(
        new net_instaweb::SystemCaches(this, shared_mem_.get(), thread_limit));

  InitStats(&stats_);
  SetStatistics(&stats_);
}

AtsRewriteDriverFactory::~AtsRewriteDriverFactory() {
  ShutDown();
}

net_instaweb::RewriteOptions* AtsRewriteDriverFactory::NewRewriteOptions() {
  AtsRewriteOptions* options = new AtsRewriteOptions();
  options->SetRewriteLevel(net_instaweb::RewriteOptions::kPassThrough);
  return options;
}

net_instaweb::Hasher* AtsRewriteDriverFactory::NewHasher() {
  return new net_instaweb::MD5Hasher();
}

net_instaweb::UrlFetcher* AtsRewriteDriverFactory::DefaultUrlFetcher() {
  assert(false);
  return NULL;
}

net_instaweb::UrlAsyncFetcher* AtsRewriteDriverFactory::DefaultAsyncUrlFetcher() {
  LOG_ENTRY

  const char* fetcher_proxy = "";

  net_instaweb::UrlAsyncFetcher* fetcher = NULL;
  if (use_native_fetcher_) {
    fetcher = new ats_pagespeed::AtsNativeAsyncFetcher();
  } else {
    fetcher = new net_instaweb::SerfUrlAsyncFetcher(
        fetcher_proxy,
        NULL,
        thread_system(),
        statistics(),
        timer(),
        2500,
        new net_instaweb::GoogleMessageHandler()); // LEAK LEAK LEAK :)

  }

  return fetcher;
}

void AtsRewriteDriverFactory::StartThreads() {
  if (threads_started_) {
    return;
  }

  net_instaweb::SchedulerThread* thread = new net_instaweb::SchedulerThread(thread_system(), scheduler());
  thread->Start();

  defer_cleanup(thread->MakeDeleter());
  threads_started_ = true;
}

net_instaweb::MessageHandler* AtsRewriteDriverFactory::DefaultHtmlParseMessageHandler() {
  return ats_html_parse_message_handler_;
}

net_instaweb::MessageHandler* AtsRewriteDriverFactory::DefaultMessageHandler() {
  return ats_message_handler_;
}

net_instaweb::FileSystem* AtsRewriteDriverFactory::DefaultFileSystem() {
  return new net_instaweb::StdioFileSystem();
}

net_instaweb::Timer* AtsRewriteDriverFactory::DefaultTimer() {
  // What happened to Google Timer?
  return new net_instaweb::PosixTimer();
}

void AtsRewriteDriverFactory::InitStats(net_instaweb::Statistics *stats) {
  LOG_ENTRY
  net_instaweb::RewriteDriverFactory::InitStats(stats);
  net_instaweb::SystemCaches::InitStats(stats);
  net_instaweb::SerfUrlAsyncFetcher::InitStats(stats);
  net_instaweb::FileCache::InitStats(stats);

}

net_instaweb::NamedLockManager* AtsRewriteDriverFactory::DefaultLockManager() {
  return NULL; // Should this not happen? In ngx_pagespeed you CHECK(false)?
}

net_instaweb::Statistics* AtsRewriteDriverFactory::MakeGlobalSharedMemStatistics(
    bool logging, int64 logging_interval_ms, const GoogleString& logging_file_base) {
  LOG_ENTRY
  //if (shared_mem_statistics_.get() == NULL) {
  //  shared_mem_statistics_.reset(AllocateAndInitSharedMemStatistics("global", logging, logging_interval_ms, logging_file_base));
  //}
  //SetStatistics(shared_mem_statistics_.get());
  //return shared_mem_statistics_.get();
  return NULL;
}

net_instaweb::SharedMemStatistics* AtsRewriteDriverFactory::AllocateAndInitSharedMemStatistics(
    const StringPiece& name, const bool logging,
    const int64 logging_interval_ms,
    const GoogleString& logging_file_base) {
  LOG_ENTRY
  net_instaweb::SharedMemStatistics* stats = new net_instaweb::SharedMemStatistics(
      logging_interval_ms, net_instaweb::StrCat(logging_file_base, name), logging,
      net_instaweb::StrCat(filename_prefix(), name), shared_mem_runtime(), message_handler(),
      file_system(), timer());
  InitStats(stats);
  stats->Init(true, message_handler());
  return stats;
}

net_instaweb::QueuedWorkerPool* AtsRewriteDriverFactory::CreateWorkerPool(net_instaweb::RewriteDriverFactory::WorkerPoolCategory pool,
                                           StringPiece name) {
  LOG_ENTRY
  TS_DEBUG(TAG, "Created new QueuedWorkerPool of type %d named '%s' of size %d", pool, name.data(), DEFAULT_THREAD_POOL_SIZE);
  net_instaweb::QueuedWorkerPool *q_pool = new net_instaweb::QueuedWorkerPool(DEFAULT_THREAD_POOL_SIZE, name, thread_system());
  return q_pool;
}

void AtsRewriteDriverFactory::SetupCaches(net_instaweb::ServerContext *server_context) {
  LOG_ENTRY
  caches_->SetupCaches(server_context);

  server_context->set_enable_property_cache(true);
}

void AtsRewriteDriverFactory::Init() {
  LOG_ENTRY

  for (AtsServerContextSet::iterator p = uninitialized_server_contexts_.begin(),
           e = uninitialized_server_contexts_.end(); p != e; ++p) {
    AtsServerContext* server_context = *p;
    caches_->RegisterConfig(server_context->config());
    server_context->Init();
  }

  caches_->RootInit();
}


void AtsRewriteDriverFactory::InitStaticAssetManager(net_instaweb::StaticAssetManager* static_asset_manager) {
  LOG_ENTRY
  static_asset_manager->set_library_url_prefix(STATIC_ASSET_PREFIX);
}

net_instaweb::ServerContext* AtsRewriteDriverFactory::NewServerContext() {
  LOG_ENTRY
  return NULL;
}


void AtsRewriteDriverFactory::ShutDown() {
  LOG_ENTRY

}


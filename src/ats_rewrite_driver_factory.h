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
#ifndef ATS_REWRITE_DRIVER_FACTORY_H_
#define ATS_REWRITE_DRIVER_FACTORY_H_

#include "ats_message_handler.h"
#include "ats_stats_variable.h"
#include "ats_stats.h"
#include "ats_thread_system.h"
#include <set>

#include "net/instaweb/system/public/system_rewrite_driver_factory.h"
#include "net/instaweb/system/public/system_caches.h"
#include "net/instaweb/util/public/md5_hasher.h"
#include "net/instaweb/util/public/scoped_ptr.h"
#include "net/instaweb/util/public/file_system.h"
#include "net/instaweb/util/public/shared_mem_statistics.h"
#include "net/instaweb/util/public/file_system_lock_manager.h"
#include "net/instaweb/http/public/content_type.h"
#include "net/instaweb/apache/serf_url_async_fetcher.h"
#include "net/instaweb/http/public/async_fetch.h"
#include "net/instaweb/http/public/fake_url_async_fetcher.h"
#include "net/instaweb/http/public/wget_url_fetcher.h"
#include "net/instaweb/rewriter/public/rewrite_driver.h"
#include "net/instaweb/rewriter/public/rewrite_driver_factory.h"
#include "net/instaweb/rewriter/public/rewrite_gflags.h"
#include "net/instaweb/util/public/lru_cache.h"
#include "net/instaweb/util/public/scoped_ptr.h"
#include "net/instaweb/util/public/md5_hasher.h"

namespace ats_pagespeed {

class AtsServerContext;

class AtsRewriteDriverFactory : public net_instaweb::SystemRewriteDriverFactory {
 public:
  typedef std::set<AtsServerContext *> AtsServerContextSet;

  static const char STATIC_ASSET_PREFIX[];

  // Override with DefaultThreadPoolSize in global config
  static int DEFAULT_THREAD_POOL_SIZE;

  explicit AtsRewriteDriverFactory(AtsThreadSystem* ats_thread_system);
  virtual ~AtsRewriteDriverFactory();

  virtual net_instaweb::Hasher* NewHasher();
  virtual net_instaweb::UrlFetcher* DefaultUrlFetcher();
  virtual net_instaweb::UrlAsyncFetcher* DefaultAsyncUrlFetcher();
  virtual net_instaweb::MessageHandler* DefaultHtmlParseMessageHandler();
  virtual net_instaweb::MessageHandler* DefaultMessageHandler();
  virtual net_instaweb::FileSystem* DefaultFileSystem();
  virtual net_instaweb::Timer* DefaultTimer();
  virtual net_instaweb::NamedLockManager* DefaultLockManager();
  virtual void InitStaticAssetManager(net_instaweb::StaticAssetManager* static_asset_manager);

  virtual net_instaweb::ServerContext* NewServerContext();

  virtual net_instaweb::RewriteOptions* NewRewriteOptions();

  virtual net_instaweb::QueuedWorkerPool* CreateWorkerPool(WorkerPoolCategory pool,
                                             StringPiece name);

  // Called from InitServerContext, but virtualized separately as it is
  // platform-specific.  This method must call on the server context:
  // set_http_cache, set_metadata_cache, set_filesystem_metadata_cache, and
  // MakePropertyCaches.
  virtual void SetupCaches(net_instaweb::ServerContext* server_context);

  net_instaweb::SystemCaches* caches() { return caches_.get(); }

  // Returns true if this platform uses beacon-based measurements to make
  // run-time decisions.  This is used to determine how to configure various
  // beacon-based filters.
  virtual bool UseBeaconResultsInFilters() const {
    return true;
  }

  net_instaweb::AbstractSharedMem* shared_mem_runtime() const {
    return shared_mem_.get();
  }

  // TODO: Move to use an ATS AsyncTimer
  void StartThreads();

  static void InitStats(net_instaweb::Statistics* statistics);


  void Init();

  net_instaweb::Statistics* MakeGlobalSharedMemStatistics( bool logging,
      int64 logging_interval_ms, const GoogleString& logging_file_base);

  net_instaweb::SharedMemStatistics* AllocateAndInitSharedMemStatistics(
      const StringPiece& name, const bool logging,
      const int64 logging_interval_ms,
      const GoogleString& logging_file_base);

  virtual void ShutDown();

  AtsMessageHandler* ats_message_handler() { return ats_message_handler_; }

  bool use_native_fetcher() {
    return use_native_fetcher_;
  }

  void set_use_native_fetcher(bool x) {
    use_native_fetcher_ = x;
  }

 private:
  bool threads_started_;
  bool use_native_fetcher_;
  AtsThreadSystem *ats_thread_system_;
  AtsMessageHandler *ats_message_handler_;
  AtsMessageHandler *ats_html_parse_message_handler_;
  scoped_ptr<net_instaweb::SystemCaches> caches_;
  scoped_ptr<net_instaweb::AbstractSharedMem> shared_mem_;
  scoped_ptr<net_instaweb::SharedMemStatistics> shared_mem_statistics_;
  AtsServerContextSet uninitialized_server_contexts_;
  AtsStats stats_;
};

} /* ats_pagespeed */

#endif /* ATS_REWRITE_DRIVER_FACTORY_H_ */

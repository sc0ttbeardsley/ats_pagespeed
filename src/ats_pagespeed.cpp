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


#include <atscppapi/GlobalPlugin.h>
#include <atscppapi/TransactionPlugin.h>
#include <atscppapi/TransformationPlugin.h>
#include <atscppapi/GzipInflateTransformation.h>
#include <atscppapi/GzipDeflateTransformation.h>
#include <atscppapi/Stat.h>
#include <atscppapi/PluginInit.h>
#include <atscppapi/Async.h>
#include <atscppapi/AsyncHttpFetch.h>
#include "ats_pagespeed.h"
#include "ats_pagespeed_logging.h"
#include "ats_message_handler.h"
#include "ats_stats.h"
#include "ats_stats_variable.h"
#include "ats_thread_system.h"
#include "ats_rewrite_driver_factory.h"
#include "ats_rewrite_options.h"
#include "ats_pagespeed_configuration.h"
#include "ats_request_context.h"
#include "ats_server_context.h"
#include <map>
#include <string>
#include <list>
#include "net/instaweb/http/public/request_headers.h"
#include "net/instaweb/http/public/response_headers.h"

#include <atscppapi/Mutex.h>
#include "net/instaweb/util/public/string_writer.h"
#include "net/instaweb/util/public/string.h"
#include "net/instaweb/util/public/string_util.h"

using namespace atscppapi;
using namespace atscppapi::transformations;
using namespace ats_pagespeed;
using namespace ats_pagespeed::config;
using namespace net_instaweb;
using std::string;
using std::map;

// TODO: add request level AtsRewriteOptions (query params/headers)

namespace ats_pagespeed {
GlobalConf global_config; // ats_pagespeed.h

// Writer implementation for directing HTML output to a string.
class ProtectedStringWriter : public Writer {
 public:
  explicit ProtectedStringWriter(GoogleString* str, atscppapi::Mutex *m) : string_(str), mutex_(m) { }
  virtual ~ProtectedStringWriter() { }
  virtual bool Write(const StringPiece& str, MessageHandler* message_handler) {
    mutex_->lock();
    string_ ->append(str.as_string());
    mutex_->unlock();
    return true;
  }

  virtual bool Flush(MessageHandler* message_handler) { return true; }
 private:
  GoogleString* string_;
  atscppapi::Mutex *mutex_;
};

class AtsPagespeedTransformationPlugin: public TransformationPlugin {
public:
  void Init() {
    request_context_.reset(new AtsRequestContext(rule_.server_context->thread_system()->NewMutex(),
                       NULL));
    request_context_->set_using_spdy(false);

    write_mutex_.reset(new atscppapi::Mutex());
    string_writer_.reset(new ProtectedStringWriter(&buffer_, write_mutex_.get()));

    rewrite_driver_ = rule_.server_context->NewRewriteDriver(request_context_);
    LOG_DEBUG("Using rewrite driver %p", rewrite_driver_);
    rewrite_driver_->set_size_limit(1*1024*1024); // 1 mb
    rewrite_driver_->set_fully_rewrite_on_flush(true);
    rewrite_driver_->SetWriter(string_writer_.get());
    if(!rewrite_driver_->StartParse(transaction_.getClientRequest().getUrl().getUrlString())) {
      LOG_ERROR("Failure starting html parse on url %s", transaction_.getClientRequest().getUrl().getUrlString().c_str());
      rule_.server_context->ReleaseRewriteDriver(rewrite_driver_);
      rewrite_driver_ = NULL;
    }
  }

  AtsPagespeedTransformationPlugin(Transaction &transaction, const config::RegexRule &rule) :
      TransformationPlugin(transaction, RESPONSE_TRANSFORMATION),
      completed_cleanly_(false), done_send_resp_headers_(false), have_resp_headers_(false),
      bytes_written_(0), bytes_received_(0), rule_(rule), transaction_(transaction) {

    buffer_.reserve(RESERVE_SIZE);

    Init();
  }

  void consume(const string &data) {
    if (!rewrite_driver_) {
     return;
    }

    bytes_received_ += data.length();

    rewrite_driver_->ParseText(data);
    {
      atscppapi::ScopedMutexLock(*write_mutex_.get());
      if (buffer_.size()) {
        bytes_written_ += buffer_.size();
        LOG_DEBUG("Pagespeed produced %d bytes of output", static_cast<int>(buffer_.size()));
        produce(buffer_);
        buffer_.clear();
      }
    }
  }

  void handleInputComplete() {
    if (bytes_received_ > 0) {
      rewrite_driver_->FinishParse();
      completed_cleanly_ = true;

      {
        atscppapi::ScopedMutexLock(*write_mutex_.get());
        if (buffer_.size()) {
          bytes_written_ += buffer_.size();
          LOG_DEBUG("Pagespeed produced a final %d bytes of output",
              static_cast<int>(buffer_.size()));
          produce(buffer_);
          buffer_.clear();
        }
      }
    }

    int64_t total_bytes_written = setOutputComplete();
    if (total_bytes_written != bytes_written_) {
      LOG_ERROR("SetOutputComplete sanity check failed, bytes_written %d != %d",
          static_cast<int>(bytes_written_), static_cast<int>(total_bytes_written));
    }
    

    LOG_DEBUG("Pagespeed received %d bytes and produced a total of %d bytes.",
        static_cast<int>(bytes_received_), static_cast<int>(bytes_written_));
  }


  virtual ~AtsPagespeedTransformationPlugin() {
    LOG_DEBUG("Transformation is dead...");
    //if (rewrite_driver_)
    //  delete rewrite_driver_;

    if (rewrite_driver_ && !completed_cleanly_) {
      rewrite_driver_->FinishParse();
    }

  }
private:
  bool completed_cleanly_;
  bool done_send_resp_headers_;
  bool have_resp_headers_;
  string buffer_;
  scoped_ptr<atscppapi::Mutex> write_mutex_;
  RefCountedPtr<RequestContext> request_context_;
  scoped_ptr<ProtectedStringWriter> string_writer_;
  scoped_ptr<RequestHeaders> request_headers_;
  scoped_ptr<ResponseHeaders> response_headers_;
  RewriteDriver *rewrite_driver_; // These are managed within server context (don't try to delete).
  string content_encoding_;
  int64_t bytes_written_;
  int64_t bytes_received_;
  const config::RegexRule &rule_;
  static const size_t RESERVE_SIZE = 1024 * 256; // 256KB
  Transaction &transaction_;
};

class AtsPagespeedSetupTransactionPlugin: public TransactionPlugin {
public:
  AtsPagespeedSetupTransactionPlugin(Transaction &transaction, const config::RegexRule &regex_rule) :
      TransactionPlugin(transaction), server_provided_gzip_(false), regex_rule_(regex_rule) {
    registerHook(HOOK_READ_RESPONSE_HEADERS);
  }

  static inline bool clientAcceptsGzip(Transaction &transaction) {
    return transaction.getClientRequest().getHeaders().getJoinedValues("Accept-Encoding").find(
        "gzip") != string::npos;
  }

  static inline bool serverReturnedGzip(Transaction &transaction) {
    return transaction.getServerResponse().getHeaders().getJoinedValues("Content-Encoding").find(
        "gzip") != string::npos;
  }


  virtual void handleReadResponseHeaders(Transaction &transaction) {
    if (transaction.getServerResponse().getHeaders().getJoinedValues("content-type").find("text/html") == string::npos) {
      transaction.resume();
      return;
    }

    // We're guaranteed to have been returned either gzipped content or Identity (see HandleSendResponseHeaders).
    server_provided_gzip_ = serverReturnedGzip(transaction);
    //string pre_remap_url = transaction.getClientRequest().getPristineUrl().getUrlString();
    string post_remap_url = transaction.getClientRequest().getUrl().getUrlString();

     // Eventually this logic should just move to let PSOL handle it.
    if (server_provided_gzip_) {
     LOG_DEBUG("Creating inflate transform on url '%s'", post_remap_url.c_str());
     transaction.addPlugin(new GzipInflateTransformation(transaction, TransformationPlugin::RESPONSE_TRANSFORMATION));
    }

    LOG_DEBUG("Creating ats pagespeed transform on url '%s'", post_remap_url.c_str());
    transaction.addPlugin(new AtsPagespeedTransformationPlugin(transaction, regex_rule_));

    if (server_provided_gzip_) {
     LOG_DEBUG("Creating deflate transformation on url '%s'", post_remap_url.c_str());
     transaction.addPlugin(new GzipDeflateTransformation(transaction, TransformationPlugin::RESPONSE_TRANSFORMATION));
    }

    transaction.resume();
  }

private:
  bool server_provided_gzip_;
  const config::RegexRule &regex_rule_;
};

class AtsPagespeedGlobalPlugin: public GlobalPlugin {
public:
  AtsPagespeedGlobalPlugin() {
    registerHook(HOOK_SEND_REQUEST_HEADERS);
  }

  virtual void handleSendRequestHeaders(Transaction &transaction) {
    if (transaction.getClientRequest().getMethod() != HTTP_METHOD_GET || transaction.isInternalRequest())
      transaction.resume();

    string pre_remap_url = transaction.getClientRequest().getPristineUrl().getUrlString();
    string post_remap_url = transaction.getClientRequest().getUrl().getUrlString();
    config::ListRegexRules::iterator first_matching_rule = std::find_if(
        config::PagespeedRegexRules.begin(), config::PagespeedRegexRules.end(),
        config::MatchesRegexRule(pre_remap_url, post_remap_url));

    if (first_matching_rule != config::PagespeedRegexRules.end() && !(first_matching_rule)->disable_pagespeed) {
      // Since we can only decompress gzip we will change the accept encoding header
      // to gzip, even if the user cannot accept gziped content we will return to them
      // uncompressed content in that case since we have to be able to transform the content.
      // Supporting deflate is pointless.
      string original_accept_encoding = transaction.getServerRequest().getHeaders().getJoinedValues(
          "Accept-Encoding");

      // Make sure it's done on the server request to avoid clobbering the clients original accept encoding header.
      if (AtsPagespeedSetupTransactionPlugin::clientAcceptsGzip(transaction)) {
        transaction.getServerRequest().getHeaders().set("Accept-Encoding", "gzip");
        LOG_DEBUG(
          "Changed the outgoing server request accept encoding header from \"%s\" to \"gzip\"",
          original_accept_encoding.c_str());
      } else {
        transaction.getServerRequest().getHeaders().erase("Accept-Encoding");
        LOG_DEBUG(
          "Changed the outgoing server request accept encoding header from \"%s\" to \"\"",
          original_accept_encoding.c_str());
      }
      transaction.addPlugin(
          new AtsPagespeedSetupTransactionPlugin(transaction, *first_matching_rule));
    }

    transaction.resume();
  }
};

}

void TSPluginInit(int argc, const char *argv[]) {
#ifdef GPERFTOOLS
  HeapProfilerStart("/tmp/ats_pagespeed.gperf");
#endif

  LOG_DEBUG("Ats Pagespeed: TSPluginInit");

  // Pagespeed Initialization
  ats_pagespeed::AtsRewriteOptions::Initialize();
  ats_pagespeed::AtsRewriteDriverFactory::Initialize();

  global_config.driver_factory = new AtsRewriteDriverFactory(new AtsThreadSystem());
  global_config.handler = new AtsMessageHandler("ats_pagespeed");
  global_config.options = new AtsRewriteOptions();

  if (!ats_pagespeed::config::loadConfig(argc, const_cast<char**>(argv))) {
    // No config file or bad config causes us to not start up..
    LOG_ERROR("Unable to start ats_pagespeed due to bad config.");
  } else {
    bool loadable = true;

    global_config.driver_factory->Init();

    if (loadable) {
      new ats_pagespeed::AtsPagespeedGlobalPlugin();
    } else
      LOG_ERROR("Unable to start ats_pagespeed!");
  }
}


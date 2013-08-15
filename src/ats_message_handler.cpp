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
#include "ats_message_handler.h"

using namespace ats_pagespeed;
using std::string;

AtsMessageHandler::AtsMessageHandler(const string &prefix) :
    prefix_(prefix) { }


void AtsMessageHandler::MessageVImpl(net_instaweb::MessageType type, const char* msg, va_list args) {
  switch (type) {
    case net_instaweb::kInfo:
      TS_DEBUG(TAG, "[%s INFO] %s", prefix_.c_str(), format(msg, args).c_str());
      break;
    case net_instaweb::kWarning:
      TS_DEBUG(TAG, "[%s WARNING] %s", prefix_.c_str(), format(msg, args).c_str());
      break;
    case net_instaweb::kError:
      TS_ERROR(TAG, "[%s ERROR] %s", prefix_.c_str(), format(msg, args).c_str());
      break;
    case net_instaweb::kFatal:
      TS_ERROR(TAG, "[%s FATAL] %s", prefix_.c_str(), format(msg, args).c_str());
      break;
  }
}

void AtsMessageHandler::FileMessageVImpl(net_instaweb::MessageType type, const char* file,
                                            int line, const char* msg, va_list args) {
  switch (type) {
    case net_instaweb::kInfo:
     TS_DEBUG(TAG, "[%s INFO] (%s:%d) %s", prefix_.c_str(), file, line, format(msg, args).c_str());
     break;
    case net_instaweb::kWarning:
     TS_DEBUG(TAG, "[%s WARNING] (%s:%d) %s", prefix_.c_str(), file, line, format(msg, args).c_str());
     break;
    case net_instaweb::kError:
     TS_ERROR(TAG, "[%s ERROR] (%s:%d) %s", prefix_.c_str(), file, line, format(msg, args).c_str());
     break;
    case net_instaweb::kFatal:
     TS_ERROR(TAG, "[%s FATAL] (%s:%d) %s", prefix_.c_str(), file, line, format(msg, args).c_str());
     break;
  }
}

string AtsMessageHandler::format(const char* msg, va_list args) {
  string buffer;

  StringAppendV(&buffer, msg, args);
  return buffer;
}

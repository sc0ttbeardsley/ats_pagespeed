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
#ifndef ATS_MESSAGE_HANDLER_H_
#define ATS_MESSAGE_HANDLER_H_
#include <string>
#include "ats_pagespeed_logging.h"

#include "net/instaweb/util/public/basictypes.h"
#include "net/instaweb/util/public/message_handler.h"
#include "net/instaweb/util/public/string.h"

namespace ats_pagespeed {
class AtsMessageHandler : public net_instaweb::MessageHandler {
public:
  AtsMessageHandler(const std::string &prefix);

protected:
  virtual void MessageVImpl(net_instaweb::MessageType type, const char* msg, va_list args);
  virtual void FileMessageVImpl(net_instaweb::MessageType type, const char* file,
                                              int line, const char* msg, va_list args);
private:
  std::string prefix_;
  std::string format(const char* msg, va_list args);
};

} /* ats_pagespeed */



#endif /* ATS_MESSAGE_HANDLER_H_ */
